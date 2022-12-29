#ifndef FSFOLDER_H
#define FSFOLDER_H

#include "fsnode.h"

namespace fswatcher {

class FSFolder : public FSNode
{
public:
    FSFolder(const QString& path);

    // FSNode interface
    NodeType type() const override;

    QStringList paths() const override;

    std::vector<FSNode *> nodes() override;

    void addNode(std::unique_ptr<FSNode> node) override;

    void removeNonrecursiveNode(const QString& path) override;

    FSNode* findNode(const QString& path) override;

    QStringList filterNonrecursiveNodes(NodeType type) const override;

private:
    std::vector<std::unique_ptr<FSNode>> mNodes;
};

} // namespace fswatcher

#endif // FSFOLDER_H
