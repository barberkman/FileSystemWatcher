#ifndef FSWATCHER_FSWATCHERIMPL_H
#define FSWATCHER_FSWATCHERIMPL_H

#include <QObject>

namespace fswatcher {

class FSWatcherManager;
class FSWatcherModel;
class FSEventWatcherModel;

class FSWatcherImpl : public QObject
{
    Q_OBJECT
public:
    FSWatcherImpl(QObject* parent = nullptr);

    FSWatcherModel* fsWatcherModel();
    FSEventWatcherModel* fsEventWatcherModel();

signals:
    void downloadImage(const QUrl& url);

private:
    FSWatcherManager* mFSWatcherManager{ nullptr };
    FSWatcherModel* mFSWatcherModel{ nullptr };
    FSEventWatcherModel* mFSEventWatcherModel{ nullptr };

    static const QUrl catImageURL;
};

} // namespace fswatcher

#endif // FSWATCHER_FSWATCHERIMPL_H
