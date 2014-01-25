include(VBE/VBE.pro)
INCLUDEPATH += .
LIBS += -lsfml-network


#DEFINES += __DLOG
SOURCES += main.cpp \
    commons.cpp \
    SceneMain/SceneMain.cpp \
    SceneMain/Camera.cpp \
    SceneMain/Player.cpp \
    SceneMain/DeferredContainer.cpp \
	SceneMain/DeferredLight.cpp \
    SceneMain/BlurContainer.cpp \
    SceneMain/Frustum.cpp \
    SceneMain/SquareObject.cpp \
    SceneMain/Map.cpp \
    SceneMain/particles/ParticleSystem.cpp \
    SceneMain/particles/ParticleEmitter.cpp \
    SceneMain/particles/Particle.cpp \
    SceneMain/particles/LightParticleEmitter.cpp \
    SceneMain/MotionBlurContainer.cpp \
    SceneMain/Trails.cpp

HEADERS += \
    commons.hpp \
    SceneMain/SceneMain.hpp \
    SceneMain/Camera.hpp \
    SceneMain/Player.hpp \
    SceneMain/DeferredContainer.hpp \
	SceneMain/DeferredLight.hpp \
    SceneMain/BlurContainer.hpp \
    SceneMain/Frustum.hpp \
    SceneMain/SquareObject.hpp \
    SceneMain/Map.hpp \
    SceneMain/particles/ParticleSystem.hpp \
    SceneMain/particles/ParticleEmitter.hpp \
    SceneMain/particles/Particle.hpp \
    SceneMain/particles/LightParticleEmitter.hpp \
    SceneMain/MotionBlurContainer.hpp \
    SceneMain/Colors.hpp \
    SceneMain/Trails.hpp

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
    data/shaders/ambientPass.frag \
    data/shaders/lines.frag \
    data/shaders/particles.vert \
    data/shaders/particles.geom \
    data/shaders/particles.frag \
    data/shaders/motionBlurPass.frag \
    data/shaders/quadlol.vert \
    data/shaders/trails.vert \
    data/shaders/trails.frag \
    data/shaders/trails.geom

