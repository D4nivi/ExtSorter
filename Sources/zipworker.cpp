#include "Headers/zipworker.h"

int ZipWorker::zipFileNum = 1;

ZipWorker::ZipWorker(QString rutaCarpeta, QSet<QString> carpetasSeleccionadas)
    : QObject{}, rutaCarpeta(rutaCarpeta), carpetasSeleccionadas(carpetasSeleccionadas)
{}

void ZipWorker::run()
{
    QString archivoZip = rutaCarpeta + "/Archive.zip";
    QStringList rutas;

    if (QFile().exists(archivoZip)) {
        archivoZip = QString(rutaCarpeta + "/Archive%1.zip").arg(++zipFileNum);
    }

    for (const QString &carpeta : std::as_const(carpetasSeleccionadas)) {
        rutas << QString("\"%1/%2\"").arg(rutaCarpeta, carpeta);
    }

    QStringList psArgs;
    psArgs << "-Command" << QString("Compress-Archive -Path %1 -DestinationPath \"%2\" -Force")
                                   .arg(rutas.join(","), archivoZip);

    QProcess proc;
    proc.start("powershell", psArgs);
    proc.waitForFinished(-1);

    emit finished(proc.exitCode());
}
