#ifndef IMAGEDOWNLOADER_IMAGEDONWLOADERIMPL_H
#define IMAGEDOWNLOADER_IMAGEDONWLOADERIMPL_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

namespace imagedownloader {

class ImageDownloaderImpl : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloaderImpl(QObject *parent = nullptr);

public:
    void downloadImage(const QUrl& url);

signals:
    void downloadProgress(double progress);

private slots:
    void onRequestFinished(QNetworkReply* reply);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager* mNetworkAccessManager{ nullptr };
};

} // namespace imagedownloader

#endif // IMAGEDOWNLOADER_IMAGEDONWLOADERIMPL_H
