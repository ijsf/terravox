#ifndef VXH
#define VXH

#include <QByteArray>
class Terrain;

namespace vxh
{
    Terrain *load(QByteArray&, QString& retMessage);
    bool save(QByteArray&, Terrain *);
}

#endif // VXH

