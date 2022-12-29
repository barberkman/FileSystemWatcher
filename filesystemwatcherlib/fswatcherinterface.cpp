#include "fswatcherinterface.h"
#include "fswatcherimpl.h"

#include "entities/fsevent.h"

namespace fswatcher {

FSWatcherInterface::FSWatcherInterface(QObject *parent)
    : QObject{ parent },
      mImpl{ new FSWatcherImpl{this} }
{
    qRegisterMetaType<fswatcher::FSEvent>("fswatcher::FSEvent");

    connect(mImpl, &FSWatcherImpl::downloadImage,
            this, &FSWatcherInterface::downloadImage);
}

FSWatcherModel *FSWatcherInterface::fsWatcherModel()
{
    return mImpl->fsWatcherModel();
}

FSEventWatcherModel *FSWatcherInterface::fsEventWatcherModel()
{
    return mImpl->fsEventWatcherModel();
}

} // namespace fswatcher
