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

    scale = vec3f(0.26f/model.mesh->getBoundingBox().getRadius());

}

Player::~Player() {
}

void Player::update(float deltaTime) {

	//PHYSICS
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

    vec3f totalForce = ACCELERATION*dir + vec3f(0, -GRAVITY, 0) + friction;

    // apply impulses
    if (Input::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y += JUMP_IMPULSE;
    }
    // integration
    velocity = glm::clamp(velocity + totalForce*deltaTime, vec3f(-MAX_VELOCITY), vec3f(MAX_VELOCITY));
	if (glm::length(velocity) < 1.0e-3f) velocity = vec3f(0);


	vec3f disp = velocity*deltaTime;


	//NOT PHYSICS

    // collision detection
	Map* map = (Map*)getGame()->getObjectByName("map");
	AABB aabb = model.mesh->getBoundingBox();
	aabb = AABB(vec3f(fullTransform*vec4f(aabb.getMin(), 1.0f)), vec3f(fullTransform*vec4f(aabb.getMax(), 1.0f)));

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
		velocity.x = 0;
		disp.x *= min;
		colliding = true;
	}

    pos.x += disp.x;


	//transform stuff
	for(int i = 0; i < 3; ++i) {
		if(rot[i] < 0) rot[i] = rot[i]+360;
		else if(rot[i] >= 360.0f) rot[i] = rot[i]-360;
	}

	transform = glm::translate(mat4f(1),pos);
	transform = glm::scale(transform, scale);
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
		m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(fullTransform,vec3f(model.mesh->getBoundingBox().getDimensions()/model.mesh->getBoundingBox().getRadius())));
        m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
        m.draw();
    }
}

