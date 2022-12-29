#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>

#include "imagedownloader_global.h"

namespace imagedownloader {

class ImageDownloaderImpl;

class IMAGEDOWNLOADER_EXPORT ImageDownloader : public QObject
{
    Q_OBJECT
public:
    ImageDownloader(QObject* parent = nullptr);

public slots:
    void onDownloadImage(const QUrl& url);

private:
    ImageDownloaderImpl* mImpl{ nullptr };
};

} // namespace imagedownloader

#endif // IMAGEDOWNLOADER_H
