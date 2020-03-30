#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T00:01:41
#
#-------------------------------------------------

QT       += core gui concurrent
CONFIG += c++11 sse4.1
win32 {
    # We want fewer dll files
    CONFIG += static
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -msse4.1 -mssse3 -msse3 -msse2 -msse

!win32-msvc2013 {
    # GCC specific option. Results in an error with MSVC.
    # FIXME: !gcc { ... }
    QMAKE_CXXFLAGS += -Wno-missing-braces
}

TARGET = Terravox
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    brusheditor.cpp \
    terrainview.cpp \
    terrain.cpp \
    session.cpp \
    terrainedit.cpp \
    terraingenerator.cpp \
    labelslider.cpp \
    toolcontroller.cpp \
    brushtool.cpp \
    brushtoolcontroller.cpp \
    brushtoolview.cpp \
    terrainviewoptionswindow.cpp \
    vxl.cpp \
    vxh.cpp \
    dithersampler.cpp \
    temporarybuffer.cpp \
    emptytooleditor.cpp \
    coherentnoisegenerator.cpp \
    colorpicker.cpp \
    colorview.cpp \
    colorpickerwindow.cpp \
    colormodelview.cpp \
    effectcontroller.cpp \
    effecteditor.cpp \
    erosioneffect.cpp \
    erosioneffectcontroller.cpp \
    erosioneditor.cpp \
    terrainview_p.cpp \
    terrainview_p_context.cpp \
    terrainview_p_ao.cpp \
    colorsamplerview.cpp \
    manipulatetoolcontroller.cpp \
    manipulatetoolview.cpp \
    luaengine.cpp \
    luaengine_p.cpp \
    luaapi.cpp \
    cpu.cpp

HEADERS  += mainwindow.h \
    brusheditor.h \
    terrainview.h \
    terrain.h \
    session.h \
    terrainedit.h \
    terraingenerator.h \
    labelslider.h \
    toolcontroller.h \
    brushtool.h \
    brushtoolcontroller.h \
    brushtoolview.h \
    terrainviewoptionswindow.h \
    vxl.h \
    vxh.h \
    dithersampler.h \
    temporarybuffer.h \
    emptytooleditor.h \
    coherentnoisegenerator.h \
    colorpicker.h \
    colorview.h \
    colorpickerwindow.h \
    colormodelview.h \
    effectcontroller.h \
    effecteditor.h \
    erosioneffect.h \
    erosioneffectcontroller.h \
    erosioneditor.h \
    terrainview_p.h \
    colorsamplerview.h \
    manipulatetoolcontroller.h \
    manipulatetoolview.h \
    luaengine.h \
    luaengine_p.h \
    lua/api.h \
    luaapi.h \
    cpu.h

FORMS    += mainwindow.ui \
    brusheditor.ui \
    terrainviewoptionswindow.ui \
    emptytooleditor.ui \
    colorpicker.ui \
    colorpickerwindow.ui \
    effecteditor.ui \
    erosioneditor.ui

RESOURCES += \
    res/resources.qrc \
    lua/builtin.qrc

TRANSLATIONS = terravox_en.ts
TRANSLATIONS += terravox_ja.ts

# Link LuaJIT
CONFIG += link_pkgconfig
QT_CONFIG -= no-pkg-config

!isEmpty(LUAJIT_CFLAGS) {
    message(LuaJIT specified by parameter.)
    DEFINES += HAS_LUAJIT
    QMAKE_CXXFLAGS += $$LUAJIT_CFLAGS
    QMAKE_CFLAGS += $$LUAJIT_CFLAGS
    QMAKE_LFLAGS += $$LUAJIT_LFLAGS
} else: packagesExist(luajit) | system(pkg-config luajit) {
    message(LuaJIT found by pkg-config)
    PKGCONFIG += luajit
    DEFINES += HAS_LUAJIT
    mac { # LuaJIT has issues with x86_64 OS X
        QMAKE_LFLAGS += -pagezero_size 10000 -image_base 100000000
    }
} else {
    warning(Project will be built without LuaJIT support.)
}
# TODO: support LuaJIT on Windows

# Add example scripts
mac {
    ScriptFiles.files = scripts
    ScriptFiles.path = Contents/Resources
    QMAKE_BUNDLE_DATA += ScriptFiles
}
# TODO: install example scripts on Windows/Linux
