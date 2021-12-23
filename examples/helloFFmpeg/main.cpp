#include <stdio.h>
#define __STDC_CONSTANT_MACROS
#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/log.h>

#ifdef __cplusplus
};
#endif

#include <QStringList>
#include <QDebug>
#include <QTime>

#define TIMEMS      qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))


int main()
{
    av_log_set_level(AV_LOG_DEBUG);//
    av_log(NULL,AV_LOG_INFO,"...%s\n","hello");
    
    av_register_all();
    avformat_network_init();
    
    //输出所有支持的解码器名称
    QStringList listCodeName;
    AVCodec* code = av_codec_next(NULL);
    while (code != NULL) {
        listCodeName << code->name;
        code = code->next;
    }

    qDebug() << TIMEMS << listCodeName;
    return 0;
}
