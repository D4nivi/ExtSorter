#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "finalwindow.h"
#include "extensionManager.h"
#include "preferencias.h"

#include <QMainWindow>
#include <QFile>
#include <QTableWidget>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /* --- Estado interno --- */
    bool excluirOtros;
    bool borrarCarpetasVacias;
    bool borrarAccesosDirectos;
    QString carpetaSeleccionada;
    QMap<QString, int> fileCounter;

    QSettings settings;
    ExtensionManager extensionManager;

private slots:
    void btnComenzarClicked();

    void btnSelecCarpetaClicked();

    void chkExcluirOtrosStateChanged();

    void chkBorrarCarpetasStateChanged();

    void chkBorrarSymlinksStateChanged();

    void actionAbrirTriggered();

    void actionAddExtensionesTriggered();

    void actionAddCategoriastriggered();

    void actionDeleteCategoriastriggered();

    void actionSalirTriggered();

    void actionListarTriggered();

    void actionRestablecerTriggered();

    void actionPreferenciasTriggered();

    void actionVerAyudaTriggered();

    void actionAcercaDeTriggered();

    void changeWindow();

    void restoreMainWindowValues();

signals:
    void sortComplete();    // Señal enviada cuando se termina el proceso del botón Comenzar

/* --- Métodos auxiliares --- */
private:
    void loadSettings();
    void crearUI();
    void conectarSeñales();
};
#endif // MAINWINDOW_H
