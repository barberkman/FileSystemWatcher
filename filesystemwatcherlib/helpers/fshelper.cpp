#include <QDir>
#include <QDirIterator>
#include <QSet>
#include <QFile>

#include "fshelper.h"

namespace fswatcher {

FSHelper::FSHelper()
{

}

QString FSHelper::rootOfPath(const QString &path) const
{
    return path.mid(0, path.lastIndexOf('/'));
}

std::pair<bool, QString> FSHelper::isDirectoryRemoved(const QString &path)
{
    QDir dir{ path };

    /* If directory not exist, it means it is deleted */
    if (!dir.exists()) {
        return std::make_pair<bool, QString>(true, dir.path());
    }
    return std::make_pair<bool, QString>(false, QString{});
}

std::pair<bool, QString> FSHelper::isFileCreated(const QString &path, FSNode *node)
{
    QStringList stored_files = node->filterNonrecursiveNodes(FSNode::NodeType::FILE);
    QStringList current_files;
    QDirIterator file_it{ path, QDir::Files };
    while (file_it.hasNext()) {
        QString file_path = file_it.next();
        current_files << file_path;
    }

    if (stored_files.size() == current_files.size()) {
        return std::make_pair<bool, QString>(false, QString{});
    }

    QStringList subtracted_list = current_files.toSet().subtract(stored_files.toSet()).toList();

    if (subtracted_list.size() >= 1) {
        return std::make_pair<bool, QString>(true, QString{subtracted_list.first()});
    }

    return std::make_pair<bool, QString>(false, QString{});
}

std::pair<bool, QString> FSHelper::isDirectoryCreated(FSNode *node)
{
    QStringList stored_dirs = node->filterNonrecursiveNodes(FSNode::NodeType::FOLDER);
    QStringList current_dirs;
    QDirIterator dir_it{ node->path(), QDir::Dirs | QDir::NoDotAndDotDot };
    while (dir_it.hasNext()) {
        QString dir_path = dir_it.next();
        current_dirs << dir_path;
    }

    if (stored_dirs.size() == current_dirs.size()) {
        return std::make_pair<bool, QString>(false, QString{});
    }

    QStringList subtracted_list = current_dirs.toSet().subtract(stored_dirs.toSet()).toList();

    if (subtracted_list.size() >= 1) {
        return std::make_pair<bool, QString>(true, QString{subtracted_list.first()});
    }

    return std::make_pair<bool, QString>(false, QString{});
}

std::pair<bool, std::pair<QString, QString> > FSHelper::isDirectoryRenamed(FSNode *node)
{
    QStringList stored_dirs = node->filterNonrecursiveNodes(FSNode::NodeType::FOLDER);
    QStringList current_dirs;
    QDirIterator dir_it{ node->path(), QDir::Dirs | QDir::NoDotAndDotDot };
    while (dir_it.hasNext()) {
        QString dir_path = dir_it.next();
        current_dirs << dir_path;
    }

    if (stored_dirs.size() != current_dirs.size()) {
        return std::make_pair<bool, std::pair<QString, QString> >(false, std::make_pair<QString, QString>(QString{}, QString{}));
    }

    QStringList subtracted_list_new_name = current_dirs.toSet().subtract(stored_dirs.toSet()).toList();
    QStringList subtracted_list_old_name = stored_dirs.toSet().subtract(current_dirs.toSet()).toList();

    if (subtracted_list_new_name.size() >= 1 && subtracted_list_old_name.size() >= 1) {
        return std::make_pair<bool, std::pair<QString, QString> >(true, std::make_pair<QString, QString>(QString{subtracted_list_new_name.first()},
                                                                                                         QString{subtracted_list_old_name.first()}));
    }

    return std::make_pair<bool, std::pair<QString, QString> >(false, std::make_pair<QString, QString>(QString{}, QString{}));
}

std::pair<bool, QString> FSHelper::isFileModified(const QString &path)
{
    if (QFile::exists(path)) {
        return std::make_pair<bool, QString>(true, QString{path});
    }
    return std::make_pair<bool, QString>(false, QString{});
}

std::pair<bool, QString> FSHelper::isFileRemoved(FSNode *node)
{
    QStringList stored_files = node->filterNonrecursiveNodes(FSNode::NodeType::FILE);
    QStringList current_files;
    QDirIterator file_it{ node->path(), QDir::Files };
    while (file_it.hasNext()) {
        QString file_path = file_it.next();
        current_files << file_path;
    }

    if (stored_files.size() == current_files.size()) {
        return std::make_pair<bool, QString>(false, QString{});
    }

    QStringList subtracted_list = stored_files.toSet().subtract(current_files.toSet()).toList();

    if (subtracted_list.size() >= 1) {
        return std::make_pair<bool, QString>(true, QString{subtracted_list.first()});
    }

    return std::make_pair<bool, QString>(false, QString{});
}

std::pair<bool, std::pair<QString, QString> > FSHelper::isFileRenamed(FSNode *node)
{
    QStringList stored_files = node->filterNonrecursiveNodes(FSNode::NodeType::FILE);
    QStringList current_files;
    QDirIterator file_it{ node->path(), QDir::Files };
    while (file_it.hasNext()) {
        QString file_path = file_it.next();
        current_files << file_path;
    }

    if (stored_files.size() != current_files.size()) {
        return std::make_pair<bool, std::pair<QString, QString> >(false, std::make_pair<QString, QString>(QString{}, QString{}));
    }

    QStringList subtracted_list_new_name = current_files.toSet().subtract(stored_files.toSet()).toList();
    QStringList subtracted_list_old_name = stored_files.toSet().subtract(current_files.toSet()).toList();

    if (subtracted_list_new_name.size() >= 1 && subtracted_list_old_name.size() >= 1) {
        return std::make_pair<bool, std::pair<QString, QString> >(true, std::make_pair<QString, QString>(QString{subtracted_list_new_name.first()},
                                                                                                         QString{subtracted_list_old_name.first()}));
    }

    return std::make_pair<bool, std::pair<QString, QString> >(false, std::make_pair<QString, QString>(QString{}, QString{}));
}

} // namespace fswatcher
