#include "Headers/mainwindow.h"
#include "./ui_mainwindow.h"
#include "Headers/categoriasAdd.h"
#include "Headers/categoriasDelete.h"
#include "Headers/customMessageBox.h"
#include "Headers/extensiones.h"
#include "Headers/preferencias.h"

#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), settings(QSettings())
{
    loadSettings();

    excluirOtros = false;
    borrarCarpetasVacias = false;
    borrarAccesosDirectos = false;

    extensionManager.leerJSON();
    extensionManager.escribirTXT();

    crearUI();
    conectarSeñales();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**** MÉTODOS AUXILIARES ****/
void aplicarPreferenciaCheckBox(QCheckBox * checkBox, ValorOpcion preferencia, bool &var) {
    switch (preferencia) {
    case ValorOpcion::SIEMPRE:
        checkBox->setChecked(true);
        checkBox->setEnabled(false);
        var = true;
        break;
    case ValorOpcion::NUNCA:
        checkBox->setChecked(false);
        checkBox->setEnabled(false);
        var = false;
        break;
    case ValorOpcion::MANUAL:
        checkBox->setChecked(false);
        checkBox->setEnabled(true);
        var = false;
        break;
    }
}

void MainWindow::loadSettings()
{
    if (settings.allKeys().isEmpty()) {
        qDebug() << "Archivo de configuración no detectado. Se ha creado el archivo de configuración";
        settings.setValue(PrefsNames::filesModified, false);
        settings.setValue(PrefsNames::settingsModified, false);
        settings.setValue(PrefsNames::nombreCarpetaDestino, "ExtSorter");
        settings.setValue(PrefsNames::showWarnings, true);
        settings.setValue(PrefsNames::minFicherosConfirmacion, 100);
        settings.setValue(PrefsNames::defaultBorrarCarpetasVacias, static_cast<int>(ValorOpcion::MANUAL));
        settings.setValue(PrefsNames::defaultBorrarAccesosDirectos, static_cast<int>(ValorOpcion::MANUAL));
        settings.setValue(PrefsNames::defaultExcluirOtros, static_cast<int>(ValorOpcion::MANUAL));
    }
    /* Debug
    QStringList a = settings.allKeys();
    qDebug() << "--- Preferencias ---";

    for (const auto &e : a) {
        qDebug() << QString("%1: %2").arg(e, settings.value(e).toString());
    }

    qDebug() << Qt::endl;
    */
}

void MainWindow::crearUI()
{
    ui->setupUi(this);
    ui->btnComenzar->setEnabled(false);

    /* Cabecera */
    QLabel * cabecera = new QLabel();
    cabecera->setFixedSize(400, 80);
    cabecera->setPixmap(QPixmap(":/images/cabecera-main-window.png").scaled(cabecera->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    cabecera->setAlignment(Qt::AlignCenter);
    cabecera->setStyleSheet("background-color: #FFFFFF; border-top: 0px solid #c6cac6; border-bottom: 1px solid #c6cac6;");
    ui->layoutCabecera->addWidget(cabecera, 0, Qt::AlignHCenter);

    /* --- Instrucciones ---*/
    ui->textoInfo->setStyleSheet("font-size: 12px; font-weight: bold;");

    /* CheckBoxes */
    aplicarPreferenciaCheckBox(ui->chkBorrarCarpetas, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultBorrarCarpetasVacias).toInt()), borrarCarpetasVacias);
    aplicarPreferenciaCheckBox(ui->chkBorrarAccesosDirectos, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultBorrarAccesosDirectos).toInt()), borrarAccesosDirectos);
    aplicarPreferenciaCheckBox(ui->chkExcluirOtros, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultExcluirOtros).toInt()), excluirOtros);

    /* --- Footer --- */
    ui->colorFondo->setStyleSheet("background-color: white; border-top: 1px solid #c6cac6;");
}

void MainWindow::conectarSeñales()
{
    connect(ui->actionAbrir, &QAction::triggered, this, &MainWindow::actionAbrirTriggered);
    connect(ui->actionAddExtensiones, &QAction::triggered, this, &MainWindow::actionAddExtensionesTriggered);
    connect(ui->actionAddCategorias, &QAction::triggered, this, &MainWindow::actionAddCategoriastriggered);
    connect(ui->actionDeleteCategorias, &QAction::triggered, this, &MainWindow::actionDeleteCategoriastriggered);
    connect(ui->actionSalir, &QAction::triggered, this, &MainWindow::actionSalirTriggered);

    connect(ui->actionListar, &QAction::triggered, this, &MainWindow::actionListarTriggered);
    connect(ui->actionRestablecer, &QAction::triggered, this, &MainWindow::actionRestablecerTriggered);
    connect(ui->actionPreferencias, &QAction::triggered, this, &MainWindow::actionPreferenciasTriggered);

    connect(ui->actionVerAyuda, &QAction::triggered, this, &MainWindow::actionVerAyudaTriggered);
    connect(ui->actionAcercaDe, &QAction::triggered, this, &MainWindow::actionAcercaDeTriggered);

    connect(ui->btnComenzar, &QPushButton::clicked, this, &MainWindow::btnComenzarClicked);
    connect(ui->btnSelecCarpeta, &QPushButton::clicked, this, &MainWindow::btnSelecCarpetaClicked);

    connect(ui->chkExcluirOtros, &QCheckBox::checkStateChanged, this, &MainWindow::chkExcluirOtrosStateChanged);
    connect(ui->chkBorrarCarpetas, &QCheckBox::checkStateChanged, this, &MainWindow::chkBorrarCarpetasStateChanged);
    connect(ui->chkBorrarAccesosDirectos, &QCheckBox::checkStateChanged, this, &MainWindow::chkBorrarSymlinksStateChanged);

    connect(this, &MainWindow::sortComplete, this, &MainWindow::changeWindow);
}


/**** SLOTS AUXILIARES ****/
void MainWindow::changeWindow()
{
    this->hide();
    QString nombreCarpetaDestino = settings.value("nombreCarpetaDestino").toString();
    int minFicherosConfirmacion = settings.value("minFicherosConfirmacion").toInt();

    FinalWindow * ventanaFinal = new FinalWindow(minFicherosConfirmacion, carpetaSeleccionada, nombreCarpetaDestino, &fileCounter, nullptr);
    connect(ventanaFinal, &FinalWindow::closeMainWindow, this, &MainWindow::actionSalirTriggered);
    ventanaFinal->exec();

    this->show();
}

void MainWindow::restoreMainWindowValues()
{
    carpetaSeleccionada = nullptr;
    fileCounter.clear();

    ui->btnComenzar->setEnabled(false);
    aplicarPreferenciaCheckBox(ui->chkBorrarCarpetas, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultBorrarCarpetasVacias).toInt()), borrarCarpetasVacias);
    aplicarPreferenciaCheckBox(ui->chkBorrarAccesosDirectos, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultBorrarAccesosDirectos).toInt()), borrarAccesosDirectos);
    aplicarPreferenciaCheckBox(ui->chkExcluirOtros, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultExcluirOtros).toInt()), excluirOtros);
    ui->textoSelectCarpeta->setText("");
}


/**** SLOTS (UI) ****/

/**** BOTONES ****/
void MainWindow::btnComenzarClicked()
{
    /* Pre-Condiciones */
    QDir carpetaOrigen(carpetaSeleccionada);

    if (carpetaOrigen.exists() && carpetaOrigen.isEmpty(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        CustomMessageBox::warning(nullptr, "Carpeta vacía", "La carpeta seleccionada está vacía.");
        return;
    }

    QFileInfo info(carpetaSeleccionada);
    qEnableNtfsPermissionChecks();

    qDebug() << "Leible:" << info.isReadable() << Qt::endl << "Escribible:" << info.isWritable();
    if (!(info.isReadable() && info.isWritable())) {
        qDisableNtfsPermissionChecks();
        QMessageBox::critical(nullptr, "Denegado", "Permisos insuficientes.");
        return;
    }

    qDisableNtfsPermissionChecks();
    this->setEnabled(false);
    QCoreApplication::processEvents();      // Si no pongo esto no se deshabilita la pantalla

    /* Algoritmo */
    QString nombreCarpetaDestino = settings.value("nombreCarpetaDestino").toString();
    QDir carpetaDestino = QDir(carpetaSeleccionada + "/" + nombreCarpetaDestino);
    QFileInfoList archivos = carpetaOrigen.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    QMap<QString, QSet<QString>> * categoriasYExtensiones = extensionManager.getCategoriasYExtensiones();
    int carpetasEncontradas = 0;
    int symlinksEncontrados = 0;
    carpetaOrigen.mkdir(nombreCarpetaDestino);

    qDebug() << "Carpeta Destino creada con éxito:" << carpetaDestino.absolutePath();
    qDebug() << "Se han detectado" << archivos.size() << "archivos en la carpeta de origen.";

    for (const QFileInfo &fichero : std::as_const(archivos)) {
        /* Tratamiento de ficheros */
        if (fichero.isFile() && !fichero.isSymLink()){
            QString extension = fichero.suffix().toLower();
            QString categoria = "Otros";

            for (auto it = categoriasYExtensiones->begin(); it != categoriasYExtensiones->end(); it++) {
                if (it->values().contains(extension)) categoria = it.key();
            }

            QString subcarpeta = carpetaDestino.absolutePath() + "/" + categoria;

            if (categoria != "Otros" || (categoria == "Otros" && !excluirOtros)){
                if (!fileCounter.contains(categoria)) {
                    QDir().mkpath(subcarpeta);
                    fileCounter.insert(categoria, 0);
                    qDebug() << "Carpeta" << categoria << "creada.";
                }

                QFile::rename(carpetaSeleccionada + "/" + fichero.fileName(), subcarpeta + "/" + fichero.fileName());
                fileCounter[categoria]++;
            }
        }

        /* Tratamiento de carpetas vacías (opcional) */
        else if (fichero.isDir() && !fichero.isSymLink()) {
            if (borrarCarpetasVacias && QDir(fichero.absoluteFilePath()).isEmpty()) {
                carpetaOrigen.rmdir(fichero.fileName());
                fileCounter.insert("Carpeta", ++carpetasEncontradas);
            }
        }

        /* Tratamiento de accesos directos (opcional) */
        else if (borrarAccesosDirectos && fichero.isSymLink()) {
            QFile(fichero.filePath()).remove();
            fileCounter.insert("Acceso Directo", ++symlinksEncontrados);
        }

        /* Tratamiento de otros tipos de ficheros */
        else {
            if (!fileCounter.contains("Otros")) QDir().mkpath(carpetaDestino.absolutePath() + "/Otros");
            QFile::rename(carpetaSeleccionada + "/" + fichero.fileName(), carpetaDestino.absolutePath() + "/Otros/" + fichero.fileName());
        }
    }

    /* Tratamiento final */
    qDebug() << "Proceso finalizado.";

    if (fileCounter.isEmpty()) {
        carpetaOrigen.rmdir(nombreCarpetaDestino);
        qDebug() << "Aviso: No se han encontrado archivos para tratar.";
        CustomMessageBox::warning(nullptr, "Aviso", "No se han encontrado archivos para tratar.");
    }
    else {
        QString msg;
        QStringList keys = fileCounter.keys();

        if (fileCounter.contains("Carpeta")) {
            qDebug() << QString("%1 carpeta(s) vacía(s) eliminada(s).").arg(fileCounter["Carpeta"]);
            msg.append(QString("\r\n%1 carpeta(s) vacía(s) eliminada(s).").arg(fileCounter["Carpeta"]));
            fileCounter.remove("Carpeta");
        }

        if (fileCounter.contains("Acceso Directo")) {
            qDebug() << QString("%1 acceso(s) directo(s) eliminado(s).").arg(fileCounter["Acceso Directo"]);
            msg.append(QString("\r\n%1 acceso(s) directo(s) eliminado(s).").arg(fileCounter["Acceso Directo"]));
            fileCounter.remove("Acceso Directo");
        }

        keys.removeOne("Carpeta");
        keys.removeOne("Acceso Directo");

        if (keys.isEmpty()) {
            carpetaOrigen.rmdir(nombreCarpetaDestino);
            CustomMessageBox::info(nullptr, "Completado", "No se han encontrado archivos para tratar." + msg);
        } else {
            qDebug() << "Diccionario resultante:";
            for (auto it = fileCounter.constBegin(); it != fileCounter.constEnd(); ++it) {
                qDebug() << "Tipo:" << it.key() << "->" << it.value() << "archivos";
            }

            CustomMessageBox::info(nullptr, "Completado", "Archivos ordenados correctamente." + msg);
            emit sortComplete();
        }

        restoreMainWindowValues();
    }

    this->setEnabled(true);
}

void MainWindow::btnSelecCarpetaClicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList archivos = dialog.selectedFiles();
        carpetaSeleccionada = archivos.first();
        ui->textoSelectCarpeta->setText("Carpeta seleccionada:\n" + carpetaSeleccionada);
        ui->btnComenzar->setEnabled(true);

        qDebug() << "Carpeta seleccionada:" + carpetaSeleccionada;
    } else {
        qDebug() << "El usuario canceló la selección de carpeta.";
    }
}


/**** CHECKBOXES ****/
void MainWindow::chkExcluirOtrosStateChanged()
{
    excluirOtros = !excluirOtros;
    qDebug() << "Excluir Otros: {" << excluirOtros << "}";
}

void MainWindow::chkBorrarCarpetasStateChanged()
{
    borrarCarpetasVacias = !borrarCarpetasVacias;
    qDebug() << "Borrar Carpetas Vacias: {" << borrarCarpetasVacias << "}";
}

void MainWindow::chkBorrarSymlinksStateChanged()
{
    borrarAccesosDirectos = !borrarAccesosDirectos;
    qDebug() << "Borrar Accesos Directos: {" << borrarAccesosDirectos << "}";
}


/**** ACCIONES ****/
void MainWindow::actionAbrirTriggered()
{
    btnSelecCarpetaClicked();
}

void MainWindow::actionAddExtensionesTriggered()
{
    Extensiones * ventana = new Extensiones(&extensionManager, this);
    ventana->setModal(true);
    ventana->show();
}

void MainWindow::actionAddCategoriastriggered()
{
    CategoriasAdd * ventana = new CategoriasAdd(&extensionManager, this);
    ventana->setModal(true);
    ventana->show();
}

void MainWindow::actionDeleteCategoriastriggered()
{
    CategoriasDelete * ventana = new CategoriasDelete(&extensionManager, this);
    ventana->setModal(true);
    ventana->show();
}

void MainWindow::actionSalirTriggered()
{
    QApplication::quit();
}

void MainWindow::actionListarTriggered()
{
    QDialog ventana = QDialog(this);

    /* --- Configuración inicial de la ventana --- */
    ventana.setWindowTitle("Categorías y Extensiones");

    /* --- Tabla --- */
    QTableWidget tabla = QTableWidget(extensionManager.getCategorias().size(), 3, &ventana);
    tabla.setHorizontalHeaderLabels({"Categoría", "Extensiones", "Nº extensiones"});

    /* Añadir datos a la tabla */
    QMap<QString, QSet<QString>> * data = extensionManager.getCategoriasYExtensiones();
    QStringList categorias = data->keys();
    QList<QSet<QString>> listaExtensiones = data->values();

    for (int i = 0; i < categorias.size(); i++) {
        QStringList extensiones = listaExtensiones.value(i).values();
        extensiones.sort(Qt::CaseInsensitive);

        tabla.setItem(i, 0, new QTableWidgetItem(categorias.at(i)));
        tabla.setItem(i, 1, new QTableWidgetItem(extensiones.join(", ")));

        QTableWidgetItem * n_extensiones = new QTableWidgetItem(QString::number(extensiones.size()));
        n_extensiones->setTextAlignment(Qt::AlignCenter);
        tabla.setItem(i, 2, n_extensiones);
    }

    tabla.resizeColumnsToContents();
    tabla.resizeRowsToContents();
    tabla.sortByColumn(0, Qt::AscendingOrder);

    /* Botón */
    QPushButton btnAceptar("Aceptar", &ventana);

    /* Layout */
    QVBoxLayout layout = QVBoxLayout(&ventana);
    QHBoxLayout layoutBoton;

    layout.addWidget(&tabla, Qt::AlignHCenter);
    layoutBoton.addStretch();
    layoutBoton.addWidget(&btnAceptar);
    layout.addLayout(&layoutBoton);

    connect(&btnAceptar, &QPushButton::clicked, &ventana, &QDialog::accept);

    /* Ajustar tamaño de la ventana */
    tabla.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ventana.adjustSize();
    ventana.setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true); // evita que se pueda redimensionar

    ventana.exec();
}

void MainWindow::actionRestablecerTriggered()
{
    if (settings.value("filesModified").toBool()) {
        extensionManager.restoreCategoriasYExtensiones();
    } else {
        CustomMessageBox::info(this, "Restablecer Extensiones", "No se han modificado las extensiones.");
    }
}

void MainWindow::actionPreferenciasTriggered()
{
    Preferencias * ventana = new Preferencias(this);
    connect(ventana, &Preferencias::opcionesChanged, this, [this]() {
        aplicarPreferenciaCheckBox(ui->chkBorrarCarpetas, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultBorrarCarpetasVacias).toInt()), borrarCarpetasVacias);
        aplicarPreferenciaCheckBox(ui->chkBorrarAccesosDirectos, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultBorrarAccesosDirectos).toInt()), borrarAccesosDirectos);
        aplicarPreferenciaCheckBox(ui->chkExcluirOtros, static_cast<ValorOpcion>(settings.value(PrefsNames::defaultExcluirOtros).toInt()), excluirOtros);
    });
    ventana->setModal(true);
    ventana->show();
}

void MainWindow::actionVerAyudaTriggered()
{
    QUrl url("https://github.com/D4nivi/ExtSorter/blob/master/README.md#cómo-usar");
    QDesktopServices::openUrl(url);
}

void MainWindow::actionAcercaDeTriggered()
{
    QDialog ventana = QDialog(this);

    /* --- Configuración inicial de la ventana --- */
    ventana.setWindowTitle("Acerca de ExtSorter");
    ventana.resize(200, 100);

    /* Logo */
    QLabel logo;
    logo.setPixmap(QPixmap(":/images/cabecera-main-window.png").scaled(logo.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    /* Linea separación */
    QFrame linea;
    linea.setFrameShape(QFrame::HLine);
    linea.setFrameShadow(QFrame::Sunken);

    /* Texto */
    QLabel label("", &ventana);
    label.setTextFormat(Qt::RichText);
    label.setText("ExtSorter creador por <b>Daniel Vidal (<i>Danivi</i>)</b>.<br><b>Versión</b>: 1.0<br><a href=\"https://github.com/D4nivi/ExtSorter\"><b>Repositorio de GitHub</b></a>");
    label.setOpenExternalLinks(true);

    /* Botón */
    QPushButton btnAceptar("Aceptar", &ventana);

    /* Layouts */
    QVBoxLayout layoutPrincipal(&ventana);
    QHBoxLayout layoutBoton;

    layoutBoton.addStretch();
    layoutBoton.addWidget(&btnAceptar);

    layoutPrincipal.addWidget(&logo);
    layoutPrincipal.addWidget(&linea);
    layoutPrincipal.addWidget(&label);
    layoutPrincipal.addLayout(&layoutBoton);

    connect(&btnAceptar, &QPushButton::clicked, &ventana, &QDialog::accept);

    ventana.exec();
}
