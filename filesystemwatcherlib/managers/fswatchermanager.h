#ifndef FSWATCHERMANAGER_H
#define FSWATCHERMANAGER_H

#include <vector>
#include <memory>

#include <QObject>
#include <QFileSystemWatcher>

#include "components/fsnode.h"
#include "helpers/fshelper.h"
#include "entities/fsevent.h"

namespace fswatcher {

class FSWatcherManager : public QObject
{
    Q_OBJECT
public:
    enum class WatchStatus : int {
        WATCHING = 0,
        IDLE = 1
    };

    FSWatcherManager(QObject* parent = nullptr);

    int watchedFolderCount() const;

    QString folderPath(int index) const;

    void addFolder(const QUrl& url);

    void removeFolder(int index);

    void clearFolders();

    void startWatching();

    void stopWatching();

    WatchStatus status() const;
    void setStatus(WatchStatus newStatus);

signals:
    void fsEvent(const fswatcher::FSEvent& event);

private slots:
    void onDirectoryChanged(const QString& path);
    void onFileChanged(const QString& path);

private:
    void directoryChangedAction(const QString& path);
    void fileChangedAction(const QString& path);

    void watchFolder(FSNode* root_folder);

private:
    QFileSystemWatcher* mFileSystemWatcher{ nullptr };
    std::unique_ptr<FSHelper> mFSHelper;
    std::vector<std::unique_ptr<FSNode>> mFSNodes;
    QMetaObject::Connection mFileChangedConnection;
    QMetaObject::Connection mDirectoryChangedConnection;

    WatchStatus mStatus{ WatchStatus::IDLE };
};

} // namespace fswatcher

#endif // FSWATCHERMANAGER_H
