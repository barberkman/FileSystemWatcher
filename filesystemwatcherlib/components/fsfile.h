#ifndef FSFILE_H
#define FSFILE_H

#include "fsnode.h"

namespace fswatcher {

class FSFile : public FSNode
{
public:
    FSFile(const QString& path);

    NodeType type() const override;
};

} // namespace fswatcher

#endif // FSFILE_H
