#include "Headers/extensiones.h"
#include "ui_extensiones.h"
#include "Headers/extensionManager.h"

Extensiones::Extensiones(ExtensionManager * extensionManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::Extensiones), extensionManager(extensionManager)
{
    ui->setupUi(this);
    crearUI();
    conectarSeñales();
}

Extensiones::~Extensiones()
{
    delete ui;
}

/**** MÉTODOS AUXILIARES ****/
void Extensiones::crearUI()
{
    /* --- Configuración inicial de la ventana --- */
    setWindowTitle("Configuración de extensiones");

    // Añadir items a la barra de selección
    QStringList categorias = extensionManager->getCategorias();
    categorias.insert(categorias.begin(), "");
    ui->barraSeleccion->addItems(categorias);

    // Caja de texto de las extensiones
    ui->cajaEditExt->setPlaceholderText("Escribe las extensiones separadas por comas (,).\nPor ejemplo: png,jpg,tiff...");
    ui->cajaEditExt->setDisabled(true);
    ui->mensajeError->setText("");

    // Botones
    ui->btnGuardar->setDisabled(true);
    ui->btnRestablecer->setEnabled(extensionManager->wereFilesModified());

    /* --- Cabecera --- */
    QLabel * textoCabecera = new QLabel("Añade más extensiones a las diferentes categorías disponibles.");
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

void Extensiones::conectarSeñales()
{
    connect(ui->barraSeleccion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Extensiones::enableSave);
    connect(ui->cajaEditExt, &QPlainTextEdit::textChanged, this, &Extensiones::enableSave);
    connect(ui->cajaEditExt, &QPlainTextEdit::textChanged, this, &Extensiones::checkExtensionBoxCharLimit);
    connect(ui->btnGuardar, &QPushButton::clicked, this, &Extensiones::btnGuardarClicked);
    connect(ui->btnAceptar, &QPushButton::clicked, this, &Extensiones::btnAceptarClicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &Extensiones::btnCancelarClicked);
    connect(ui->btnRestablecer, &QPushButton::clicked, this, &Extensiones::btnRestablecerClicked);
    connect(this, &Extensiones::extensionsReseted, this, &Extensiones::reloadComboBoxItems);
}


/**** SLOTS (UI) ****/
void Extensiones::enableSave()
{
    int index = ui->barraSeleccion->currentIndex();
    textoCaja = ui->cajaEditExt->toPlainText();
    categoriaExtension = ui->barraSeleccion->itemText(index);
    bool indexValido = index > 0;

    // Activar/desactivar el QTextEdit
    ui->cajaEditExt->setEnabled(indexValido);

    // Activar el botón
    bool activar = indexValido && extensionManager->checkExtensionListFormat(textoCaja, ui->mensajeError);
    if (activar) ui->mensajeError->setText("");
    ui->btnGuardar->setEnabled(activar);
}

/**** BOTONES ****/
void Extensiones::btnGuardarClicked()
{
    qDebug() << "Extensiones a añadir: {" + textoCaja + "}";
    qDebug() << "Categoria: {" + categoriaExtension + "}";

    QString extensionesCaja = textoCaja.toLower();
    QStringList exts = textoCaja.split(",");
    auto categoriasYExtensiones = extensionManager->getCategoriasYExtensiones();

    /* Actualizamos el QMap de ExtensionManager */
    QSet<QString> newSet = categoriasYExtensiones->value(categoriaExtension);
    for (int i = 0; i < exts.size(); ++i) {
        newSet.insert(exts[i]);
    }

    categoriasYExtensiones->insert(categoriaExtension, newSet);

    /* Actualizamos el fichero de extensiones */
    extensionManager->addExtensionesTXT(extensionesCaja);

    /* Escribimos el archivo json */
    if (extensionManager->escribirExtensionesJSON()) {
        ui->btnGuardar->setEnabled(false);
        ui->btnRestablecer->setEnabled(true);
    } else {
        QMessageBox::critical(this, "Error", "No se ha podido crear el archivo de extensiones.");
    }

}

void Extensiones::btnAceptarClicked()
{
    close();
}

void Extensiones::btnCancelarClicked()
{
    close();
}

void Extensiones::btnRestablecerClicked()
{
    if (extensionManager->restoreCategoriasYExtensiones()) {
        /* Ajustes a la UI */
        ui->btnRestablecer->setEnabled(false);
        emit extensionsReseted();
    }
}


/**** MÉTODOS AUXILIARES ****/
void Extensiones::reloadComboBoxItems()
{
    QStringList categorias = extensionManager->getCategorias();

    for (int i = 0; i < categorias.count(); i++) {
        ui->barraSeleccion->removeItem(1);
    }

    ui->barraSeleccion->addItems(categorias);
}

void Extensiones::checkExtensionBoxCharLimit()
{
    QString textoCaja = ui->cajaEditExt->toPlainText();
    if (ui->cajaEditExt->toPlainText().size() > ExtensionManager::maxExtensionBoxChars) {
        textoCaja.truncate(ExtensionManager::maxExtensionBoxChars);
        ui->cajaEditExt->setPlainText(textoCaja);

        // Para mover el cursor al final (bruh)
        QTextCursor cursor = ui->cajaEditExt->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->cajaEditExt->setTextCursor(cursor);

        ui->mensajeError->setText("<span style='color: red;'><b>Límite de caracteres superado.</b></span>");
    }
}
