#ifndef PREFERENCIAS_H
#define PREFERENCIAS_H

#include <QDialog>
#include <QLabel>
#include <QStandardPaths>
#include <QSettings>

namespace Ui {
class Preferencias;
}

enum class ValorOpcion { MANUAL, SIEMPRE, NUNCA };

class Preferencias : public QDialog
{
    Q_OBJECT

public:
    explicit Preferencias(QSettings * settings, QWidget *parent = nullptr);
    ~Preferencias();

private:
    Ui::Preferencias *ui;
    QSettings * settings;

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
