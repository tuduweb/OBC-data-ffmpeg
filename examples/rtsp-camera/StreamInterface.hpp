#pragma once

#include <QObject>
#include <QImage>
#include <QByteArray>


class StreamInterface : public QObject
{
    //Q_OBJECT

    public:
        enum StreamType
        {
            CameraStream = 1,
            DataStream = 2
        };
        Q_ENUM(StreamType);

    public:
        StreamInterface() {};
        ~StreamInterface() {};

    public:
        virtual int StreamInit() = 0;
        virtual int StreamStart() = 0;
        // int StreamPause();
        // int StreamStop();
        // int StreamDeinit();
        // int StreamDestory();

    signals:
        void FrameReceived(const QImage& frameImage);
        void DataReceived(const QByteArray& byteArray);
};