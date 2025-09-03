#include "Headers/customMessageBox.h"

bool CustomMessageBox::mostrarConfirmacion(QWidget * parent, const QString &titulo, const QString &mensaje, const IconManager::IconType icono,
                                           const QString &textoBotonAceptar, const QString &textoBotonCancelar)
{
    bool confirm = true;
    static const QSettings s;

    if (s.value("showWarnings").toBool()) {
        QPushButton * btnAceptar = new QPushButton(textoBotonAceptar);
        QPushButton * btnCancelar = new QPushButton(textoBotonCancelar);

        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(titulo);
        msgBox.setText(mensaje);
        msgBox.setIconPixmap(IconManager::getIcon(icono).pixmap(32, 32));

        msgBox.addButton(btnAceptar, QMessageBox::AcceptRole);
        msgBox.addButton(btnCancelar, QMessageBox::RejectRole);

        if (icono == IconManager::IconType::Warning) {
            QApplication::beep();
        }

        msgBox.exec();
        confirm = msgBox.clickedButton() == btnAceptar;
    }

    return confirm;
}

void CustomMessageBox::info(QWidget * parent, const QString &titulo, const QString &mensaje)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(titulo);
    msgBox.setText(mensaje);
    msgBox.setIconPixmap(IconManager::getIcon(IconManager::IconType::Info).pixmap(32, 32));
    QApplication::beep();
    msgBox.exec();
}

void CustomMessageBox::warning(QWidget * parent, const QString &titulo, const QString &mensaje)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(titulo);
    msgBox.setText(mensaje);
    msgBox.setIconPixmap(IconManager::getIcon(IconManager::IconType::Warning).pixmap(32, 32));
    QApplication::beep();
    msgBox.exec();
}
