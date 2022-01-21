#include "RTSPThread.hpp"

#include "qdebug.h"

RTSPThread::RTSPThread(QObject *parent) : QThread(parent)
{
    qDebug() << "hello thread";

    isRunning = true;

}

void RTSPThread::initLib()
{
    av_log_set_level(AV_LOG_DEBUG);//
    av_log(NULL,AV_LOG_INFO,"...%s\n","hello");
    
    av_register_all();
    avformat_network_init();
}

void RTSPThread::run()
{


	//申请 AVFormatContext 结构体
	//AVFormatContext* pFormatCtx = avformat_alloc_context();
	pFormatCtx = avformat_alloc_context();
	if (pFormatCtx == nullptr) {
		qDebug() << "alloc av format error!\n";
		return;
	}

	//AVDictionary* avdic = NULL;
	//使用TCP的方式 使内网下也可以穿透
	av_dict_set(&avdic, "rtsp_transport", "tcp", 0);
	av_dict_set(&avdic, "buffer_size", "8192000", 0);
	//char option_key2[] = "max_delay";
	//char option_value2[] = "100";
	//av_dict_set(&avdic, option_key2, option_value2, 0);

	//设置超时断开连接时间,单位微秒,3000000表示3秒
	av_dict_set(&avdic, "stimeout", "3000000", 0);
	//设置最大时延,单位微秒,1000000表示1秒
	av_dict_set(&avdic, "max_delay", "1000000", 0);
	//自动开启线程数
	av_dict_set(&avdic, "threads", "auto", 0);

	av_dict_set(&avdic, "stimeout", "5000000", 0);//5s

    
    QString url("rtsp://admin:admin123@172.28.140.140");

	//打开媒体文件
	if (avformat_open_input(&pFormatCtx, url.toStdString().c_str(), NULL, &avdic) != 0) {
		qDebug("can't open the file. \n");
        return;
	}

		//查找解码器
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		qDebug("Could't find stream infomation.\n");
		return;
	}

	//查找解码器参数
	int videoStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
		}
	}
	if (videoStream == -1) {
		qDebug("Didn't find a video stream.\n");
		return;
	}

	//申请解码器
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(NULL);
	//从AVFormatContext设置解码器参数
	avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
	//查找解码器
	AVCodec* pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	//设置解码器参数
	pCodecCtx->bit_rate = 0;
	pCodecCtx->time_base.num = 1; //下面两行：一秒钟25帧
	pCodecCtx->time_base.den = 10;
	pCodecCtx->frame_number = 1;  //每包一个视频帧

	if (pCodec == NULL) {
		qDebug("Codec not found.\n");
		return;
	}

	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		qDebug("Could not open codec.\n");
		return;
	}

	//初始化SwsContext 设置转换为 AV_PIX_FMT_RGB32
	struct SwsContext* img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
		AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);//SWS_FAST_BILINEAR//SWS_BICUBIC

	//申请内存
	AVFrame* pFrame = av_frame_alloc();
	AVFrame* pFrameRGB = av_frame_alloc();
	uint8_t* out_buffer = (uint8_t*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1) * sizeof(uint8_t));

	av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);

	AVPacket* packet = (AVPacket*)malloc(sizeof(AVPacket)); //分配一个packet
	av_new_packet(packet, pCodecCtx->width * pCodecCtx->height); //分配packet的数据
    
	static int errorCnt = 0;
	while (isRunning)
	{
        if(isPaused) {
            QThread::usleep(10000);
            continue;
        }
		//从stream中获取一帧率数据
		if(av_read_frame(pFormatCtx, packet) < 0) {
			break;
		}

		if (packet->stream_index == videoStream) {

			//送入解码器
			int ret = avcodec_send_packet(pCodecCtx, packet);
			int got_picture = avcodec_receive_frame(pCodecCtx, pFrame);

			if (ret < 0) {
				errorCnt++;
				qDebug() << QString("decode error. cnt %1").arg(errorCnt);
				goto _next;
				//break;
			}

			if (!got_picture) {
				//颜色空间转换，最后输出到out_buffer
				sws_scale(img_convert_ctx, (uint8_t const* const*)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
					pFrameRGB->linesize);
				// if (callback != NULL) {
				// 	//Do something you need...
				// 	callback((uchar*)out_buffer, pCodecCtx->width, pCodecCtx->height);

				// }
				//////////需要修改成更合理的模式
				QImage tmmImage(out_buffer, pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB888);
				QImage image = tmmImage.copy();
				emit NewFrameArrived(image);
                qDebug() << image;
                //TODO: 加入统计方法
			}
		}
	_next:

		av_packet_unref(packet);
		QThread::usleep(500);
	}

	//RemoteLog("释放资源中...");

	av_free(out_buffer);
	av_free(pFrame);
	av_free(pFrameRGB);

	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	sws_freeContext(img_convert_ctx);


}

int RTSPThread::setUrl(const QString& url)
{
    _url = url;
    return 0;
}