#include "Headers/categoriasDelete.h"
#include "ui_categoriasDelete.h"
#include "Headers/customMessageBox.h"

CategoriasDelete::CategoriasDelete(ExtensionManager * extensionManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::CategoriasDelete), extensionManager(extensionManager)
{
    crearUI();
    conectarSeñales();
}

CategoriasDelete::~CategoriasDelete()
{
    delete ui;
}


/**** MÉTODOS AUXILIARES ****/
void CategoriasDelete::crearUI()
{
    ui->setupUi(this);

    /* --- Configuración inicial de la ventana --- */
    setWindowTitle("Eliminar Categorías");
    ui->msgImportante->setText("<span style='color: red;'><b>Importante</b></span>: las categorías eliminadas <b>no pueden recuperarse</b>.");

    // Añadir items al QComboBox
    QStringList categorias = extensionManager->getCategorias();
    categorias.insert(categorias.begin(), "");
    ui->barraSeleccion->addItems(categorias);

    // Botones
    ui->btnEliminar->setDisabled(true);
    ui->btnEliminarTodas->setEnabled(!extensionManager->getCategorias().isEmpty());

    /* --- Cabecera --- */
    QLabel * textoCabecera = new QLabel("Elimina categorías para crear otras nuevas.");
    textoCabecera->setFixedHeight(50);
    textoCabecera->setStyleSheet(
        "background-color: #FFFFFF; color: black;"
        "font-size: 12px; font-weight: bold;"
        "padding-left: 20px;"
        "border-bottom: 1px solid #c6cac6; border-top: 1px solid #c6cac6;"
    );

    /* Meter texto en la cabecera */
    ui->layoutCabecera->addWidget(textoCabecera, Qt::AlignLeft);
}

void CategoriasDelete::conectarSeñales()
{
    connect(ui->barraSeleccion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CategoriasDelete::enableDelete);
    connect(ui->btnEliminar, &QPushButton::clicked, this, &CategoriasDelete::btnEliminarClicked);
    connect(ui->btnAceptar, &QPushButton::clicked, this, &CategoriasDelete::btnAceptarClicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &CategoriasDelete::btnCancelarClicked);
    connect(ui->btnEliminarTodas, &QPushButton::clicked, this, &CategoriasDelete::btnEliminarTodasClicked);
}


/**** SLOTS (UI) ****/
void CategoriasDelete::enableDelete()
{
    ui->btnEliminar->setEnabled(ui->barraSeleccion->currentIndex() > 0);
}

/**** BOTONES ****/
void CategoriasDelete::btnEliminarClicked()
{
    int index = ui->barraSeleccion->currentIndex();
    QString categoria = ui->barraSeleccion->itemText(index);
    QString msg = QString("¿Estás seguro de que deseas eliminar la categoría %1?").arg(categoria);

    if (!CustomMessageBox::mostrarConfirmacion(this, "Confirmar Eliminar", msg)) {
        qDebug() << "Eliminación de categoría abortada.";
        return;
    }

    qDebug() << "Categoria a Eliminar:" << categoria;
    qDebug() << "Extensiones:" << extensionManager->getCategoriasYExtensiones()->value(categoria);

    /* Actualizar atributos y ficheros */
    extensionManager->getCategoriasYExtensiones()->remove(categoria);
    extensionManager->escribirExtensionesJSON();
    extensionManager->escribirTXT(true);

    /* Actualizar UI */
    ui->barraSeleccion->removeItem(index);
    ui->btnEliminarTodas->setEnabled(ui->barraSeleccion->currentIndex());
}

void CategoriasDelete::btnAceptarClicked()
{
    close();
}

void CategoriasDelete::btnCancelarClicked()
{
    close();
}

void CategoriasDelete::btnEliminarTodasClicked()
{
    QString msg = QString("¿Estás seguro de que deseas eliminar todas las categorías?");
    if (!CustomMessageBox::mostrarConfirmacion(this, "Confirmar Eliminar Todas", msg)) {
        qDebug() << "Eliminación de todas las categorías abortada.";
    } else {
        /* Actualizar UI */
        for (int i = 0; i < extensionManager->getCategorias().count(); i++) {
            ui->barraSeleccion->removeItem(1);
        }

        /* Actualizar atributos y ficheros */
        extensionManager->getCategoriasYExtensiones()->clear();
        extensionManager->escribirExtensionesJSON();
        extensionManager->escribirTXT(true);
    }
}
