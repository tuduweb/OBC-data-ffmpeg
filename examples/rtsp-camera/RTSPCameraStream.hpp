#pragma once
#include "StreamInterface.hpp"

#include <stdio.h>
#define __STDC_CONSTANT_MACROS
#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>

#ifdef __cplusplus
};
#endif

#include "QJsonObject.h"


#include "qdebug.h"

class RTSPCameraStream : public StreamInterface
{
    public:
        RTSPCameraStream() { qDebug() << "hello c"; }
        int StreamInit() override;
        int StreamStart() override;

    protected:
        

    protected:
        AVFormatContext* pFormatCtx = NULL;
        AVDictionary* avdic = NULL;

    private:
        QThread* _rtspThread;

};