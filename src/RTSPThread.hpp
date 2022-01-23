#pragma once
#include <QObject>
#include <QThread>

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

#include "qimage.h"

class RTSPThread : public QThread
{
    Q_OBJECT
public:
    explicit RTSPThread(QObject *parent = nullptr);
    static void initLib();

    int setUrl(const QString& url);
    int setPause() { isPaused = true; return 0; };
    int setStart() { isPaused = false; return 0; };

protected:
    void run() override;

signals:
    void NewFrameArrived(const QImage& image);

protected:
    AVFormatContext* pFormatCtx = NULL;
    AVDictionary* avdic = NULL;

private:
    QString _url;
    bool isRunning = false;
    bool isPaused = false;
};