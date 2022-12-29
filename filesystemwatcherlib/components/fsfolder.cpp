#include "fsfolder.h"

namespace fswatcher {

FSFolder::FSFolder(const QString &path)
    : FSNode{ path }
{

}

FSNode::NodeType FSFolder::type() const
{
    return NodeType::FOLDER;
}

QStringList FSFolder::paths() const
{
    QStringList path_list{ path() };
    for (const auto& node : mNodes) {
        path_list << node->paths();
    }
    return path_list;
}

std::vector<FSNode *> FSFolder::nodes()
{
    std::vector<FSNode*> node_list{ this };
    for (auto& node : mNodes) {
        for (FSNode* temp_node : node->nodes()) {
            node_list.push_back(temp_node);
        }
    }
    return node_list;
}

void FSFolder::addNode(std::unique_ptr<FSNode> node)
{
    mNodes.push_back(std::move(node));
}

void FSFolder::removeNonrecursiveNode(const QString &path)
{
    for (auto it = mNodes.begin(); it != mNodes.end(); ++it) {
        if (it->get()->path() == path) {
            mNodes.erase(it);
            break;
        }
    }
}

FSNode *FSFolder::findNode(const QString &path)
{
    // Optimization for root folder
    if (path == this->path()) {
        return this;
    }

    std::vector<FSNode*> temp_node = nodes();
    auto it = std::find_if(temp_node.begin(), temp_node.end(), [&path] (const FSNode* node) {
        return path == node->path();
    });
    if (it != temp_node.end()) { return *it; }

    return nullptr;
}

QStringList FSFolder::filterNonrecursiveNodes(NodeType type) const
{
    QStringList path_list;
    for (const auto& node : mNodes) {
        if (node->type() == type) {
             path_list << node->path();
        }
    }
    return path_list;
}

} // namespace fswatcher
