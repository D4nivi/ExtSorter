#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "finalwindow.h"
#include "extensionManager.h"

#include <QMainWindow>
#include <QFile>
#include <QTableWidget>


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

    static const QString nombreCarpetaDestino;

private:
    Ui::MainWindow *ui;

    /* --- Estado interno --- */
    bool excluirOtros;
    bool borrarCarpetasVacias;
    bool borrarAccesosDirectos;
    QString carpetaSeleccionada;
    QMap<QString, int> fileCounter;

    ExtensionManager extensionManager;
    FinalWindow * ventanaFinal;

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

    void actionVerAyudaTriggered();

    void actionAcercaDeTriggered();

    void changeWindow();

    void restoreMainWindowValues();

signals:
    void sortComplete();    // Señal enviada cuando se termina el proceso del botón Comenzar

private:
    /* --- Métodos auxiliares --- */
    void crearUI();
    void conectarSeñales();
};
#endif // MAINWINDOW_H
