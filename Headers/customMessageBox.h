#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include "iconManager.h"

#include <QApplication>
#include <QMessageBox>
#include <QPushButton>

class CustomMessageBox
{
public:

    /* Crea un equivalente a un QMessageBox de información.
     * La función existe porque las opciones del QMessageBox están en inglés y no se pueden cambiar
    */
    static bool mostrarConfirmacion(QWidget * parent, const QString &titulo, const QString &mensaje,const IconManager::IconType icono,
                                    const QString &textoBotonAceptar = "Sí", const QString &textoBotonCancelar = "No");

    static void info(QWidget * parent, const QString &titulo, const QString &mensaje);
    static void warning(QWidget * parent, const QString &titulo, const QString &mensaje);
    /* Nota: no hago uno para critical porque quiero que suene el beep característico */
};

#endif // CUSTOMMESSAGEBOX_H
