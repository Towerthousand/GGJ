include(VBE/VBE.pro)
INCLUDEPATH += .
#DEFINES += __DLOG
SOURCES += main.cpp \
    commons.cpp \
    SceneMain/SceneMain.cpp \
    SceneMain/Camera.cpp \
    SceneMain/DeferredContainer.cpp \
	SceneMain/DeferredLight.cpp \
    SceneMain/BlurContainer.cpp \
    SceneMain/Frustum.cpp \
    SceneMain/SquareObject.cpp

HEADERS += \
    commons.hpp \
    SceneMain/SceneMain.hpp \
    SceneMain/Camera.hpp \
    SceneMain/DeferredContainer.hpp \
	SceneMain/DeferredLight.hpp \
    SceneMain/BlurContainer.hpp \
    SceneMain/Frustum.hpp \
    SceneMain/SquareObject.hpp

OTHER_FILES += \
    data/shaders/quad.vert \
    data/shaders/light.frag \
    data/shaders/blurPassVertical.frag \
    data/shaders/blurPassHoritzontal.frag \
    data/shaders/blurMaskPass.frag \
    data/shaders/quad.frag \
    data/shaders/depth.frag \
    data/shaders/depth.vert \
    data/shaders/standardDeferred.vert \
    data/shaders/standardDeferred.frag \
    data/shaders/ambientPass.frag

