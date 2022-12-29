#ifndef FSWATCHER_FILEWATCHERMODEL_H
#define FSWATCHER_FILEWATCHERMODEL_H

#include <memory>

#include <QAbstractListModel>

namespace fswatcher {

class FSWatcherManager;

class FSWatcherModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
public:
    enum Roles {
        PathRole = Qt::UserRole + 1,
    };

    FSWatcherModel(FSWatcherManager* fswatchermanager,
                     QObject* parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    /* From QML */
    Q_INVOKABLE void addFolder(const QUrl& path);
    Q_INVOKABLE void removeFolder(int index);
    Q_INVOKABLE void clearFolders();
    Q_INVOKABLE void startWatching();
    Q_INVOKABLE void stopWatching();

    int status() const;

signals:
    void statusChanged();

private:
    bool isIndexValid(const QModelIndex& index) const;

private:
    QHash<int, QByteArray> mRoles;
    FSWatcherManager* mFSWatcherManager{ nullptr };
};

} // namespace fswatcher

#endif // FSWATCHER_FILEWATCHERMODEL_H
