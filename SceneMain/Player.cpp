#include "Player.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "Map.hpp"

#define MAX_VELOCITY    10.0f
#define GRAVITY         9.8f
#define ACCELERATION    40.0f
#define FRICTION_COEFF  20.0f
#define JUMP_IMPULSE    7.0f
#define ELASTICITY      0.0f


Player::Player(const std::string& playerName, const vec3f& pos, const vec3f& rot)
    : pos(pos), rot(rot) {
    this->setName(playerName);
    model.mesh = Meshes.get("brush2");
    model.program = Programs.get("deferredModel");
    renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");

    modelAabb = model.mesh->getBoundingBox();
    modelOffset = glm::translate(mat4f(1.0f), -modelAabb.getCenter()+vec3f(0,0,0.5));

    cam = new Camera("playerCam");
    cam->projection = glm::perspective(FOV, float(SCRWIDTH)/float(SCRHEIGHT), ZNEAR, ZFAR);
    cam->pos = vec3f(0,0,20*modelAabb.getRadius());
    cam->addTo(this);

    velocity = vec3f(0.0f);
    colliding = false;

    totalForce = vec3f(0.0f);
    animState = Player::IDLE;

    animCount = 0.0f;
    animNumber = 1;
    animIter = 0;


    scale = vec3f(0.26f/modelAabb.getRadius());
}

Player::~Player() {
}

void Player::update(float deltaTime) {
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
    friction = FRICTION_COEFF*vec3f(velocity.x > 0 ? -1.0 : 1.0, 0, 0);

    totalForce += ACCELERATION*dir + vec3f(0, -GRAVITY, 0) + friction;

    // apply impulses
    if (animState != Player::JUMP && Input::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y += JUMP_IMPULSE;
    }
    // integration
    velocity = glm::clamp(velocity + totalForce*deltaTime, vec3f(-MAX_VELOCITY), vec3f(MAX_VELOCITY));
	if (glm::length(velocity) < 1.0e-3f) velocity = vec3f(0);

    //Reset totalForce;
    totalForce = vec3f(0.0f);



	vec3f disp = velocity*deltaTime;


	//NOT PHYSICS

    // collision detection
	Map* map = (Map*)getGame()->getObjectByName("map");
    AABB aabb = modelAabb;
    mat4f trans = fullTransform*modelOffset;
    aabb = AABB(vec3f(trans*vec4f(aabb.getMin(), 1.0f)), vec3f(trans*vec4f(aabb.getMax(), 1.0f)));

	colliding = false;

	//Y
	AABB newboxY(aabb.getMin()+vec3f(0,disp.y,0), aabb.getMax()+vec3f(0,disp.y,0));
	if(map->isColliding(newboxY)) {
		float min = 0;
		float max = 1;
		while(max-min > 0.001) { //search for the maximum distance you can move
			float m = (max+min)/2;
			newboxY = AABB(aabb.getMin()+vec3f(0,disp.y*m,0), aabb.getMax()+vec3f(0,disp.y*m,0));
			if(map->isColliding(newboxY))
				max = m;
			else
				min = m;
		}
        if(velocity.y < 0) collidingFloor = true;
		velocity.y = 0;
		disp.y *= min;
		colliding = true;
	}

	pos.y += disp.y;
	aabb = AABB(aabb.getMin()+vec3f(0,disp.y,0), aabb.getMax()+vec3f(0,disp.y,0));

	//X
	AABB newboxX(aabb.getMin()+vec3f(disp.x,0,0), aabb.getMax()+vec3f(disp.x,0,0));
	if(map->isColliding(newboxX)) {
		float min = 0;
		float max = 1;
		while(max-min > 0.001) { //search for the maximum distance you can move
			float m = (max+min)/2;
			newboxX = AABB(aabb.getMin()+vec3f(disp.x*m,0,0), aabb.getMax()+vec3f(disp.x*m,0,0));
			if(map->isColliding(newboxX))
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

        } else if(velocity.x < 0 ) {
            if (Input::isKeyPressed(sf::Keyboard::Up)) velocity.x += JUMP_IMPULSE*5;
        } else velocity.x = 0;



		disp.x *= min;
		colliding = true;
	}

    pos.x += disp.x;

    //ANIMATION
    if(collidingSides)  animState = Player::WALL;
    else if(collidingFloor) {
        if(running)      {
            animState = Player::RUN;
            animNumber = 3;
        } else {
            animState = Player::IDLE;
            animNumber = 1;

        }
    } else {
        animState = Player::JUMP;
        animNumber = 5;

    }

    animCount += deltaTime;
    if(animCount >= 0.1f) {
        animCount -= 0.1f;
        animIter = 1 - animIter;
    }
    std::string s = "brush" + toString(animNumber+animIter);
    model.mesh = Meshes.get(s);




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
    if(renderer->getMode() == DeferredContainer::Deferred) {
        Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
        mat4f t = mat4f(1.0f);
        if(velocity.x < 0) t = glm::rotate(t,180.0f,vec3f(0,1,0));
        model.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform*t*modelOffset);
        model.program->uniform("M")->set(fullTransform*t*modelOffset);
        model.program->uniform("V")->set(cam->view);
        model.program->uniform("ambient")->set(0.5f);
        model.program->uniform("specular")->set(1.0f);
        model.program->uniform("diffuseTex")->set(Textures2D.get("brushR"));
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

