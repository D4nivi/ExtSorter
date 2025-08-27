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
    ui->btnRestablecer->setEnabled(ExtensionManager::filesModified);

    // Cajas de texto
    ui->mensajeError->setText("");
    ui->cajaCategoria->setMaxLength(ExtensionManager::maxCategoryChars);
    ui->cajaCategoria->setPlaceholderText("Puedes usar emojis. Por ejemplo: 🌐 Web, ✨ Adobe, ...");
    ui->cajaExtensiones->setPlaceholderText("Escribe las extensiones separadas por comas (,).\nPor ejemplo: png,jpg,tiff...");

    /* --- Cabecera --- */
    QLabel * textoCabecera = new QLabel("Añade tus propias categorías y personaliza al máximo el ordenamiento.");
    textoCabecera->setFixedHeight(50);
    textoCabecera->setWordWrap(true);
    textoCabecera->setStyleSheet(
        "background-color: #FFFFFF; color: black;"
        "font-size: 12px; font-weight: bold;"
        "padding-left: 20px; padding-bottom: 10px;"
        "border-bottom: 1px solid #c6cac6;"
        );

    /* Meter texto y logo en la cabecera */
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
    QString categoriaRepetida;
    bool catRep = extensionManager->getCategorias().contains(nuevaCategoria, Qt::CaseInsensitive);

    if (catRep) {
        categoriaRepetida = QString(nuevaCategoria[0].toUpper() + nuevaCategoria.sliced(1).toLower());     // Esto es como un .capitalize()
        ui->mensajeError->setText(QString("<span style='color: red;'><b>Categoría ya existente</b></span>: la categoría <b>\"%1\"</b> ya existe.").arg(categoriaRepetida));
    }

    bool categoriaCorrecta = !nuevaCategoria.isEmpty() && !catRep;

    /* Nota: esta comprobación se hacía con un else if dentro de checkExtensionListFormat para evitar que apareciera texto
     * cuando se borraba de golpe el cuadro de categoria. Se deja aquí porque nunca se comprobaría al asignar "activar",
     * ya que si esto pasa categoriaCorrecta siempre es false, y nunca se llega a entrar a checkExtensionListFormat. */
    if (textoCaja.isEmpty() && !catRep) ui->mensajeError->setText("");

    // Activar el botón
    bool activar = categoriaCorrecta && extensionManager->checkExtensionListFormat(textoCaja, ui->mensajeError);
    if (activar) ui->mensajeError->setText("");
    ui->btnGuardar->setEnabled(activar);
}

/**** BOTONES ****/
void CategoriasAdd::btnGuardarClicked()
{
    /* --- Actualizar Atributos (internos y de clase ExtensionManager) --- */
    QStringList categorias = extensionManager->getCategorias();
    QStringList extensionesCaja = ui->cajaExtensiones->toPlainText().split(",");
    QSet<QString> nuevasExtensiones;
    QStringList nuevasExtensionesList;
    categorias.append(nuevaCategoria);

    qDebug() << "Nueva Categoría:" << nuevaCategoria << Qt::endl << "Nuevas Extensiones:" << extensionesCaja;

    for (const QString &ext : std::as_const(extensionesCaja)) {
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
    extensionManager->addExtensionesTXT(ui->cajaExtensiones->toPlainText());
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
