#ifndef FSHELPER_H
#define FSHELPER_H

#include <utility>

#include <QString>

#include "components/fsnode.h"

namespace fswatcher {

class FSHelper
{
public:
    FSHelper();

    QString rootOfPath(const QString& path) const;

    std::pair<bool, QString> isDirectoryRemoved(const QString& path);
    std::pair<bool, QString> isFileCreated(const QString& path, FSNode* node);
    std::pair<bool, QString> isDirectoryCreated(FSNode* node);

    /*!
     * \brief isDirectoryRenamed
     * \param path
     * \param node
     * \return
     * first: success or fail
     * second: returned names
     *         first: new name
     *         second: old name
     */
    std::pair<bool, std::pair<QString, QString>> isDirectoryRenamed(FSNode* node);

    std::pair<bool, QString> isFileModified(const QString& path);
    std::pair<bool, QString> isFileRemoved(FSNode* node);

    /*!
     * \brief isFileRenamed
     * \param node
     * \return
     * first: success or fail
     * second: returned names
     *         first: new name
     *         second: old name
     */
    std::pair<bool, std::pair<QString, QString>> isFileRenamed(FSNode* node);
};

} // namespace fswatcher

#endif // FSHELPER_H
