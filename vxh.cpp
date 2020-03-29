#include "vxh.h"
#include "terrain.h"
#include <QScopedPointer>
#include <QString>
#include <QApplication>

namespace vxh
{
    static const quint32 VERSION = 1;

    Terrain *load(QByteArray &bytes, QString &retMessage)
    {
        QScopedPointer<Terrain> terrain(new Terrain(QSize(512, 512)));

        retMessage = "";

        const auto *v = reinterpret_cast<const quint8 *>(bytes.data());
        quint32 offset = 0;
        const quint32 length = bytes.size();

        if (length < (4 * 4)) {
            retMessage = "Invalid VXH file.";
            return nullptr;
        }
        QString header = QString::fromUtf8(reinterpret_cast<const char*>(v), 4);
        if (header != "VXH ") {
            retMessage = "Invalid VXH file.";
            return nullptr;
        }
        offset += 4;
        quint32 version = *reinterpret_cast<const quint32*>(v + offset);
        if (version != VERSION) {
            retMessage = "Invalid VXH file.";
            return nullptr;
        }
        offset += 4;
        quint32 width = *reinterpret_cast<const quint32*>(v + offset);
        offset += 4;
        quint32 height = *reinterpret_cast<const quint32*>(v + offset);
        offset += 4;

        for (unsigned int y = 0; y < width; ++y) {
            for (unsigned int x = 0; x < height; ++x) {
                if (offset >= length) {
                    retMessage = "Invalid VXH file.";
                    return nullptr;
                }
                float surfZ = *reinterpret_cast<const float*>(v + offset);
                offset += 4;
                quint32 surfColor = *reinterpret_cast<const quint32*>(v + offset);
                offset += 4;

                terrain->landform(x, y) = surfZ;
                terrain->color(x, y) = surfColor;
            }
        }

        return terrain.take();
    }

    bool save(QByteArray &bytes, Terrain *t)
    {
        Q_ASSERT(t);
        auto size = t->size();
        const quint32 width = size.width();
        const quint32 height = size.height();

        const quint32 version = VERSION;
        bytes.append("VXH ", 4);
        bytes.append(reinterpret_cast<const char *>(&version), 4);

        bytes.append(reinterpret_cast<const char *>(&width), 4);
        bytes.append(reinterpret_cast<const char *>(&height), 4);

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                const float surfZ = t->landform(x, y);
                quint32 color = t->color(x, y);
                bytes.append(reinterpret_cast<const char *>(&surfZ), 4);
                bytes.append(reinterpret_cast<const char *>(&color), 4);
            }
        }
        return true;
    }

}
