#include "fswatchermodel.h"
#include "managers/fswatchermanager.h"

#include <QDebug>

namespace fswatcher {

FSWatcherModel::FSWatcherModel(FSWatcherManager *fswatchermanager,
                                   QObject *parent)
    : QAbstractListModel{ parent },
      mFSWatcherManager{ fswatchermanager }
{
    mRoles[Roles::PathRole] = "path";
}

int FSWatcherModel::rowCount(const QModelIndex &parent) const
{
    return mFSWatcherManager->watchedFolderCount();
}

QVariant FSWatcherModel::data(const QModelIndex &index, int role) const
{
    if (!isIndexValid(index))
        return QVariant{};

    switch (role) {
    case Roles::PathRole:
        return mFSWatcherManager->folderPath(index.row());
        break;
    case Qt::DisplayRole:
        return mFSWatcherManager->folderPath(index.row());
    default:
        break;
    }

    return QVariant{};
}

QHash<int, QByteArray> FSWatcherModel::roleNames() const
{
    return mRoles;
}

void FSWatcherModel::addFolder(const QUrl &path)
{
    int row_index = rowCount();
    beginInsertRows(QModelIndex(), row_index, row_index);
    try {
        mFSWatcherManager->addFolder(path);
    } catch (const std::runtime_error& ex) {
        qWarning() << "FSWatcherModel::watchFolder: exception" << ex.what();
        return;
    }

    endInsertRows();
}

void FSWatcherModel::removeFolder(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    try {
        mFSWatcherManager->removeFolder(index);
    } catch (const std::runtime_error& ex) {
        qWarning() << "FSWatcherModel::removeFolder: exception: " << ex.what();
        return;
    }
    endRemoveRows();

    emit statusChanged();
}

void FSWatcherModel::clearFolders()
{
    beginResetModel();
    try {
        mFSWatcherManager->clearFolders();
    } catch (const std::runtime_error& ex) {
        qWarning() << "FSWatcherModel::clearFolders: exception: " << ex.what();
        return;
    }
    endResetModel();

    emit statusChanged();
}

void FSWatcherModel::startWatching()
{
    try {
        mFSWatcherManager->startWatching();
    } catch (const std::runtime_error& ex) {
        qWarning() << "FSWatcherModel::startWatching: exception: " << ex.what();
        return;
    }

    emit statusChanged();
}

void FSWatcherModel::stopWatching()
{
    try {
        mFSWatcherManager->stopWatching();
    } catch (const std::runtime_error& ex) {
        qWarning() << "FSWatcherModel::stopWatching: exception: " << ex.what();
        return;
    }

    emit statusChanged();
}

int FSWatcherModel::status() const
{
    return static_cast<int>(mFSWatcherManager->status());
}

bool FSWatcherModel::isIndexValid(const QModelIndex &index) const
{
    if (index.row() < 0
            || index.row() >= rowCount()
            || !index.isValid()) {
        return false;
    }
    return true;
}

} // namespace fswatcher
