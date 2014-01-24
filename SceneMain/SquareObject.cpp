#include "SquareObject.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"

SquareObject::SquareObject() : pos(0.0f) {
    quad.mesh = Meshes.get("monkey");
    quad.program = Programs.get("deferredModel");
    renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
}

SquareObject::~SquareObject() {
}

void SquareObject::update(float deltaTime) {
    (void) deltaTime;
    transform = glm::translate(mat4f(1.0f),pos);
}

void SquareObject::draw() const {
    if(renderer->getMode() != DeferredContainer::Deferred) return;
    Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
    quad.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform);
    quad.program->uniform("M")->set(fullTransform);
    quad.program->uniform("V")->set(cam->view);
    quad.program->uniform("ambient")->set(0.5f);
    quad.program->uniform("specular")->set(1.0f);
    quad.program->uniform("diffuseTex")->set(Textures2D.get("nullRed"));
    quad.draw();
}
