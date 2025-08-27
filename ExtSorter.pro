QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/categoriasAdd.cpp \
    Sources/categoriasDelete.cpp \
    Sources/customMessageBox.cpp \
    Sources/extensionManager.cpp \
    Sources/extensiones.cpp \
    Sources/finalwindow.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/zipworker.cpp

HEADERS += \
    Headers/categoriasAdd.h \
    Headers/categoriasDelete.h \
    Headers/customMessageBox.h \
    Headers/extensionManager.h \
    Headers/extensiones.h \
    Headers/finalwindow.h \
    Headers/iconManager.h \
    Headers/mainwindow.h \
    Headers/zipworker.h

FORMS += \
    Forms/categoriasAdd.ui \
    Forms/categoriasDelete.ui \
    Forms/extensiones.ui \
    Forms/finalwindow.ui \
    Forms/mainwindow.ui

RC_ICONS = icono-app.ico

RESOURCES = resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
