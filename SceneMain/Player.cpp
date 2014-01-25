#include "Player.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "Map.hpp"
#include "Trails.hpp"
#include "particles/LightParticleEmitter.hpp"

#define MAX_VELOCITY    10.0f
#define GRAVITY         9.8f
#define ACCELERATION    40.0f
#define FRICTION_COEFF  20.0f
#define JUMP_IMPULSE    7.0f
#define ELASTICITY      0.0f


Player::Player(const std::string& playerName, const vec3f& pos, const vec3f& rot, Color col)
	: pos(pos), rot(rot), color(col) {
    this->setName(playerName);
    model.mesh = Meshes.get("brushidle0");
    model.program = Programs.get("deferredModel");
    renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");

    modelAabb = model.mesh->getBoundingBox();
    modelOffset = glm::translate(mat4f(1.0f), -modelAabb.getCenter()+vec3f(0,0,0.5));

    velocity = vec3f(0.0f);
    colliding = false;

    totalForce = vec3f(0.0f);
    animState = Player::IDLE;

    animCount = 0.0f;
    animTime = randomFloat(0.1f, 0.4f);

    anim = "idle";
    animIter = 0;

	scale = vec3f(0.30f/modelAabb.getRadius());

	ParticleEmitter* emitter = new LightParticleEmitter(vec3f(1),color-1);
	emitter->addTo(this);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
    vec3f initPos = pos;

    //Animation Conditions
    bool collidingSides = false;
    bool collidingFloor = false;
    bool running = false;

    //PHYSICS
    // apply forces
    vec3f dir(0);
    if(Input::isKeyDown(sf::Keyboard::Left)) {
        dir += vec3f(-1, 0, 0);
        running = true;
    }
    if(Input::isKeyDown(sf::Keyboard::Right)) {
        dir += vec3f(1, 0, 0);
        running = true;
    }

    vec3f friction(0);
    if(!(fabs(velocity.x) < 0.08f)) friction = FRICTION_COEFF*vec3f(velocity.x > 0 ? -1.0 : 1.0, 0, 0);


    totalForce += ACCELERATION*dir + vec3f(0, -GRAVITY, 0) + friction;

    // apply impulses
    if (animState != Player::JUMP && Input::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y += JUMP_IMPULSE;
    }
    // integration
    velocity = glm::clamp(velocity + totalForce*deltaTime, vec3f(-MAX_VELOCITY), vec3f(MAX_VELOCITY));

    // apply impulses
    if (animState != Player::JUMP && Input::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y += JUMP_IMPULSE;
    }
    // integration
    velocity = glm::clamp(velocity + totalForce*deltaTime, vec3f(-MAX_VELOCITY), vec3f(MAX_VELOCITY));
    if (fabs(velocity.x) < 0.08f) velocity.x = 0;


    //Reset totalForce;
    totalForce = vec3f(0.0f);



    vec3f disp = velocity*deltaTime;


    //NOT PHYSICS

    // collision detection
    Map* map = (Map*)getGame()->getObjectByName("map");
    AABB aabb = modelAabb;
    mat4f trans = fullTransform*modelOffset;
    aabb = AABB(vec3f(trans*vec4f(aabb.getMin(), 1.0f)), vec3f(trans*vec4f(aabb.getMax(), 1.0f)));

    vec3f bbmin = vec3f(trans*vec4f(modelAabb.getMin(), 1.0f));
    vec3f bbmax = vec3f(trans*vec4f(modelAabb.getMin() + modelAabb.getDimensions()*vec3f(1.0f, 0.05f, 1.0f), 1.0f));
    AABB brushBox(bbmin + disp, bbmax + disp);

    colliding = false;
    bool isBrushColliding = false;

    //Y
    AABB newboxY(aabb.getMin()+vec3f(0,disp.y,0), aabb.getMax()+vec3f(0,disp.y,0));
    Color blockColor;
    if(map->isColliding(newboxY, blockColor)) {
        float min = 0;
        float max = 1;
        Color foo;
        while(max-min > 0.001) { //search for the maximum distance you can move
            float m = (max+min)/2;
            newboxY = AABB(aabb.getMin()+vec3f(0,disp.y*m,0), aabb.getMax()+vec3f(0,disp.y*m,0));
            if(map->isColliding(newboxY, foo))
                max = m;
            else
                min = m;
        }
        if(velocity.y < 0) collidingFloor = true;
        velocity.y = 0;
        disp.y *= min;
        colliding = true;
        isBrushColliding = true;
    }

    pos.y += disp.y;
    aabb = AABB(aabb.getMin()+vec3f(0,disp.y,0), aabb.getMax()+vec3f(0,disp.y,0));

    //X
    bool isRightWall = false;
    AABB newboxX(aabb.getMin()+vec3f(disp.x,0,0), aabb.getMax()+vec3f(disp.x,0,0));
    if(map->isColliding(newboxX, blockColor)) {

        Color foo;
        isBrushColliding = map->isColliding(brushBox, foo);

        float min = 0;
        float max = 1;
        while(max-min > 0.001) { //search for the maximum distance you can move
            float m = (max+min)/2;
            newboxX = AABB(aabb.getMin()+vec3f(disp.x*m,0,0), aabb.getMax()+vec3f(disp.x*m,0,0));
            if(map->isColliding(newboxX, foo))
                max = m;
            else
                min = m;
        }
        //WALL JUMP
        vec3f wallFriction(0);
        if(velocity.x != 0) {
            wallFriction = FRICTION_COEFF*vec3f(0, velocity.y > 0 ? -.1 : .4, 0);
            collidingSides = true;
        }
        totalForce += wallFriction;

        if(velocity.x > 0 ) {
            if (Input::isKeyPressed(sf::Keyboard::Up)) velocity.x -= JUMP_IMPULSE*5;
            isRightWall = true;
        } else if(velocity.x < 0 ) {
            if (Input::isKeyPressed(sf::Keyboard::Up)) velocity.x += JUMP_IMPULSE*5;
        } else velocity.x = 0;

        disp.x *= min;
        colliding = true;
    }

    pos.x += disp.x;

    //ANIMATION
    if(collidingSides)  {
        animState = Player::WALL;
        anim = velocity.y > 0 ? "wallu" : "walld";
    } else if(collidingFloor) {
        if(running)      {
            animState = Player::RUN;
            anim = fabs(velocity.x) >= MAX_VELOCITY/2 ? "runb" : "runa";
        } else {
            animState = Player::IDLE;
            anim = "idle";

        }
    } else {
        animState = Player::JUMP;
        anim = fabs(velocity.x) > MAX_VELOCITY/2 ? "jumpb" : "jumpa";
    }

    animCount += deltaTime;
    if(animCount >= animTime) {
        animCount -= animTime;
        animTime = randomFloat(0.1f, 0.2f);
        VBE_LOG(animTime);
        animIter = 1 - animIter;
    }
    std::string s = "brush" + anim + toString(animIter);
    //VBE_LOG(s);
    model.mesh = Meshes.get(s);



    // TRAILS

	vec3f posoff(0, -0.29f*modelAabb.getDimensions().y, 0.5);
    if (prevOnfloor && collidingFloor && (blockColor == Color::WHITE || blockColor == color) ) {
        Trails* trails = (Trails*)getGame()->getObjectByName("trails");
        if (initPos.x < pos.x)
            trails->addTrailSegment(color, Trails::HORIZONTAL, Trails::Segment(initPos + posoff, pos + posoff));
        else
            trails->addTrailSegment(color, Trails::HORIZONTAL, Trails::Segment(pos + posoff, initPos + posoff));
    }
    if (prevOnside && collidingSides && isBrushColliding && (blockColor == Color::WHITE || blockColor == color) ) {
        Trails* trails = (Trails*)getGame()->getObjectByName("trails");
        Trails::Direction dir;
        if (isRightWall)
            dir = Trails::VERTICAL_RIGHT;
        else
            dir = Trails::VERTICAL_LEFT;
        if (initPos.y < pos.y)
            trails->addTrailSegment(color, dir, Trails::Segment(initPos + posoff, pos + posoff));
        else
            trails->addTrailSegment(color, dir, Trails::Segment(pos + posoff, initPos + posoff));
    }
    prevOnfloor = collidingFloor;
    prevOnside = collidingSides;


    //transform stuff
    for(int i = 0; i < 3; ++i) {
        if(rot[i] < 0) rot[i] = rot[i]+360;
        else if(rot[i] >= 360.0f) rot[i] = rot[i]-360;
    }
    transform = glm::translate(mat4f(1), pos);
    transform = glm::scale(transform, scale);
}

void Player::draw() const
{
    Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");

    if(renderer->getMode() == DeferredContainer::Deferred) {
        mat4f t = mat4f(1.0f);
        if(velocity.x < 0) t = glm::rotate(t,180.0f,vec3f(0,1,0));
        model.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform*t*modelOffset);
        model.program->uniform("M")->set(fullTransform*t*modelOffset);
        model.program->uniform("V")->set(cam->view);
        model.program->uniform("ambient")->set(0.5f);
        model.program->uniform("specular")->set(1.0f);

        std::string s = "brush" + toString(color);

        model.program->uniform("diffuseTex")->set(Textures2D.get(s));
        model.draw();
    }
    else if (renderer->getMode() == DeferredContainer::Forward) {
        AABB aabb = modelAabb;
        Model m;
        m.mesh = Meshes.get("1x1WireCube");
        m.program = Programs.get("lines");
        m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(fullTransform, vec3f(aabb.getDimensions()/float(sqrt(3.0f)))));
        m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
        m.draw();
    }
}

