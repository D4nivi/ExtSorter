#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

/* Clase auxiliar para iconos mostrados en CustomMessageBox */
class IconManager
{
public:
    enum class IconType { Warning, Info, Comprimir, Eliminar, Mover, NoIcon };

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
        case IconType::NoIcon:
        default:
            return QIcon();
        }
    }

    static IconType fromString(const QString &iconName) {
        QString normalizedIconName = iconName.toLower();
        IconType icon;

        if (normalizedIconName == "warning") icon = IconType::Warning;
        else if (normalizedIconName == "information") icon = IconType::Info;
        else if (normalizedIconName == "comprimir") icon = IconType::Comprimir;
        else if (normalizedIconName == "eliminar") icon = IconType::Eliminar;
        else if (normalizedIconName == "mover") icon = IconType::Mover;
        else icon = IconType::NoIcon;

        return icon;
    }
};

class CustomMessageBox
{
public:

    /* Crea un equivalente a un QMessageBox de información.
     * La función existe porque las opciones del QMessageBox están en inglés y no se pueden cambiar
    */
    static bool mostrarConfirmacion(QWidget * parent,
                                    const QString &titulo,
                                    const QString &mensaje,
                                    const IconManager::IconType icono = IconManager::IconType::Warning,
                                    const QString &textoBotonAceptar = "Sí",
                                    const QString &textoBotonCancelar = "No"
    );

    static void info(QWidget * parent, const QString &titulo, const QString &mensaje);
    static void warning(QWidget * parent, const QString &titulo, const QString &mensaje);
    /* Nota: no hago uno para critical porque quiero que suene el beep característico */
};

#endif // CUSTOMMESSAGEBOX_H
