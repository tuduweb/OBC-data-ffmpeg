#include "RTSPCameraStream.hpp"

int RTSPCameraStream::StreamInit()
{
	/* 初始化的数据 */
	QJsonObject obj;

    /* -- 单独封装到库中 -- */

    av_log_set_level(AV_LOG_DEBUG);//
    av_log(NULL,AV_LOG_INFO,"...%s\n","hello");
    
    av_register_all();
    avformat_network_init();
    
	//申请 AVFormatContext 结构体
	//AVFormatContext* pFormatCtx = avformat_alloc_context();
	pFormatCtx = avformat_alloc_context();
	if (pFormatCtx == nullptr) {
		qDebug() << "alloc av format error!\n";
		return -1;
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

}

int RTSPCameraStream::StreamStart()
{

    QString url("rtsp://28.140.140");

	//打开媒体文件
	if (avformat_open_input(&pFormatCtx, url.toStdString().c_str(), NULL, &avdic) != 0) {
		qDebug("can't open the file. \n");
        return -1;
	}

		//查找解码器
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		qDebug("Could't find stream infomation.\n");
		return -1;
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
		return -1;
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
		return -1;
	}

	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		qDebug("Could not open codec.\n");
		return -1;
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

	


	return 0;
}