#include "Headers/categoriasAdd.h"
#include "ui_categoriasAdd.h"

CategoriasAdd::CategoriasAdd(ExtensionManager * extensionManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::CategoriasAdd), extensionManager(extensionManager)
{
    crearUI();
    conectarSeñales();
}

CategoriasAdd::~CategoriasAdd()
{
    delete ui;
}


/**** MÉTODOS AUXILIARES ****/
void CategoriasAdd::crearUI()
{
    ui->setupUi(this);

    /* --- Configuración inicial de la ventana --- */
    setWindowTitle("Añadir Categorías");

    // Botones
    ui->btnGuardar->setDisabled(true);
    ui->btnRestablecer->setEnabled(extensionManager->wereFilesModified());

    // Cajas de texto
    ui->mensajeError->setText("");
    ui->cajaCategoria->setPlaceholderText("Puedes usar emojis. Por ejemplo: 🌐 Web, ✨ Adobe, ...");
    ui->cajaCategoria->setMaxLength(ExtensionManager::maxCategoryChars);
    ui->cajaExtensiones->setPlaceholderText("Escribe las extensiones separadas por comas (,).\nPor ejemplo: png,jpg,tiff...");

    /* --- Cabecera --- */
    QLabel * textoCabecera = new QLabel("Añade tus propias categorías al programa.");
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

void CategoriasAdd::conectarSeñales()
{
    connect(ui->btnGuardar, &QPushButton::clicked, this, &CategoriasAdd::btnGuardarClicked);
    connect(ui->btnAceptar, &QPushButton::clicked, this, &CategoriasAdd::btnAceptarClicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &CategoriasAdd::btnCancelarClicked);
    connect(ui->btnRestablecer, &QPushButton::clicked, this, &CategoriasAdd::btnRestablecerClicked);
    connect(ui->cajaCategoria, &QLineEdit::textChanged, this, &CategoriasAdd::enableSave);
    connect(ui->cajaExtensiones, &QPlainTextEdit::textChanged, this, &CategoriasAdd::enableSave);
    connect(ui->cajaExtensiones, &QPlainTextEdit::textChanged, this, &CategoriasAdd::checkExtensionBoxCharLimit);
}


/**** SLOTS (UI) ****/
void CategoriasAdd::enableSave()
{
    QString textoCaja = ui->cajaExtensiones->toPlainText();
    nuevaCategoria = ui->cajaCategoria->text();
    bool catRep = extensionManager->getCategorias().contains(nuevaCategoria, Qt::CaseInsensitive);
    bool nombreValido = ExtensionManager::isFolderNameValid(nuevaCategoria);

    if (catRep) {
        ui->mensajeError->setText(QString("<span style='color: red;'><b>Categoría ya existente</b></span>: la categoría <b>\"%1\"</b> ya existe.").arg(nuevaCategoria.toLower()));
    }
    else if (!nombreValido) {
        ui->mensajeError->setText(QString("<span style='color: red;'><b>Nombre de categoría inválido.</b></span>"));
    }

    /* Nota: esta comprobación se hacía con un else if dentro de checkExtensionListFormat para evitar que apareciera texto
     * cuando se borraba de golpe el cuadro de categoria. Se deja aquí porque nunca se comprobaría al asignar "activar",
     * ya que si esto pasa categoriaCorrecta siempre es false, y nunca se llega a entrar a checkExtensionListFormat. */
    if (textoCaja.isEmpty() && !catRep) ui->mensajeError->setText("");

    // Activar el botón
    bool activar = !catRep && nombreValido && extensionManager->checkExtensionListFormat(textoCaja, ui->mensajeError);
    if (activar) ui->mensajeError->setText("");
    ui->btnGuardar->setEnabled(activar);
}

/**** BOTONES ****/
void CategoriasAdd::btnGuardarClicked()
{
    /* --- Actualizar Atributos (internos y de clase ExtensionManager) --- */
    QStringList categorias = extensionManager->getCategorias();
    QString extensionesCaja = ui->cajaExtensiones->toPlainText().toLower();
    QStringList listaExtensiones = extensionesCaja.split(",");

    QSet<QString> nuevasExtensiones;
    QStringList nuevasExtensionesList;
    categorias.append(nuevaCategoria);

    qDebug() << "Nueva Categoría:" << nuevaCategoria << Qt::endl << "Nuevas Extensiones:" << extensionesCaja;

    for (const QString &ext : std::as_const(listaExtensiones)) {
        nuevasExtensiones.insert(ext);
        nuevasExtensionesList.append(ext);
    }

    extensionManager->getCategoriasYExtensiones()->insert(nuevaCategoria, nuevasExtensiones);

    /* --- Actualizar ficheros --- */

    /* Fichero JSON */
    if (extensionManager->addCategoriasJSON(nuevaCategoria, nuevasExtensionesList)) {
        ui->btnGuardar->setEnabled(false);
        ui->btnRestablecer->setEnabled(true);
    } else {
        QMessageBox::critical(this, "Error", "No se ha podido crear el archivo de extensiones.");
    }

    /* Fichero .txt de extensiones */
    extensionManager->addExtensionesTXT(extensionesCaja);
}

void CategoriasAdd::btnAceptarClicked()
{
    close();
}

void CategoriasAdd::btnCancelarClicked()
{
    close();
}

void CategoriasAdd::btnRestablecerClicked()
{
    if (extensionManager->restoreCategoriasYExtensiones()) {
        /* Ajustes a la UI */
        ui->btnRestablecer->setEnabled(false);
    }
}


/**** MÉTODO AUXILIAR ****/
void CategoriasAdd::checkExtensionBoxCharLimit()
{
    QString textoCaja = ui->cajaExtensiones->toPlainText();
    if (ui->cajaExtensiones->toPlainText().size() > ExtensionManager::maxExtensionBoxChars) {
        textoCaja.truncate(ExtensionManager::maxExtensionBoxChars);
        ui->cajaExtensiones->setPlainText(textoCaja);

        // Para mover el cursor al final (bruh)
        QTextCursor cursor = ui->cajaExtensiones->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->cajaExtensiones->setTextCursor(cursor);

        ui->mensajeError->setText("<span style='color: red;'><b>Límite de caracteres superado.</b></span>");
    }
}
