#include <QDebug>

#include "imagedownloader.h"
#include "imagedownloaderimpl.h"

namespace imagedownloader {

ImageDownloader::ImageDownloader(QObject *parent)
    : QObject{ parent },
      mImpl{ new ImageDownloaderImpl{this} }
{
}

void ImageDownloader::onDownloadImage(const QUrl &url)
{
    mImpl->downloadImage(url);
}

} // namespace imagedownloader
