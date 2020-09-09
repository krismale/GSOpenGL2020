QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    Colliders/collider.cpp \
    Colliders/cubecollider.cpp \
    Colliders/spherecollider.cpp \
    Dependencies/stb_image.cpp \
    MyMath/matrix4x4.cpp \
    MyMath/misccalculations.cpp \
    beziercurve.cpp \
    billboard.cpp \
    camera.cpp \
    gridsystem.cpp \
    interactiveobject.cpp \
    light.cpp \
    meshImporter.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    scenemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    skybox.cpp \
    terrain.cpp \
    trianglesurface.cpp \
    ui.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    Colliders/collider.h \
    Colliders/cubecollider.h \
    Colliders/spherecollider.h \
    Dependencies/stb_image.h \
    MyMath/mathconstants.h \
    MyMath/matrix4x4.h \
    MyMath/misccalculations.h \
    beziercurve.h \
    billboard.h \
    camera.h \
    gridsystem.h \
    interactiveobject.h \
    light.h \
    meshImporter.h \
    mystructs.h \
    octahedronball.h \
    renderwindow.h \
    scenemanager.h \
    shader.h \
    mainwindow.h \
    skybox.h \
    terrain.h \
    trianglesurface.h \
    ui.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    billboardfragment.frag \
    billboardvertex.vert \
    phongfragment.frag \
    phongvertex.vert \
    plainfragment.frag \
    plaintexfragment.frag \
    plainvertex.vert \
    skyboxfragment.frag \
    skyboxvertex.vert

QMAKE_CXXFLAGS_WARN_OFF = -Wno-unused-parameter
