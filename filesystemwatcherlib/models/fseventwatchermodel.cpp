#include "fseventwatchermodel.h"

namespace fswatcher {

FSEventWatcherModel::FSEventWatcherModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int FSEventWatcherModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(mEvents.size());
}

int FSEventWatcherModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant FSEventWatcherModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    const FSEvent& event = mEvents.at(row);

    switch (role) {
    case Qt::DisplayRole:
        if (col == 0) { return event.strEvent(); }
        else if (col == 1) { return event.path(); }
        else if (col == 2) { return event.isFolder() ? "Yes" : "No"; }
        else if (col == 3) { return event.timestamp().toString("dd/MM/yyyy hh:mm"); }
        break;
    default:
        break;
    }

    return QVariant{};
}

QVariant FSEventWatcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (section) {
    case 0:
        return "Event Type";
        break;
    case 1:
        return "Path";
        break;
    case 2:
        return "Is Folder";
        break;
    case 3:
        return "Timestamp";
        break;
    default:
        break;
    }

    return QVariant{};
}

void FSEventWatcherModel::onAddFSEvent(const FSEvent event)
{
    int row_index = rowCount();
    beginInsertRows(QModelIndex(), row_index, row_index);
    mEvents.push_back(event);
    endInsertRows();
}

} // namespace fswatcher
