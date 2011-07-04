# -------------------------------------------------
# Project created by QtCreator 2009-12-29T18:41:32
# -------------------------------------------------
TARGET = EpilepsyEditor
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    Graphics.cpp \
    qdirectxwidget.cpp \
    eptoolbar.cpp \
    FreeCamera.cpp \
    iobject.cpp \
    qmylist.cpp \
    epbox.cpp \
    epsphere.cpp \
    mytreeitem.cpp \
    editor/qmysidebar.cpp \
    editor/qmylist.cpp \
    editor/mytreeitem.cpp \
    editor/mainwindow.cpp \
    editor/qmysidebar.cpp \
    editor/qmylist.cpp \
    editor/qdirectxwidget.cpp \
    editor/mytreeitem.cpp \
    editor/mylistwidget.cpp \
    editor/mainwindow.cpp \
    editor/eptoolbar.cpp \
    render/iobject.cpp \
    render/icamera.cpp \
    render/Graphics.cpp \
    render/FreeCamera.cpp \
    render/epsphere.cpp \
    render/epbox.cpp \
    physEngine/momentsqueue.cpp \
    physEngine/MomentoQueue.cpp \
    physEngine/EpilepsyObject.cpp \
    physEngine/EpilepsyRectangle.cpp \
    physEngine/EpilepsySphere.cpp \
    physEngine/EpilepsyController.cpp
HEADERS += \
    d3dx9.h \
    Graphics.h \
    qdirectxwidget.h \
    eptoolbar.h \
    FreeCamera.h \
    ICamera.h \
    iobject.h \
    qmylist.h \
    epsphere.h \
    EpilepsiyaObject.h \
    EpController.h \
    mytreeitem.h \
    editor/qmysidebar.h \
    editor/qmylist.h \
    editor/mytreeitem.h \
    editor/mainwindow.h \
    editor/qmysidebar.h \
    editor/qmylist.h \
    editor/qdirectxwidget.h \
    editor/mytreeitem.h \
    editor/mainwindow.h \
    editor/eptoolbar.h \
    render/irenderable.h \
    render/iobject.h \
    render/ICamera.h \
    render/Graphics.h \
    render/FreeCamera.h \
    render/epsphere.h \
    render/epbox.h \
    physEngine/MomentoQueue.h \
    physEngine/EpilepsyObject.h \
    physEngine/EpilepsyRectangle.h \
    physEngine/EpilepsySphere.h \
    physEngine/EpilepsyController.h
FORMS += mainwindow.ui \
    editor/mainwindow.ui \
    editor/mainwindow.ui
OTHER_FILES += 
CONFIG += qt \
    thread \
    warn_on \
    debug

RESOURCES += \
    resources.qrc
