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
    maxvelocity = 10.0f;
    gravity = 9.8f;
    colliding = true;

}

Player::~Player() {
}

void Player::update(float deltaTime) {

    float accel = 40.0f;
    float kfriction = 20.0f;

    //MOVEMENT
    vec3f dir(0);
    if(Input::isKeyDown(sf::Keyboard::Left)) {
        dir += vec3f(-1, 0, 0);
    }
    if(Input::isKeyDown(sf::Keyboard::Right)) {
        dir += vec3f(1, 0, 0);
    }

    vec3f friction(0);
    friction = kfriction*vec3f(velocity.x > 0 ? -1.0 : 1.0, 0, 0);


    //GRAVITY AND COLLISIONS
    vec3f contactForce(0);
    if(colliding) {
        contactForce = vec3f(0, gravity, 0);
        velocity.y = 0;
        //pos.y = //posicion de la colision;
    }

    vec3f totalForce = accel*dir + vec3f(0, -gravity, 0) + contactForce + friction;


    velocity += totalForce*deltaTime;
    glm::clamp(velocity, vec3f(-maxvelocity), vec3f(maxvelocity));

    if(Input::isKeyDown(sf::Keyboard::X)) {
        colliding = false;
    } else colliding = true;



    //JUMP
    if(Input::isKeyPressed(sf::Keyboard::Up))
        velocity.y = 5.0f;


    pos += velocity*deltaTime;





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

