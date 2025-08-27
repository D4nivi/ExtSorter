#ifndef CATEGORIASADD_H
#define CATEGORIASADD_H

#include "extensionManager.h"

namespace Ui {
class CategoriasAdd;
}

class CategoriasAdd : public QDialog
{
    Q_OBJECT

public:
    explicit CategoriasAdd(ExtensionManager * extensionManager, QWidget *parent = nullptr);
    ~CategoriasAdd();

private:
    Ui::CategoriasAdd *ui;

    /* --- Estado Interno --- */
    QString nuevaCategoria;
    ExtensionManager * extensionManager;

private:
    void enableSave();

    void btnGuardarClicked();

    void btnAceptarClicked();

    void btnCancelarClicked();

    void btnRestablecerClicked();

    void checkExtensionBoxCharLimit();

signals:
    void categoryAdded(QString &nuevaCategoria, QSet<QString> &nuevasExtensiones);

private:
    /*--- Métodos Auxiliares ---*/
    void crearUI();
    void conectarSeñales();
};

#endif // CATEGORIASADD_H
