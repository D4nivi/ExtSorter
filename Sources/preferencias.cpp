#include "Headers/preferencias.h"
#include "ui_preferencias.h"
#include "Headers/customMessageBox.h"
#include "Headers/extensionManager.h"

Preferencias::Preferencias(QSettings * settings, QWidget *parent)
    : QDialog(parent), ui(new Ui::Preferencias), settings(settings)
{
    ui->setupUi(this);

    crearUI();
    conectarSeñales();
}

Preferencias::~Preferencias()
{
    delete ui;
}

/* Método auxiliar para cambiar los valores por defecto de los widgets */
void Preferencias::changeDefaultWidgetsValues()
{
    ui->lineEditNombreCarpetaDestino->setText(settings->value("nombreCarpetaDestino").toString());
    ui->spinBoxFicherosMinimos->setValue(settings->value("minFicherosConfirmacion").toInt());
    ui->comboBoxDefaultBorrarCarpetasVacias->setCurrentIndex(settings->value("defaultBorrarCarpetasVacias").toInt());
    ui->comboBoxDefaultBorrarAccesosDirectos->setCurrentIndex(settings->value("defaultBorrarAccesosDirectos").toInt());
    ui->comboBoxDefaultExcluirOtros->setCurrentIndex(settings->value("defaultExcluirOtros").toInt());
}

/**** MÉTODOS AUXILIARES ****/
void Preferencias::crearUI()
{
    ui->setupUi(this);

    /* --- Configuración inicial de la ventana --- */
    setWindowTitle("Preferencias");

    /* --- Cabecera --- */
    QLabel * textoCabecera = new QLabel("Configura algunos parámetros de la aplicación.");
    textoCabecera->setFixedHeight(50);
    textoCabecera->setStyleSheet(
        "background-color: #FFFFFF; color: black;"
        "font-size: 12px; font-weight: bold;"
        "padding-left: 20px;"
        "border-bottom: 1px solid #c6cac6; border-top: 1px solid #c6cac6;"
        );

    /* Meter texto en la cabecera */
    ui->layoutCabecera->addWidget(textoCabecera, Qt::AlignLeft);

    /* Parámetros iniciales de las preferencias */
    changeDefaultWidgetsValues();

    // Botones
    ui->btnGuardar->setDisabled(true);
    ui->btnRestablecer->setEnabled(settings->value("settingsModified").toBool());
}

void Preferencias::conectarSeñales()
{
    connect(ui->btnGuardar, &QPushButton::clicked, this, &Preferencias::btnGuardarClicked);
    connect(ui->btnAceptar, &QPushButton::clicked, this, &Preferencias::btnAceptarClicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &Preferencias::btnCancelarClicked);
    connect(ui->btnRestablecer, &QPushButton::clicked, this, &Preferencias::btnRestablecerClicked);

    connect(ui->lineEditNombreCarpetaDestino, &QLineEdit::textChanged, this, &Preferencias::enableSave);
    connect(ui->spinBoxFicherosMinimos, &QSpinBox::textChanged, this, &Preferencias::enableSave);
    connect(ui->comboBoxDefaultBorrarCarpetasVacias, &QComboBox::currentIndexChanged, this, &Preferencias::enableSave);
    connect(ui->comboBoxDefaultBorrarAccesosDirectos, &QComboBox::currentIndexChanged, this, &Preferencias::enableSave);
    connect(ui->comboBoxDefaultExcluirOtros, &QComboBox::currentIndexChanged, this, &Preferencias::enableSave);
}

/**** SLOTS ****/
void Preferencias::enableSave()
{
    QString nombreCarpeta = ui->lineEditNombreCarpetaDestino->text();
    ui->btnGuardar->setEnabled(ExtensionManager::isFolderNameValid(nombreCarpeta));
}

/**** SLOTS (UI) ****/

/**** BOTONES ****/
void Preferencias::btnGuardarClicked()
{
    settings->setValue("settingsModified", true);
    settings->setValue("nombreCarpetaDestino", ui->lineEditNombreCarpetaDestino->text());
    settings->setValue("minFicherosConfirmacion", ui->spinBoxFicherosMinimos->value());
    settings->setValue("defaultBorrarCarpetasVacias", ui->comboBoxDefaultBorrarCarpetasVacias->currentIndex());
    settings->setValue("defaultBorrarAccesosDirectos", ui->comboBoxDefaultBorrarAccesosDirectos->currentIndex());
    settings->setValue("defaultExcluirOtros", ui->comboBoxDefaultExcluirOtros->currentIndex());
    settings->sync();

    ui->btnGuardar->setDisabled(true);
    ui->btnRestablecer->setEnabled(true);

    emit opcionesChanged();
}

void Preferencias::btnAceptarClicked()
{
    close();
}

void Preferencias::btnCancelarClicked()
{
    close();
}

void Preferencias::btnRestablecerClicked()
{
    if (CustomMessageBox::mostrarConfirmacion(nullptr, "Confirmar Restablecer preferencias", "¿Estás seguro de que deseas restablecer las preferencias?")) {
        bool filesModified = settings->value("filesModified").toBool();

        settings->clear();
        settings->sync();

        /* No se puede llamar a loadSettings() porque pone siempre filesModified a false */
        settings->setValue("filesModified", filesModified);
        settings->setValue("settingsModified", false);
        settings->setValue("nombreCarpetaDestino", "ExtSorter");
        settings->setValue("minFicherosConfirmacion", 100);
        settings->setValue("defaultBorrarCarpetasVacias", static_cast<int>(ValorOpcion::MANUAL));
        settings->setValue("defaultBorrarAccesosDirectos", static_cast<int>(ValorOpcion::MANUAL));
        settings->setValue("defaultExcluirOtros", static_cast<int>(ValorOpcion::MANUAL));

        changeDefaultWidgetsValues();
        ui->btnRestablecer->setDisabled(true);
        ui->btnGuardar->setDisabled(true);      // Se activa al usar la función changeDefaultWidgetsValues()

        emit opcionesChanged();
    }
}
