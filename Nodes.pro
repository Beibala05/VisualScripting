QT += core gui widgets

SOURCES += \
    src/main.cpp \
    src/main_window.cpp \
    src/node.cpp \
    src/viewport.cpp \
    src/functional.cpp \

HEADERS += \
    src/main_window.h \
    src/node.h \
    src/viewport.h \
    src/functional.h \
    src/style.h \
    src/cross.h \

RESOURCES += \
    resources.qrc \

win32:RC_FILE = logo.rc