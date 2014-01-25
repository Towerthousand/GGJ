#include "Player.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"



Player::Player(const std::string& playerName, const vec3f& pos, const vec3f& rot)
    : pos(pos), rot(rot) {
    this->setName(playerName);
    model.mesh = Meshes.get("monkey");
    model.program = Programs.get("deferredModel");
    renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");

    cam = new Camera("playerCam");
    cam->projection = glm::perspective(FOV, float(SCRWIDTH)/float(SCRHEIGHT), ZNEAR, ZFAR);
    cam->pos = vec3f(0,0,10);
    cam->addTo(this);

    velocity = vec3f(0,0,0);
    maxvelocity = .1f;
    gravity = 1.0f;
    colliding = true;

}

Player::~Player() {
}

void Player::update(float deltaTime) {

    float speed = 1.0f;

    //MOVEMENT
    if(Input::isKeyDown(sf::Keyboard::Left)) {
        velocity.x -= speed*deltaTime;
        if(velocity.x < -maxvelocity) velocity.x = -maxvelocity;
    } else if(Input::isKeyDown(sf::Keyboard::Right)) {
        velocity.x += speed*deltaTime;
        if(velocity.x > maxvelocity) velocity.x = maxvelocity;
    } else velocity.x = 0;

    if(Input::isKeyDown(sf::Keyboard::X)) {
        colliding = false;
    } else colliding = true;

    //JUMP
    if(Input::isKeyPressed(sf::Keyboard::Up)) velocity.y = .5f;



    pos += velocity;

    //GRAVITY
    if(!colliding) {
        velocity.y -= gravity*deltaTime;
    } else velocity.y = 0;


    //transform stuff
    for(int i = 0; i < 3; ++i) {
        if(rot[i] < 0) rot[i] = rot[i]+360;
        else if(rot[i] >= 360.0f) rot[i] = rot[i]-360;
    }
    transform = glm::translate(mat4f(1.0f),pos); //Esto hace que te vayas a tu posicion

}

void Player::draw() const
{
    if(renderer->getMode() != DeferredContainer::Deferred) return;
    Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
    model.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform);
    model.program->uniform("M")->set(fullTransform);
    model.program->uniform("V")->set(cam->view);
    model.program->uniform("ambient")->set(0.5f);
    model.program->uniform("specular")->set(1.0f);
    model.program->uniform("diffuseTex")->set(Textures2D.get("nullRed"));
    model.draw();


}

