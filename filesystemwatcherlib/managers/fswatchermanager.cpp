#include <exception>

#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QTimer>
#include <QThread>
#include <QDebug>

#include "fswatchermanager.h"
#include "components/fsfolder.h"
#include "components/fsfile.h"

namespace fswatcher {

FSWatcherManager::FSWatcherManager(QObject *parent)
    : QObject{ parent },
      mFileSystemWatcher{ new QFileSystemWatcher{this} },
      mFSHelper{ std::make_unique<FSHelper>() }
{
    mFileChangedConnection = connect(mFileSystemWatcher, &QFileSystemWatcher::fileChanged,
                                     this, &FSWatcherManager::onFileChanged);
    mDirectoryChangedConnection = connect(mFileSystemWatcher, &QFileSystemWatcher::directoryChanged,
                                          this, &FSWatcherManager::onDirectoryChanged);
}

int FSWatcherManager::watchedFolderCount() const
{
    return static_cast<int>(mFSNodes.size());
}

QString FSWatcherManager::folderPath(int index) const
{
    return mFSNodes.at(index)->path();
}

void FSWatcherManager::addFolder(const QUrl &url)
{
    if (url.isEmpty()) { throw std::runtime_error{"Path is empty"}; }

    QString path = url.toString();
    if (url.isLocalFile()) { path = url.toLocalFile(); }

    QDir root_dir{ path };
    if (!root_dir.exists()) { throw std::runtime_error{"Root dir is not exist"}; }

    for (auto& node : mFSNodes) {
        if (node->findNode(path)) {
            throw std::runtime_error{"Path already in watching folders"};
        }
    }

    std::unique_ptr<FSFolder> root_folder = std::make_unique<FSFolder>(path);

    if (mStatus == WatchStatus::WATCHING) {
        watchFolder(root_folder.get());
    }

    mFSNodes.push_back(std::move(root_folder));
}

void FSWatcherManager::removeFolder(int index)
{
    qDebug() << "FSWatcherManager::removeFolder: " << index;

    mFileSystemWatcher->removePaths( mFSNodes.at(index)->paths() );
    mFSNodes.erase(mFSNodes.begin() + index);

    if (mFSNodes.size() == 0) {
        setStatus(WatchStatus::IDLE);
    }
}

void FSWatcherManager::clearFolders()
{
    qDebug() << "FSWatcherManager::clearFolders";

    stopWatching();
    mFSNodes.clear();
}

void FSWatcherManager::startWatching()
{
    qDebug() << "FSWatcherManager::startWatching";

    if (mStatus == WatchStatus::WATCHING)
        return;

    stopWatching();
    for (auto& node : mFSNodes) {
        watchFolder(node.get());
    }

    setStatus(WatchStatus::WATCHING);
}

void FSWatcherManager::stopWatching()
{
    qDebug() << "FSWatcherManager::stopWatching";

    if (mStatus == WatchStatus::IDLE)
        return;


    for (auto& node : mFSNodes) {
        mFileSystemWatcher->removePaths(node->paths());
    }

    setStatus(WatchStatus::IDLE);
}

void FSWatcherManager::onDirectoryChanged(const QString &path)
{
    /*
     * QTBUG-7905
     * https://bugreports.qt.io/browse/QTBUG-7905
     * https://stackoverflow.com/questions/25658123/qfilesystemwatcher-locks-directory-on-windows
    */
    disconnect(mDirectoryChangedConnection);

    // File system can't finish operation before checks.
    // We have to create an async delay
    QTimer::singleShot(100, [path, this] {
        directoryChangedAction(path);
        mDirectoryChangedConnection = connect(mFileSystemWatcher, &QFileSystemWatcher::directoryChanged,
                                              this, &FSWatcherManager::onDirectoryChanged);
    });
}

void FSWatcherManager::onFileChanged(const QString &path)
{
    disconnect(mFileChangedConnection);

    // File system can't finish operation before checks.
    // We have to create an async delay
    QTimer::singleShot(100, [path, this] {
        fileChangedAction(path);
        mFileChangedConnection = connect(mFileSystemWatcher, &QFileSystemWatcher::fileChanged,
                                         this, &FSWatcherManager::onFileChanged);
    });
}

void FSWatcherManager::directoryChangedAction(const QString &path)
{
    /* Find Node */
    FSNode* root_node{ nullptr };
    for (auto& node : mFSNodes) {
        FSNode* temp_node = node->findNode(path);
        if (temp_node) {
            root_node = temp_node;
            break;
        }
    }

    if (!root_node) {
        qWarning() << "FileWatcher::onDirectoryChanged: root node not found";
        return;
    }

    /* Check directory removed */
    auto response_directory_removed = mFSHelper->isDirectoryRemoved(path);
    if (response_directory_removed.first) {
        mFileSystemWatcher->removePath(response_directory_removed.second);
        root_node->removeNonrecursiveNode(response_directory_removed.second);
        FSEvent event{ FSEvent::EventType::REMOVED, response_directory_removed.second, true };
        emit fsEvent(event);
        return;
    }

    /* Check file created */
    auto response_file_created = mFSHelper->isFileCreated(path, root_node);
    if (response_file_created.first) {
        auto file = std::make_unique<FSFile>(response_file_created.second);
        file->setRoot(root_node);
        mFileSystemWatcher->addPath(file->path());
        root_node->addNode(std::move(file));
        FSEvent event{ FSEvent::EventType::CREATED, response_file_created.second, false };
        emit fsEvent(event);
        return;
    }

    /* Check directory created */
    auto response_directory_created = mFSHelper->isDirectoryCreated(root_node);
    if (response_directory_created.first) {
        auto folder = std::make_unique<FSFolder>(response_directory_created.second);
        folder->setRoot(root_node);
        mFileSystemWatcher->addPath(folder->path());
        root_node->addNode(std::move(folder));
        FSEvent event{ FSEvent::EventType::CREATED, response_directory_created.second, true };
        emit fsEvent(event);
        return;
    }

    /* Check directory renamed */
    auto response_directory_renamed = mFSHelper->isDirectoryRenamed(root_node);
    if (response_directory_renamed.first) {
        mFileSystemWatcher->removePath(response_directory_renamed.second.second);
        root_node->removeNonrecursiveNode(response_directory_renamed.second.second);
        auto folder = std::make_unique<FSFolder>(response_directory_renamed.second.first);
        folder->setRoot(root_node);
        mFileSystemWatcher->addPath(folder->path());
        root_node->addNode(std::move(folder));
        FSEvent event{ FSEvent::EventType::RENAMED, response_directory_renamed.second.first, true };
        emit fsEvent(event);
        return;
    }
}

void FSWatcherManager::fileChangedAction(const QString &path)
{
    /* Check file modified */
    auto response_file_modified = mFSHelper->isFileModified(path);
    if (response_file_modified.first) {
        FSEvent event{ FSEvent::EventType::EDITED, response_file_modified.second, false };
        emit fsEvent(event);
        return;
    }

    /* Find Node */
    FSNode* file{ nullptr };
    for (auto& node : mFSNodes) {
        FSNode* temp_node = node->findNode(path);
        if (temp_node) {
            file = temp_node;
            break;
        }
    }

    if (!file) {
        qWarning() << "FileWatcher::onFileChanged: file not found";
        return;
    }

    FSNode* root_node = file->root();
    if (!root_node) {
        qWarning() << "FileWatcher::onFileChanged: file does not have root node";
        return;
    }

    /* Check file removed */
    auto response_file_removed = mFSHelper->isFileRemoved(root_node);
    if (response_file_removed.first) {
        mFileSystemWatcher->removePath(response_file_removed.second);
        root_node->removeNonrecursiveNode(response_file_removed.second);
        FSEvent event{ FSEvent::EventType::REMOVED, response_file_removed.second, false };
        emit fsEvent(event);
        return;
    }

    /* Check file renamed */
    auto response_file_renamed = mFSHelper->isFileRenamed(root_node);
    if (response_file_renamed.first) {
        mFileSystemWatcher->removePath(response_file_renamed.second.second);
        root_node->removeNonrecursiveNode(response_file_renamed.second.second);
        auto file = std::make_unique<FSFile>(response_file_renamed.second.first);
        file->setRoot(root_node);
        mFileSystemWatcher->addPath(file->path());
        root_node->addNode(std::move(file));
        FSEvent event{ FSEvent::EventType::RENAMED, response_file_renamed.second.first, false };
        emit fsEvent(event);
        return;
    }
}

void FSWatcherManager::watchFolder(FSNode *root_folder)
{
    /* Find folders */
    std::vector<QString> folder_list;
    QDirIterator folder_it{ root_folder->path(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories };
    while( folder_it.hasNext() ) {
        QString folder_path = folder_it.next();
        folder_list.push_back(folder_path);
    }

    std::sort(folder_list.begin(), folder_list.end(), [] (const QString& l, const QString& r) {
        return l.count('/') < r.count('/');
    });

    int root_folder_slash_size = root_folder->path().count('/');
    for (const QString& folder_path : folder_list) {
        int folder_slash_size = folder_path.count('/');
        if (folder_slash_size > root_folder_slash_size) {
            QString root_folder_path = mFSHelper->rootOfPath(folder_path);
            FSNode* root_node = root_folder->findNode(root_folder_path);
            if (!root_node) {
                qWarning() << "Root node is not exist";
                continue;
            }
            std::unique_ptr<FSFolder> folder = std::make_unique<FSFolder>(folder_path);
            folder->setRoot(root_node);
            root_node->addNode(std::move(folder));
        }
    }

   /* Find Files */
   for (FSNode* node : root_folder->nodes()) {
       QDirIterator file_it{ node->path(), QDir::Files };
       while (file_it.hasNext()) {
           QString file_path = file_it.next();
           std::unique_ptr<FSFile> file = std::make_unique<FSFile>(file_path);
           file->setRoot(node);
           node->addNode(std::move(file));
       }
   }

   /* Watch folder */
   mFileSystemWatcher->addPaths(root_folder->paths());
}

FSWatcherManager::WatchStatus FSWatcherManager::status() const
{
    return mStatus;
}

void FSWatcherManager::setStatus(WatchStatus newStatus)
{
    mStatus = newStatus;
}

} // namespace fswatcher
