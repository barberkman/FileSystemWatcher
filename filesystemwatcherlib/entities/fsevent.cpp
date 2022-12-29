#include "fsevent.h"

namespace fswatcher {

FSEvent::FSEvent()
    : mTimestamp{ QDateTime::currentDateTimeUtc() }
{

}

FSEvent::FSEvent(EventType eventType, QString path, bool isFolder)
    : mEventType{ eventType },
      mPath{ path },
      mIsFolder{ isFolder },
      mTimestamp{ QDateTime::currentDateTimeUtc() }
{

}

FSEvent::EventType FSEvent::eventType() const
{
    return mEventType;
}

const QString &FSEvent::path() const
{
    return mPath;
}

bool FSEvent::isFolder() const
{
    return mIsFolder;
}

const QDateTime &FSEvent::timestamp() const
{
    return mTimestamp;
}

QString FSEvent::strEvent() const
{
    if (mEventType == EventType::CREATED) {
        return "Created";
    } else if (mEventType == EventType::REMOVED) {
        return "Removed";
    } else if (mEventType == EventType::EDITED) {
        return "Edited";
    } else if (mEventType == EventType::RENAMED) {
        return "Renamed";
    }

    return QString{};
}

} // namespace fswatcher
