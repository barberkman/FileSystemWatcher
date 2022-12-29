#include "fsfile.h"

namespace fswatcher {

FSFile::FSFile(const QString &path)
    : FSNode{ path }
{

}

FSNode::NodeType FSFile::type() const
{
    return NodeType::FILE;
}

} // namespace fswatcher
