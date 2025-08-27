#ifndef FINALWINDOW_H
#define FINALWINDOW_H

#include <QDesktopServices>
#include <QFileDialog>
#include <QGroupBox>
#include <QListWidget>
#include <QThread>

class MainWindow;

namespace Ui {
class FinalWindow;
}

class FinalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FinalWindow(QString carpetaSeleccionada, QMap<QString, int> fileCounter, QWidget *parent = nullptr);
    ~FinalWindow();

private:
    Ui::FinalWindow *ui;

    /* --- Estado interno --- */
    bool todoSeleccionado = false;
    bool actualizandoTodas = false;
    int casillasMarcadas = 0;
    QSet<QString> carpetasSeleccionadas;

    /* --- Atributos necesarios del padre --- */
    QString carpetaSeleccionada;
    QMap<QString, int> fileCounter;

    /* --- Widgets principales --- */
    QListWidget * lista;
    QPushButton * btnSeleccionarTodo;
    QPushButton * btnAbrirCarpeta;

private slots:
    void seleccionarTodoClicked();

    void casillaCambiada(QListWidgetItem * item);

    void enableApply();

    void btnAplicarClicked();

    void btnVolverClicked();

    void btnTerminarClicked();

    void btnAbrirCarpetaClicked();

signals:
    void selecccionarTodoRefresh();

    void closeMainWindow();

    void backToMainWindow();

    void mostrarTextoCuadro();

private:
    /* --- Métodos auxiliares --- */
    void crearUI();
    void conectarSeñales();

    void aplicarComprimir(QString rutaCarpeta);
    bool aplicarEliminar(const QString &rutaCarpeta, QMap<QString, bool> &carpetasProcesadasrutaCarpeta);
    bool aplicarMover(const QString &rutaCarpeta, QMap<QString, bool> &carpetasProcesadas);
    void actualizarLista(const QMap<QString, bool> &carpetasProcesadas, const QString &accion);
};

#endif // FINALWINDOW_H
