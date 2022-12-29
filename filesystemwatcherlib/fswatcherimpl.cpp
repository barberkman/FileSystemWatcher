#include "fswatcherimpl.h"
#include "managers/fswatchermanager.h"
#include "models/fswatchermodel.h"
#include "models/fseventwatchermodel.h"

#include <QUrl>
#include <QDebug>

namespace fswatcher {

const QUrl FSWatcherImpl::catImageURL{QStringLiteral("https://cataas.com/cat/says/hello%20world!")};

FSWatcherImpl::FSWatcherImpl(QObject *parent)
    : QObject{ parent },
      mFSWatcherManager{ new FSWatcherManager{this} },
      mFSWatcherModel{ new FSWatcherModel{mFSWatcherManager, this} },
      mFSEventWatcherModel{ new FSEventWatcherModel{this} }
{
    connect(mFSWatcherManager, &FSWatcherManager::fsEvent,
            mFSEventWatcherModel, &FSEventWatcherModel::onAddFSEvent);

    // Download cat image when remove event occured
    connect(mFSWatcherManager, &FSWatcherManager::fsEvent,
            [this] (const fswatcher::FSEvent& event) {
        if (event.eventType() == fswatcher::FSEvent::EventType::REMOVED) {
            emit downloadImage(catImageURL);
        }
    });
}

FSWatcherModel *FSWatcherImpl::fsWatcherModel()
{
    return mFSWatcherModel;
}

FSEventWatcherModel *FSWatcherImpl::fsEventWatcherModel()
{
    return mFSEventWatcherModel;
}

} // namespace fswatcher
