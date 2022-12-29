#ifndef FSWATCHER_FSWATCHERINTERFACE_H
#define FSWATCHER_FSWATCHERINTERFACE_H

#include "filesystemwatcherlib_global.h"

#include <QObject>

namespace fswatcher {

class FSWatcherImpl;
class FSWatcherModel;
class FSEventWatcherModel;

class FILESYSTEMWATCHERLIB_EXPORT FSWatcherInterface : public QObject
{
    Q_OBJECT
public:
    explicit FSWatcherInterface(QObject* parent = nullptr);

    FSWatcherModel* fsWatcherModel();
    FSEventWatcherModel* fsEventWatcherModel();

signals:
    void downloadImage(const QUrl& url);

private:
    FSWatcherImpl* mImpl{ nullptr };
};

} // namespace fswatcher

#endif // FSWATCHER_FSWATCHERINTERFACE_H
