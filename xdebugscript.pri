QT       += script

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
