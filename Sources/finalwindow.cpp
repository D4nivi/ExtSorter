#include "Headers/finalwindow.h"
#include "Headers/mainwindow.h"
#include "ui_finalwindow.h"
#include "Headers/zipworker.h"
#include "Headers/customMessageBox.h"

/* Emojis */
QHash<QString, QString> emojis = {
    {"Audio", "🎵"}, {"Código", "💻"}, {"Comprimido", "📦"}, {"Documento", "📄"}, {"Ejecutable", "⚙️"},
    {"Imagen", "🖼️"}, {"Otros", "📌"}, {"Proyecto", "🗂️"}, {"Texto", "📜"}, {"Video", "🎬"}
};

FinalWindow::FinalWindow(QString carpetaSeleccionada, QMap<QString, int> fileCounter, QWidget *parent)
    : QDialog(parent), ui(new Ui::FinalWindow), carpetaSeleccionada(carpetaSeleccionada), fileCounter(fileCounter)
{
    lista = new QListWidget(this);
    crearUI();
    conectarSeñales();
}

FinalWindow::~FinalWindow()
{
    delete ui;
}

/**** MÉTODOS AUXILIARES ****/
void FinalWindow::crearUI()
{
    ui->setupUi(this);
    this->setFixedSize(400, 390);
    this->setWindowTitle("ExtSorter");
    ui->btnAplicar->setEnabled(false);
    btnSeleccionarTodo = new QPushButton("Seleccionar todo", this);
    btnAbrirCarpeta = new QPushButton("Abrir Carpeta", this);

    /* --- Cabecera --- */
    QLabel * textoCabecera = new QLabel("Manipula las carpetas creadas.");
    textoCabecera->setFixedHeight(60);
    textoCabecera->setWordWrap(true);
    textoCabecera->setStyleSheet(
        "background-color: #FFFFFF; color: black;"
        "font-size: 12px; font-weight: bold;"
        "padding-left: 20px;"
        "border-bottom: 1px solid #c6cac6; border-top: 1px solid #c6cac6;"
    );

    /* Meter texto y logo en la cabecera */
    ui->layoutCabcera->addWidget(textoCabecera, Qt::AlignLeft);

    /* --- Cuadro Casillas --- */
    QGroupBox * groupBox = new QGroupBox("Selecciona las carpetas a manipular", this);
    QVBoxLayout * layoutCuadro = new QVBoxLayout();
    QHBoxLayout * layoutPrincipal = new QHBoxLayout(this);
    QHBoxLayout * layoutBotones = new QHBoxLayout();

    /* Rellenar lista con casillas */
    QStringList opciones = fileCounter.keys();

    // Esta gilipollez es para que Otros aparezca al final siempre, y no en medio
    if (opciones.contains("Otros")) {
        opciones.remove(opciones.indexOf("Otros"));
        opciones.append("Otros");
    }

    for (const QString &categoria : std::as_const(opciones)) {
        QString textoCasilla = QString("%1 %2 (%3 elementos)")
            .arg(emojis.value(categoria))
            .arg(categoria)
            .arg(fileCounter.value(categoria));

        QListWidgetItem * item = new QListWidgetItem(textoCasilla, lista);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    // Texto para cuando no haya más casillas
    QLabel * placeholder = new QLabel("No hay más carpetas para manipular.", lista);
    placeholder->setStyleSheet("color: gray; font-style: italic; padding: 5px 0px 0px 5px;");
    placeholder->setVisible(false);
    connect(this, &FinalWindow::mostrarTextoCuadro, this, [=]{ placeholder->setVisible(lista->count() == 0); });

    /* --- Layouts --- */
    layoutBotones->addWidget(btnSeleccionarTodo);
    layoutBotones->addStretch();                    // empuja el segundo botón a la derecha
    layoutBotones->addWidget(btnAbrirCarpeta);

    layoutCuadro->addWidget(lista, 0, Qt::AlignHCenter);
    layoutCuadro->addLayout(layoutBotones);

    lista->setFixedWidth(350);
    groupBox->setFixedSize(370, 250);
    groupBox->setLayout(layoutCuadro);

    layoutPrincipal->addWidget(groupBox, 0, Qt::AlignVCenter);
}

void FinalWindow::conectarSeñales()
{
    connect(ui->barraSeleccion, &QComboBox::currentIndexChanged, this, &FinalWindow::enableApply);
    connect(ui->btnVolver, &QPushButton::clicked, this, &FinalWindow::btnVolverClicked);
    connect(ui->btnTerminar, &QPushButton::clicked, this, &FinalWindow::btnTerminarClicked);
    connect(ui->btnAplicar, &QPushButton::clicked, this, &FinalWindow::btnAplicarClicked);
    connect(btnSeleccionarTodo, &QPushButton::clicked, this, &FinalWindow::seleccionarTodoClicked);
    connect(btnSeleccionarTodo, &QPushButton::clicked, this, &FinalWindow::enableApply);
    connect(btnAbrirCarpeta, &QPushButton::clicked, this, &FinalWindow::btnAbrirCarpetaClicked);
    connect(lista, &QListWidget::itemChanged, this, &FinalWindow::casillaCambiada);
    connect(lista, &QListWidget::itemChanged, this, &FinalWindow::enableApply);
    connect(this, &FinalWindow::selecccionarTodoRefresh, this, &FinalWindow::seleccionarTodoClicked);
}


/**** SLOTS (UI) ****/
void FinalWindow::casillaCambiada(QListWidgetItem *item)
{
    if (actualizandoTodas) return;

    if (item->checkState() == Qt::Checked) {
        casillasMarcadas++;
        carpetasSeleccionadas.insert(item->text().split(" ")[1]);
    } else {
        casillasMarcadas--;
        carpetasSeleccionadas.remove(item->text().split(" ")[1]);
    }

    todoSeleccionado = (casillasMarcadas == lista->count());
    btnSeleccionarTodo->setText(todoSeleccionado ? "Deseleccionar todo" : "Seleccionar todo");
}

void FinalWindow::enableApply()
{
    qDebug() << "Carpetas Seleccionadas:" << carpetasSeleccionadas;
    qDebug() << "Acción:" << ui->barraSeleccion->itemText(ui->barraSeleccion->currentIndex());
    ui->btnAplicar->setEnabled(casillasMarcadas > 0 && ui->barraSeleccion->currentIndex() > 0);
}

/**** BOTONES ****/
void FinalWindow::seleccionarTodoClicked()
{
    actualizandoTodas = true;

    todoSeleccionado = !todoSeleccionado;
    btnSeleccionarTodo->setText(todoSeleccionado ? "Deseleccionar todo" : "Seleccionar todo");

    for (int i = 0; i < lista->count(); i++) {
        lista->item(i)->setCheckState(todoSeleccionado ? Qt::Checked : Qt::Unchecked);

        if (todoSeleccionado) carpetasSeleccionadas.insert(lista->item(i)->text().split(" ")[1]);
        else carpetasSeleccionadas.remove(lista->item(i)->text().split(" ")[1]);
    }

    casillasMarcadas = todoSeleccionado ? lista->count() : 0;
    actualizandoTodas = false;
}

void FinalWindow::btnVolverClicked()
{
    done(0);
    emit backToMainWindow();
}

void FinalWindow::btnTerminarClicked()
{
    done(0);
    emit closeMainWindow();
}

void FinalWindow::btnAbrirCarpetaClicked()
{
    QUrl url = QUrl::fromLocalFile(carpetaSeleccionada + "/" + MainWindow::nombreCarpetaDestino);
    QDesktopServices::openUrl(url);
}

void FinalWindow::btnAplicarClicked()
{
    this->setEnabled(false);
    QApplication::processEvents();    // Aunque no me guste, si no pongo esto la pantalla no se desactiva al comprimir

    QString rutaCarpeta = carpetaSeleccionada + "/" + MainWindow::nombreCarpetaDestino;
    int index = ui->barraSeleccion->currentIndex();
    QString accion = ui->barraSeleccion->itemText(ui->barraSeleccion->currentIndex());
    QMap<QString, bool> carpetasProcesadas;
    int n_archivos = 0;

    for (const QString &categoria : std::as_const(carpetasSeleccionadas)) {
        n_archivos += fileCounter.value(categoria);
    }

    qDebug() << "Ruta Carpeta:" << rutaCarpeta;
    qDebug() << "Archivos a manipular:" << n_archivos;

    /* --- Comprimir --- */
    if (index == 1) {
        aplicarComprimir(rutaCarpeta);
    }

    else {
        if (n_archivos >= 100) {
            IconManager::IconType icono = accion == "Eliminar" ? IconManager::IconType::Eliminar : IconManager::IconType::Mover;
            QString msg = QString("Estás a punto de %1 %2 archivos. ¿Deseas continuar?").arg(accion.toLower()).arg(n_archivos);

            if (!CustomMessageBox::mostrarConfirmacion(this, "Confirmar " + accion, msg, icono)) {
                this->setEnabled(true);
                return;
            }
        }

        bool success = false;
        if (index == 2)
            success = aplicarEliminar(rutaCarpeta, carpetasProcesadas);
        else
            success = aplicarMover(rutaCarpeta, carpetasProcesadas);

        if (!success) {
            this->setEnabled(true);
            return;
        }

        actualizarLista(carpetasProcesadas, accion);
        emit mostrarTextoCuadro();
    }

    this->setEnabled(true);
}


/**** MÉTODOS AUXILIARES (para función btnAplicarClicked) ****/
void FinalWindow::aplicarComprimir(QString rutaCarpeta)
{
    /* Barra de progreso */
    QDialog * dialog = new QDialog(this);
    dialog->setWindowTitle("Comprimiendo...");
    dialog->setFixedSize(300, 100);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    QVBoxLayout * layout = new QVBoxLayout(dialog);

    QLabel * label = new QLabel("Comprimiendo carpetas. Esto puede tardar un poco.", dialog);
    label->setAlignment(Qt::AlignCenter);

    QProgressBar * progress = new QProgressBar(dialog);
    progress->setRange(0, 0);  // modo indeterminado
    progress->setTextVisible(false);

    layout->addWidget(label);
    layout->addWidget(progress);

    dialog->show();

    /* Creación de hilo para la compresión */
    QThread * thread = new QThread(this);
    ZipWorker * worker = new ZipWorker(rutaCarpeta, carpetasSeleccionadas);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &ZipWorker::run);
    connect(worker, &ZipWorker::finished, this, [this, thread, worker, dialog, rutaCarpeta](int exitCode) {
        dialog->close();

        if (exitCode == 0) {
            CustomMessageBox::info(this, "Completado", "Carpeta(s) comprimida(s) con éxito.");

            /* Abrir la carpeta tras finalizar */
            QUrl url = QUrl::fromLocalFile(rutaCarpeta);
            QDesktopServices::openUrl(url);
        }
        else QMessageBox::critical(this, "Error", "Ha ocurrido un error al intentar comprimir las carpetas.");

        thread->quit();
        thread->wait();
        worker->deleteLater();
        thread->deleteLater();
    });

    thread->start();
}

bool FinalWindow::aplicarEliminar(const QString &rutaCarpeta, QMap<QString, bool> &carpetasProcesadas)
{
    for (const QString &categoria : std::as_const(carpetasSeleccionadas)) {
        QDir carpetaCategoria(rutaCarpeta + "/" + categoria);
        QFileInfoList ficheros = carpetaCategoria.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

        for (const QFileInfo &fichero : std::as_const(ficheros)) {
            QFile::remove(fichero.absoluteFilePath());
        }
        carpetasProcesadas.insert(categoria, carpetaCategoria.rmdir(carpetaCategoria.absolutePath()));
    }

    return true;
}

bool FinalWindow::aplicarMover(const QString &rutaCarpeta, QMap<QString, bool> &carpetasProcesadas)
{
    QString carpetaDestino;
    CustomMessageBox::info(this, "Selección de carpeta", "Selecciona la carpeta donde mover los archivos.");

    /* Selección de carpeta y comprobación de permisos */
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (dialog.exec() != QDialog::Accepted) {
        qDebug() << "El usuario canceló la selección de carpeta.";
        return false;
    }

    /* NOTA MUY IMPORTANTE: No meter comprobación de permisos con qEnableNtfsPermissionChecks(), no puto funciona */

    /* Algoritmo */
    QStringList archivos = dialog.selectedFiles();
    carpetaDestino = archivos.first();
    qDebug() << "Carpeta seleccionada:" << carpetaDestino;

    for (const QString &categoria : std::as_const(carpetasSeleccionadas)) {
        QDir carpetaOrigen(rutaCarpeta + "/" + categoria);
        bool success = QDir().rename(carpetaOrigen.absolutePath(), QString("%1/%2").arg(carpetaDestino, categoria));
        carpetasProcesadas.insert(categoria, success);
    }

    return true;
}

void FinalWindow::actualizarLista(const QMap<QString, bool> &carpetasProcesadas, const QString &accion)
{
    int i = 0;
    QStringList carpetasError;

    while (i < lista->count()) {
        QString carpeta = lista->item(i)->text().split(" ")[1];

        if (!carpetasProcesadas.contains(carpeta))
            i++;
        else if (carpetasProcesadas.value(carpeta)) {
            carpetasSeleccionadas.remove(carpeta);
            delete lista->takeItem(i);
        } else {
            carpetasError.append(carpeta);
            lista->item(i)->setCheckState(Qt::Unchecked);
            i++;
        }
    }

    /* Cuadros de información finales */
    if (carpetasError.isEmpty())
        CustomMessageBox::info(this, "Completado", "Acción completada correctamente.");
    else if (carpetasError.size() == carpetasProcesadas.size())
        QMessageBox::critical(this, "Denegado", QString("No se pudieron %1 los archivos.").arg(accion.toLower()));
    else {
        QString msgError = "Acción completada con fallos. Las siguientes carpetas no pudieron ser manipuladas:";

        for (const QString &dir : carpetasError) {
            msgError.append(QString(" %1").arg(dir));
        }

        msgError.append(".");
        CustomMessageBox::warning(this, "Acción completada con fallos", msgError);
    }

    /* Ajustes finales a la UI */
    ui->btnAplicar->setEnabled(false);
    todoSeleccionado = true;
    emit selecccionarTodoRefresh();
}
