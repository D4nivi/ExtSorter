#include "Headers/mainwindow.h"

#include <QApplication>
#include "QProcess"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/icono-app.png"));

    /* Para manejar QSettings */
    QCoreApplication::setOrganizationName("Danivi");
    QCoreApplication::setApplicationName("ExtSorter");

    /* Ventana principal */
    MainWindow w;
    w.setWindowTitle("ExtSorter");
    w.setFixedSize(400, 400);

    w.show();
    return app.exec();
}
