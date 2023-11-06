lessThan(QT_MAJOR_VERSION, 6): QT += script
greaterThan(QT_MAJOR_VERSION, 5): QT += qml

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xdebugscript.h \
    $$PWD/xdebugscriptengine.h \
    $$PWD/xdebugscripthelper.h

SOURCES += \
    $$PWD/xdebugscript.cpp \
    $$PWD/xdebugscriptengine.cpp \
    $$PWD/xdebugscripthelper.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md
