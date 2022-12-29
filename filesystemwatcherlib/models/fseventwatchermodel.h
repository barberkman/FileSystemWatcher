#ifndef FSWATCHER_FSEVENTWATCHERMODEL_H
#define FSWATCHER_FSEVENTWATCHERMODEL_H

#include <vector>

#include <QAbstractTableModel>

#include "entities/fsevent.h"

namespace fswatcher {

class FSEventWatcherModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FSEventWatcherModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;

public slots:
    void onAddFSEvent(const fswatcher::FSEvent event);

private:
    std::vector<FSEvent> mEvents;
};

} // namespace fswatcher

#endif // FSWATCHER_FSEVENTWATCHERMODEL_H
