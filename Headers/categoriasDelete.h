#ifndef CATEGORIASDELETE_H
#define CATEGORIASDELETE_H

#include "extensionManager.h"

namespace Ui {
class CategoriasDelete;
}

class CategoriasDelete : public QDialog
{
    Q_OBJECT

public:
    explicit CategoriasDelete(ExtensionManager * extensionManager, QWidget *parent = nullptr);
    ~CategoriasDelete();

private:
    Ui::CategoriasDelete *ui;
    ExtensionManager * extensionManager;

private slots:
    void enableDelete();

    void btnEliminarClicked();

    void btnAceptarClicked();

    void btnCancelarClicked();

    void btnEliminarTodasClicked();

private:
    /* --- Métodos Auxiliares --- */
    void crearUI();
    void conectarSeñales();
};

#endif // CATEGORIASDELETE_H
