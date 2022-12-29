#ifndef FSEVENT_H
#define FSEVENT_H

#include <QString>
#include <QDateTime>

namespace fswatcher {

class FSEvent
{
public:
    enum class EventType
    {
        CREATED,
        REMOVED,
        EDITED,
        RENAMED
    };

    FSEvent();
    FSEvent(EventType eventType, QString path, bool isFolder);

    EventType eventType() const;
    const QString &path() const;
    bool isFolder() const;
    const QDateTime &timestamp() const;

    QString strEvent() const;

private:
    EventType mEventType;
    QString mPath;
    bool mIsFolder{ false };
    QDateTime mTimestamp;
};

} // namespace fswatcher

#endif // FSEVENT_H
