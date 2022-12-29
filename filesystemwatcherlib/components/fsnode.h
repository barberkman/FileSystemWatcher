#ifndef FSNODE_H
#define FSNODE_H

#include <vector>
#include <memory>

#include <QString>
#include <QStringList>

namespace fswatcher {

class FSNode
{
public:
    enum class NodeType {
        FILE,
        FOLDER
    };

    virtual NodeType type() const = 0;

    QString path() const {
        return mPath;
    }

    FSNode* root() {
        return mRoot;
    }

    void setRoot(FSNode* root) {
        mRoot = root;
    }

    virtual QStringList paths() const {
        return QStringList{ mPath };
    }

    virtual std::vector<FSNode*> nodes() {
        return std::vector<FSNode*>{ this };
    }

    virtual void addNode(std::unique_ptr<FSNode> node) {}

    virtual void removeNonrecursiveNode(const QString& path) {}

    virtual FSNode* findNode(const QString& path) {
        return nullptr;
    }

    virtual QStringList filterNonrecursiveNodes(NodeType type) const {
        return QStringList{};
    }

protected:
    FSNode(const QString& path)
        : mPath{ path }
    {}

private:
    QString mPath;
    FSNode* mRoot{ nullptr };
};

} // namespace fswatcher

#endif // FSNODE_H
