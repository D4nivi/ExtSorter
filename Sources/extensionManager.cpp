#include "Headers/extensionManager.h"
#include "Headers/customMessageBox.h"

/**** Variables ****/
const QString ExtensionManager::jsonFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ExtSorter/extensiones.json";
const QString ExtensionManager::txtFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ExtSorter/extensiones.txt";
const int ExtensionManager::maxCategoryChars = 20;
const int ExtensionManager::maxExtensionBoxChars= 512;
const QMap<QString, QSet<QString>> ExtensionManager::defaultCategoriasYExtensiones = {
    {"Audio",      {"aac", "ac3", "aiff", "aif", "flac", "m4a", "mp3", "ogg", "pca", "w64", "wav", "wma"}},
    {"Código",     {"asm", "c", "cc", "cpp", "cs", "go", "h", "hpp", "java", "lua", "pl", "py", "rb", "rs", "swift", "ts"}},
    {"Comprimido", {"7z", "bz2", "gz", "rar", "tar", "zip"}},
    {"Documento",  {"accdb", "doc", "docm", "docx", "mpp", "odg", "odp", "ods", "odt", "pdf", "ppt", "pptx", "xls", "xlsm", "xlsx"}},
    {"Ejecutable", {"bat", "cmd", "exe", "msi"}},
    {"Imagen",     {"bmp", "dib", "gif", "jfif", "jpeg", "jpg", "pbm", "pgm", "png", "pnm", "ppm", "qtif", "svg", "tiff", "webp"}},
    {"Proyecto",   {"ai", "aep", "aup3", "kra", "prproj", "psd", "veg", "xcf"}},
    {"Texto",      {"csv", "log", "md", "txt"}},
    {"Video",      {"avi", "flv", "m4v", "mkv", "mov", "mp4", "mpg", "mxf", "webm", "wmv"}}
};

bool ExtensionManager::filesModified = false;

/* Función auxiliar para checkExtensionListFormat() */
int isAlphaNumOrComma(const QString &s)
{
    for (QChar c : s) {
        if (!(c.isDigit() || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ',')) {
            return false;
        }
    }
    return true;
}

ExtensionManager::ExtensionManager(QObject *parent)
    : QObject{parent}
{
    categoriasYExtensiones = new QMap<QString, QSet<QString>>;
}

/**** MÉTODOS JSON ****/
bool ExtensionManager::escribirExtensionesJSON()
{
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile file(jsonFilePath);
    QJsonObject rootObject;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo JSON para escribir:" << jsonFilePath;
        return false;
    }

    /* Convertir el QMap en QJsonObject */
    for (auto it = categoriasYExtensiones->constBegin(); it != categoriasYExtensiones->constEnd(); ++it) {
        QJsonArray arrExt;
        const QSet<QString> &extensiones = it.value();

        for (const QString &ext : extensiones) {
            arrExt.append(ext);
        }

        rootObject.insert(it.key(), arrExt);
    }

    /* Crear documento JSON */
    qDebug() << "Escribiendo fichero JSON...";
    QJsonDocument doc(rootObject);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    filesModified = true;
    return true;
}

bool ExtensionManager::addCategoriasJSON(QString &nuevaCategoria, QStringList &nuevasExtensiones)
{
    QFile file(jsonFilePath);

    /* Pre-Condiciones */
    if (!file.exists()) {
        qDebug() << "Archivo JSON no encontrado. Creando archivo JSON...";
        escribirExtensionesJSON();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No se pudo abrir el archivo JSON para leer:" << jsonFilePath;
        return false;
    }

    /* Leemos el JSON */
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    /* Añadimos la nueva categoria a un objeto JSON */
    QJsonArray nuevasExtensionesArray;
    for (const QString &ext : nuevasExtensiones) {
        nuevasExtensionesArray.append(ext);
    }

    QJsonObject jsonObject = jsonDoc.object();
    jsonObject.insert(nuevaCategoria, nuevasExtensionesArray);

    jsonDoc.setObject(jsonObject);

    /* Escribimos la nueva categoria */
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << QString("No se pudo abrir el archivo %1 para escribir las categorias.").arg(ExtensionManager::jsonFilePath);
        return false;
    } else {
        qDebug() << "Añadiendo Categorías al fichero JSON...";

        file.write(jsonDoc.toJson(QJsonDocument::Indented));
        file.close();
    }

    filesModified = true;
    return true;
}

void ExtensionManager::leerJSON()
{
    QFile file(jsonFilePath);

    /* Pre-Condiciones */
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No se pudo abrir el archivo JSON para leer. Usando Categorías y Extensiones por defecto...";
        resetCategoriasYExtensiones();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        qDebug() << "El JSON no es un objeto válido.";
        resetCategoriasYExtensiones();
        return;
    }

    /* Algoritmo */
    qDebug() << "El fichero JSON es válido. Comenzando lectura...";
    QJsonObject rootObject = doc.object();

    for (const QString &categoria : rootObject.keys()) {
        QSet<QString> setExtensiones;
        QJsonValue extensionesJSON = rootObject.value(categoria);

        qDebug() << "Categoria:" << categoria << Qt::endl << "Extensiones:" << extensionesJSON << Qt::endl;

        if (extensionesJSON.isArray()) {
            QJsonArray arr = extensionesJSON.toArray();

            for (const QJsonValue &extVal : std::as_const(arr)) {
                setExtensiones.insert(extVal.toString());
            }
        }
        categoriasYExtensiones->insert(categoria, setExtensiones);
    }

    qDebug() << "Fichero JSON leído correctamente.";
}


/**** MÉTODOS TXT ****/
bool ExtensionManager::escribirTXT(bool force)
{
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile file(txtFilePath);

    /* Solo se fuerza la escritura al eliminar una categoria */
    if (file.exists() && !force) {
        qDebug() << "El archivo \"extensiones.txt\" ya existe. No se escribirá nada.";
        return false;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se ha podido abrir el archivo para escribir:" << txtFilePath;
        return false;
    }

    qDebug() << "Escribiendo fichero \"extensiones.txt\"...";

    /* Algoritmo */
    QTextStream out(&file);
    QStringList lista;

    for (const QSet<QString> &set : std::as_const(*categoriasYExtensiones)) {
        for (const QString &extension : set) {
            lista.append(extension);
        }
    }

    out << lista.join(",");
    file.close();

    return true;
}

bool ExtensionManager::addExtensionesTXT(QString extensionesEnTexto)
{
    QFile txtExtensiones(txtFilePath);

    if (!txtExtensiones.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << QString("No se pudo abrir el archivo %1 para escribir.").arg(txtFilePath);
        return false;
    } else {
        qDebug() << "Actualizando fichero extensiones.txt...";
        QTextStream outfile(&txtExtensiones);
        outfile << "," << extensionesEnTexto;
        txtExtensiones.close();
    }

    return true;
}


/**** GETTERS ****/
QMap<QString, QSet<QString>> * ExtensionManager::getCategoriasYExtensiones()
{
    return categoriasYExtensiones;
}

QStringList ExtensionManager::getCategorias()
{
    QStringList categorias;

    for (auto it = categoriasYExtensiones->keyBegin(); it != categoriasYExtensiones->keyEnd(); it++) {
        categorias.append(*it);
    }

    return categorias;
}

QSet<QString> ExtensionManager::getExtensionsFromTXT()
{
    QFile extFile(txtFilePath);

    if (!extFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo" << extFile.fileName();
        return {};
    }

    QString contenido = extFile.readAll();
    QSet<QString> set;

    /* El .split() devuelve una lista, no un Set */
    for (const QString &ext : contenido.split(",")) {
        set.insert(ext);
    }

    return set;
}


/**** MÉTODOS AUXILIARES ****/
void ExtensionManager::resetCategoriasYExtensiones()
{
    *categoriasYExtensiones = defaultCategoriasYExtensiones;
    filesModified = true;
}

bool ExtensionManager::restoreCategoriasYExtensiones()
{
    QString msg = "¿Estás seguro de que deseas restablecer las extensiones a sus valores predeterminados? Esto eliminará cualquier categoría/extensión que hayas creado/añadido.";
    bool success = false;

    if (CustomMessageBox::mostrarConfirmacion(nullptr, "Confirmar Restablecer", msg, IconManager::IconType::Warning)) {
        qDebug() << "Restableciendo extensiones...";

        /* Actualizar Atributos y Ficheros */
        resetCategoriasYExtensiones();
        if (escribirExtensionesJSON() && escribirTXT(true)) {
            success = true;
        }

        if (success) {
            CustomMessageBox::info(nullptr, "Extensiones Restablecidas", "Extensiones restablecidas con éxito.");
            qDebug() << "Extensiones restablecidas.";
        } else {
            QMessageBox::critical(nullptr, "Error al restablecer extensiones", "Ha ocurrido un error al intentar restablecer las extensiones. Se recomienda reiniciar la aplicación.");
        }
    }

    return success;
}

bool ExtensionManager::checkExtensionListFormat(QString &textoCaja, QLabel * mensajeError)
{
    bool hayTexto = !textoCaja.isEmpty();
    bool formatoCorrecto = false;
    bool caracteresCorrectos = false;
    bool extensionesRepetidas = false;

    if (hayTexto) {
        QStringList extensiones = textoCaja.split(",", Qt::KeepEmptyParts);
        caracteresCorrectos = isAlphaNumOrComma(textoCaja);
        formatoCorrecto = !extensiones.contains("");

        /* Cambiar mensaje de error */
        if (!caracteresCorrectos) {
            mensajeError->setText("<span style='color: red;'><b>Formato incorrecto</b></span>: solo caracteres alfanuméricos.");

        } else if (!formatoCorrecto) {
            mensajeError->setText("<span style='color: red;'><b>Formato incorrecto</b></span>: las extensiones deben ir separadas con comas (,).");

        } else {
            QSet<QString> extensionesFichero = getExtensionsFromTXT();
            QString extensionRepetida;
            for (int i = 0; i < extensiones.count(); i++) {
                if (extensionesFichero.contains(extensiones[i].toLower())) {
                    extensionRepetida = extensiones[i];
                    extensionesRepetidas = true;
                    mensajeError->setText(QString("<span style='color: red;'><b>Extensión repetida</b></span>: la extensión <b>\".%1\"</b> ya está en una categoría.").arg(extensionRepetida.toLower()));
                    break;
                }
            }
        }
    } else mensajeError->setText("");

    return hayTexto && formatoCorrecto && caracteresCorrectos && !extensionesRepetidas;
}
