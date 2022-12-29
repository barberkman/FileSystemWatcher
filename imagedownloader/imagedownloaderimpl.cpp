#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QRandomGenerator>
#include <QDebug>

#include "imagedownloaderimpl.h"

namespace imagedownloader {

ImageDownloaderImpl::ImageDownloaderImpl(QObject *parent)
    : QObject{parent},
      mNetworkAccessManager{ new QNetworkAccessManager{this} }
{
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished,
            this, &ImageDownloaderImpl::onRequestFinished);
}

void ImageDownloaderImpl::downloadImage(const QUrl &url)
{
    QNetworkRequest request{url};
    QNetworkReply* reply = mNetworkAccessManager->get(request);
    connect(reply, &QNetworkReply::downloadProgress,
            this, &ImageDownloaderImpl::onDownloadProgress);
}

void ImageDownloaderImpl::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qWarning() << "Download error: " << reply->errorString();
        return;
    }

    /* Check and create download directory */
    QDir download_dir{ QDir::currentPath() + "/cats" };
    if (!download_dir.exists()) {
        download_dir.mkdir(download_dir.path());
    }

    QString download_path = download_dir.path() + "/catimage_" + QString::number(QRandomGenerator::global()->generate()) + ".png";
    QFile file{ download_path };
    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "File could not opened";
        return;
    }
    file.write(reply->readAll());
    file.close();
}

void ImageDownloaderImpl::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    double progress = static_cast<double>(bytesReceived) / bytesTotal;
    emit downloadProgress(progress);
}

} // namespace imagedownloader
