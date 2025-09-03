#ifndef EXTENSIONES_H
#define EXTENSIONES_H

#include "extensionManager.h"
#include <QMessageBox>

namespace Ui {
class Extensiones;
}

class Extensiones : public QDialog
{
    Q_OBJECT

public:
    explicit Extensiones(ExtensionManager * extensionManager, QWidget *parent = nullptr);
    ~Extensiones();

private:
    Ui::Extensiones *ui;

    /* --- Estado interno --- */
    QString textoCaja;
    QString categoriaExtension;
    ExtensionManager * extensionManager;

private slots:
    void enableSave();

    void btnGuardarClicked();

    void btnAceptarClicked();

    void btnCancelarClicked();

    void btnRestablecerClicked();

    void reloadComboBoxItems();

    void checkExtensionBoxCharLimit();

signals:
    void extensionsReseted();

private:
    /*--- Métodos Auxiliares ---*/
    void crearUI();
    void conectarSeñales();
};

#endif // EXTENSIONES_H
