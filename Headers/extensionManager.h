#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <QProgressBar>
#include <QVBoxLayout>

class ExtensionManager : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionManager(QObject *parent = nullptr);

    static const QString jsonFilePath;
    static const QString txtFilePath;
    static const int maxCategoryChars;
    static const int maxExtensionBoxChars;
    static const QMap<QString, QSet<QString>> defaultCategoriasYExtensiones;
    static bool filesModified;

private:
    QMap<QString, QSet<QString>> * categoriasYExtensiones;

/* --- Funciones --- */
public:
    bool escribirExtensionesJSON();

    bool addCategoriasJSON(QString &nuevaCategoria, QStringList &nuevasExtensiones);

    void leerJSON();

    bool escribirTXT(bool force = false);

    bool addExtensionesTXT(QString extensionesEnTexto);

    QMap<QString, QSet<QString>> * getCategoriasYExtensiones();

    QStringList getCategorias();

    QSet<QString> getExtensionsFromTXT();

    void resetCategoriasYExtensiones();

    bool restoreCategoriasYExtensiones();

    bool checkExtensionListFormat(QString &textoCaja, QLabel * mensajeError);

signals:
};

#endif // EXTENSIONMANAGER_H
