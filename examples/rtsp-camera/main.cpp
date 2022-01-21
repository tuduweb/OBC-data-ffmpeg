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

#include <QStringList>
#include <QDebug>
#include <QTime>

#include <QWidget>
#include <QApplication>
#include <QVBoxLayout>

#define TIMEMS      qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))

#include "RTSPCameraStream.hpp"

#include "RTSPThread.hpp"

#include "qobject.h"
#include "qlabel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    RTSPCameraStream* stream = new RTSPCameraStream();

    RTSPThread* thread = new RTSPThread();

    thread->start();

    QWidget w;
    w.resize(500,400);
    QVBoxLayout* layout = new QVBoxLayout;
    w.setLayout(layout);
    w.show();

    QLabel* label = new QLabel("image");
    layout->addWidget(label);

    QObject::connect(thread, &RTSPThread::NewFrameArrived, &w, [=](const QImage& image){
        label->setPixmap(QPixmap::fromImage(image));
    });

    return app.exec();

    return 0;

}
