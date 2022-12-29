#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QThread>

#include "fswatcherinterface.h"
#include "models/fswatchermodel.h"
#include "models/fseventwatchermodel.h"

#include "imagedownloader.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qDebug() << "Main Thread: " << QThread::currentThread();

    /* File Watcher */
    fswatcher::FSWatcherInterface fswatcher_interface;

    /* Image Downloader */
    imagedownloader::ImageDownloader image_downloader;
    QThread t_image_downloader;
    image_downloader.moveToThread(&t_image_downloader);

    /* Connections: FileWatcher -> ImageDownloader */
    QObject::connect(&fswatcher_interface, &fswatcher::FSWatcherInterface::downloadImage,
                     &image_downloader, &imagedownloader::ImageDownloader::onDownloadImage,
                     Qt::QueuedConnection);

    /* Connections: QApplication */
    QObject::connect(&app, &QGuiApplication::aboutToQuit,
                     [&t_image_downloader] {
        t_image_downloader.quit();
        t_image_downloader.wait();
    });

    /* UI */
    QQuickView view;

    QQmlContext* root_context = view.rootContext();
    root_context->setContextProperty("FSWatcherModel", fswatcher_interface.fsWatcherModel());
    root_context->setContextProperty("FSEventWatcherModel", fswatcher_interface.fsEventWatcherModel());

    view.setSource(QUrl{QStringLiteral("qrc:/main.qml")});
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.resize(640, 480);
    view.setTitle("File Watcher");


    t_image_downloader.start();
    view.show();
    return app.exec();
}
