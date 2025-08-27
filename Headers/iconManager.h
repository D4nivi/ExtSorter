#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QIcon>

/* --- Clase simple para manejar los iconos que se muestran en los CustomMessageBox --- */

class IconManager
{
public:
    enum class IconType {
        Warning,
        Info,
        Comprimir,
        Eliminar,
        Mover
    };

    static QIcon getIcon(IconType type) {
        switch (type) {
        case IconType::Warning:
            return QIcon(":/images/warning.png");
        case IconType::Info:
            return QIcon(":/images/information.png");
        case IconType::Comprimir:
            return QIcon(":/images/zip.png");
        case IconType::Eliminar:
            return QIcon(":/images/delete.png");
        case IconType::Mover:
            return QIcon(":/images/transfer.png");
        default:
            return QIcon();
        }
    }
};

#endif // ICONMANAGER_H
