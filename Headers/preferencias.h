#ifndef PREFERENCIAS_H
#define PREFERENCIAS_H

#include <QDialog>
#include <QLabel>
#include <QStandardPaths>
#include <QSettings>

/* Enumerado para las opciones de las casillas de la Ventana Principal */
enum class ValorOpcion { MANUAL, SIEMPRE, NUNCA };

/* Struct para guardar los nombres internos de las preferencias */
struct PrefsNames {
    inline static const QString filesModified                = "filesModified";
    inline static const QString settingsModified             = "settingsModified";
    inline static const QString nombreCarpetaDestino         = "nombreCarpetaDestino";
    inline static const QString showWarnings                 = "showWarnings";
    inline static const QString minFicherosConfirmacion      = "minFicherosConfirmacion";
    inline static const QString defaultBorrarCarpetasVacias  = "defaultBorrarCarpetasVacias";
    inline static const QString defaultBorrarAccesosDirectos = "defaultBorrarAccesosDirectos";
    inline static const QString defaultExcluirOtros          = "defaultExcluirOtros";
};

namespace Ui {
class Preferencias;
}

class Preferencias : public QDialog
{
    Q_OBJECT

public:
    explicit Preferencias(QWidget *parent = nullptr);
    ~Preferencias();

private:
    Ui::Preferencias *ui;
    QSettings settings;

private slots:
    void enableSave();

    void btnGuardarClicked();

    void btnAceptarClicked();

    void btnCancelarClicked();

    void btnRestablecerClicked();

signals:
    void opcionesChanged();

private:
    bool saveSettings();

private:
    /*--- Métodos Auxiliares ---*/
    void changeDefaultWidgetsValues();
    void crearUI();
    void conectarSeñales();
};

#endif // PREFERENCIAS_H
