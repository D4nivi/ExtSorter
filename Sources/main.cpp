#include "Headers/mainwindow.h"

#include <QApplication>
#include "QProcess"

int main(int argc, char *argv[])
{
    // TODO: Quitar cuando publique el programa
    QProcess proceso;
    proceso.start("python", {"C:\\Users\\danie\\Documents\\PROGRAMACION\\Qt\\ExtSorter\\Pruebas\\Generador.py"});
    proceso.waitForFinished();

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/icono-app.png"));

    MainWindow w;

    /* Propiedades de la ventana principal */
    w.setWindowTitle("ExtSorter");
    w.setFixedSize(400, 400);

    w.show();
    return app.exec();
}
