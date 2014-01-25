#include "Player.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "Map.hpp"

#define MAX_VELOCITY    10.0f
#define GRAVITY         9.8f
#define ACCELERATION    40.0f
#define FRICTION_COEFF  20.0f
#define JUMP_IMPULSE    5.0f
#define ELASTICITY      0.0f


Player::Player(const std::string& playerName, const vec3f& pos, const vec3f& rot)
    : pos(pos), rot(rot) {
    this->setName(playerName);
    model.mesh = Meshes.get("monkey");
    model.program = Programs.get("deferredModel");
    renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");

    cam = new Camera("playerCam");
    cam->projection = glm::perspective(FOV, float(SCRWIDTH)/float(SCRHEIGHT), ZNEAR, ZFAR);
    cam->pos = vec3f(0,0,20*model.mesh->getBoundingBox().getRadius());
    cam->addTo(this);

    velocity = vec3f(0,0,0);
    colliding = false;

    scale = vec3f(0.5f/model.mesh->getBoundingBox().getRadius());

}

Player::~Player() {
}

void Player::update(float deltaTime) {

    vec3f prevPos = pos;

    // apply forces
    vec3f dir(0);
    if(Input::isKeyDown(sf::Keyboard::Left)) {
        dir += vec3f(-1, 0, 0);
    }
    if(Input::isKeyDown(sf::Keyboard::Right)) {
        dir += vec3f(1, 0, 0);
    }

    vec3f friction(0);
    friction = FRICTION_COEFF*vec3f(velocity.x > 0 ? -1.0 : 1.0, 0, 0);

    vec3f contactForce(0);
    if (colliding) {
        contactForce = vec3f(0, GRAVITY, 0);
    }

    vec3f totalForce = ACCELERATION*dir + vec3f(0, -GRAVITY, 0) + contactForce + friction;

    // apply impulses
    if (Input::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y += JUMP_IMPULSE;
    }

    // integration
    velocity = glm::clamp(velocity + totalForce*deltaTime, vec3f(-MAX_VELOCITY), vec3f(MAX_VELOCITY));
    pos += velocity*deltaTime;

    //transform stuff
    for(int i = 0; i < 3; ++i) {
        if(rot[i] < 0) rot[i] = rot[i]+360;
        else if(rot[i] >= 360.0f) rot[i] = rot[i]-360;
    }
    transform = glm::scale(mat4f(1), scale);
    transform = glm::translate(transform,pos);


    // collision detection
    AABB aabb = model.mesh->getBoundingBox();
    AABB mybox(vec3f(fullTransform*vec4f(aabb.getMin(), 1.0f)), vec3f(fullTransform*vec4f(aabb.getMax(), 1.0f)));
    vec3f collpos, collnor;
    float fraction = 1.0f;
    colliding = ((Map*)getGame()->getObjectByName("map"))->checkCollisions(mybox, collpos, collnor);
    if (colliding) {
        vec3f velNor = glm::dot(velocity, collnor)*collnor;
        vec3f velTan = velocity - velNor;
        velocity = velTan - ELASTICITY*velNor;
        pos = pos - (1 + ELASTICITY)*(glm::dot(collnor, pos) - glm::dot(collnor, collpos))*collnor + model.mesh->getBoundingBox().getRadius()*collnor;
    }
    /*while (((Map*)getGame()->getObjectByName("map"))->checkCollisions(mybox, collpos, collnor)) {
        fraction *= 0.5f;
        pos = prevPos + fraction*velocity*deltaTime;
        transform = glm::scale(mat4f(1), scale);
        transform = glm::translate(transform,pos);
        mybox = AABB(vec3f(fullTransform*vec4f(aabb.getMin(), 1.0f)), vec3f(fullTransform*vec4f(aabb.getMax(), 1.0f)));
    }*/



}

void Player::draw() const
{
    if(renderer->getMode() == DeferredContainer::Deferred) {
        Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
        model.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform);
        model.program->uniform("M")->set(fullTransform);
        model.program->uniform("V")->set(cam->view);
        model.program->uniform("ambient")->set(0.5f);
        model.program->uniform("specular")->set(1.0f);
        model.program->uniform("diffuseTex")->set(Textures2D.get("nullRed"));
        model.draw();
    }
    else if (renderer->getMode() == DeferredContainer::Forward) {
        Model m;
        m.mesh = Meshes.get("1x1WireCube");
        m.program = Programs.get("lines");
        m.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform);
        m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
        m.draw();
    }

}

