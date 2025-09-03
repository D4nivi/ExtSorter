#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QProgressBar>
#include <QVBoxLayout>

class ExtensionManager
{
public:
    ExtensionManager();
    ~ExtensionManager();

    static const QString jsonFilePath;
    static const QString txtFilePath;
    static const int maxCategoryChars;
    static const int maxExtensionBoxChars;
    static const QMap<QString, QSet<QString>> defaultCategoriasYExtensiones;

    static bool isFolderNameValid(QString &nombreCarpeta);

private:
    QMap<QString, QSet<QString>> * categoriasYExtensiones;
    QSettings settings;

/* --- Funciones --- */
public:
    bool escribirExtensionesJSON();

    bool addCategoriasJSON(QString &nuevaCategoria, QStringList &nuevasExtensiones);

    void leerJSON();

    bool escribirTXT(bool force = false);

    bool addExtensionesTXT(QString extensionesEnTexto);

    bool restoreCategoriasYExtensiones();

    bool checkExtensionListFormat(QString &textoCaja, QLabel * mensajeError);



    /* GETTERS */
    QMap<QString, QSet<QString>> * getCategoriasYExtensiones();

    QStringList getCategorias();

    QSet<QString> getExtensionsFromTXT();

    bool wereFilesModified();

private:
    /* MÉTODOS AUXILIARES */
    void resetCategoriasYExtensiones();

};

#endif // EXTENSIONMANAGER_H
