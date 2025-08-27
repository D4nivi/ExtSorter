#ifndef ZIPWORKER_H
#define ZIPWORKER_H

#include <QProcess>
#include <QSet>

class ZipWorker : public QObject
{
    Q_OBJECT
public:
    ZipWorker(QString rutaCarpeta, QSet<QString> carpetasSeleccionadas);

public:
    QString rutaCarpeta;
    QSet<QString> carpetasSeleccionadas;

public slots:
    void run();

signals:
    void finished(int exitCode);
};

#endif // ZIPWORKER_H
