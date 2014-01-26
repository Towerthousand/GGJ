#include "Player.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "Map.hpp"
#include "Trails.hpp"
#include "particles/LightParticleEmitter.hpp"
#include "SceneMain.hpp"

#define MAX_VELOCITY    10.0f
#define GRAVITY         9.8f
#define ACCELERATION    40.0f
#define FRICTION_COEFF  20.0f
#define JUMP_IMPULSE    7.0f
#define ELASTICITY      0.0f


Player::Player(int playerNum, const vec3f& rot, Color col)
    : rot(rot), color(col), playerNum(playerNum) {
	Map* map = (Map*)getGame()->getObjectByName("map");
	pos = vec3f(map->getStartingPos(col).x,map->getStartingPos(col).y-0.6,0);
	fullTransform = glm::translate(mat4f(1.0f),pos);
	this->setName("player"+char('0'+playerNum));
	model.mesh = Meshes.get("brushidle0");
	model.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");

	modelAabb = model.mesh->getBoundingBox();
	modelOffset = glm::translate(mat4f(1.0f), -modelAabb.getCenter()+vec3f(0,0,0.5));

	velocity = vec3f(0.0f, JUMP_IMPULSE*0.65f, 0.0f);

	colliding = false;

	totalForce = vec3f(0.0f);
	animState = Player::JUMP;

	animCount = 0.0f;
	animTime = randomFloat(0.1f, 0.4f);

	anim = "idle";
	animIter = 0;

	scale = vec3f(0.30f/modelAabb.getRadius());

	emitter = new LightParticleEmitter(vec3f(1),color-1);
	emitter->addTo(this);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	checkMapStatus();
	vec3f initPos = pos;

	//Animation Conditions
	bool collidingSides = false;
	bool collidingFloor = false;
	bool running = false;

	//PHYSICS
	// apply forces
	vec3f dir(0);
	if(input.getKeyState(InputHandler::PLAYER_LEFT)) {
		dir += vec3f(-1, 0, 0);
		running = true;
	}
	if(input.getKeyState(InputHandler::PLAYER_RIGHT)) {
		dir += vec3f(1, 0, 0);
		running = true;
	}

	vec3f friction(0);
	if(!(fabs(velocity.x) < 0.08f)) friction = FRICTION_COEFF*vec3f(velocity.x > 0 ? -1.0 : 1.0, 0, 0);


	totalForce += ACCELERATION*dir + vec3f(0, -GRAVITY, 0) + friction;

	bool saltito = false;
	// apply impulses
	if (animState != Player::JUMP && input.getKeyDown(InputHandler::PLAYER_UP)) {
		velocity.y += JUMP_IMPULSE;
		saltito = true;
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
    vec3f bbmax = vec3f(trans*vec4f(modelAabb.getMin() + modelAabb.getDimensions()*vec3f(0.2f, 0.05f, 1.0f), 1.0f));
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
        isBrushColliding = map->isColliding(brushBox, foo);
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
        isBrushColliding = isBrushColliding || map->isColliding(brushBox, foo);

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
			if (input.getKeyDown(InputHandler::PLAYER_UP))
			{
				velocity.x -= JUMP_IMPULSE*5;
				emitter->boomSide(20, -1);
				saltito = false;
			}
			isRightWall = true;
		} else if(velocity.x < 0 ) {
			if (input.getKeyDown(InputHandler::PLAYER_UP))
			{
				velocity.x += JUMP_IMPULSE*5;
				emitter->boomSide(20, 1);
				saltito = false;
			}

		} else velocity.x = 0;

		disp.x *= min;
		colliding = true;
	}

	if(saltito)
		emitter->boom(20);

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
		animIter = 1 - animIter;
	}
	std::string s = "brush" + anim + toString(animIter);
	model.mesh = Meshes.get(s);



	// TRAILS

	if(collidingFloor && !prevOnfloor)
		emitter->boom(20);

	if (collidingFloor && isBrushColliding && (blockColor == Color::WHITE || blockColor == color) ) {
		Trails* trails = (Trails*)getGame()->getObjectByName("trails");
        trails->addTrailSegment(color, Trails::HORIZONTAL, pos.x, initPos.x, int(pos.y - 0.1), 1.5f*modelAabb.getDimensions().x);
	}
	if (collidingSides && isBrushColliding && (blockColor == Color::WHITE || blockColor == color) ) {
		Trails* trails = (Trails*)getGame()->getObjectByName("trails");
		Trails::Direction dir;
		if (isRightWall)
			dir = Trails::VERTICAL_RIGHT;
		else
			dir = Trails::VERTICAL_LEFT;

		float off = 0.21;
        trails->addTrailSegment(color, dir, pos.y-off, initPos.y-off, int(pos.x+(isRightWall?1:0)), 1.5f*modelAabb.getDimensions().x);
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

	if(pos.y < -2) die();
}

void Player::draw() const
{
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");

	if(renderer->getMode() == DeferredContainer::Deferred) {
		mat4f t = mat4f(1.0f);
		if(velocity.x < 0) t = glm::rotate(t,180.0f,vec3f(0,1,0));
		t = glm::scale(t, vec3f(1.5f));
		t = glm::translate(t, vec3f(0, 0.11, 0));
		model.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform*t*modelOffset);
		model.program->uniform("M")->set(fullTransform*t*modelOffset);
		model.program->uniform("V")->set(cam->view);
		model.program->uniform("ambient")->set(0.5f);
		model.program->uniform("specular")->set(1.0f);

		std::string s = "brush" + toString(color);

		model.program->uniform("diffuseTex")->set(Textures2D.get(s));
		model.draw();
	}/*
	else if (renderer->getMode() == DeferredContainer::Forward) {
		AABB aabb = modelAabb;
		Model m;
		m.mesh = Meshes.get("1x1WireCube");
		m.program = Programs.get("lines");
		m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(fullTransform, vec3f(aabb.getDimensions()/float(sqrt(3.0f)))));
		m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
		m.draw();
	}*/
}

void Player::checkMapStatus() {
	Map* map = (Map*)getGame()->getObjectByName("map");
	vec3f p = vec3f(fullTransform*vec4f(0,0,0,1));
	Map::Cube c = map->getCube(p);
	if(c.type == Map::Cube::FINISH) {
		//YAAAAY
		std::string s = "canvas" + toString(playerNum+1);
		map->setCanvasTex(s);
        SceneMain* scn = (SceneMain*)getGame()->getObjectByName("SCENE");
        vec4f col;
        switch(playerNum+1) {
            case 1: col = vec4f(0.25f, 0, 0, 1); break;
            case 2: col = vec4f(0, 0.25f, 0, 1); break;
            case 3: col = vec4f(0, 0, 0.25f, 1); break;
        }
        scn->setBackgroundColor(col);
	}
	Map::Cube l = map->getCube(p-vec3f(0,0.5,0));
	if (l.type == Map::Cube::SAW) {
		die();
		map->dieAt(vec3f(p-vec3f(0,0.5,0)),color);
	}
}

void Player::die() {
	Map* map = (Map*)getGame()->getObjectByName("map");
	pos = vec3f(map->getStartingPos(color).x,map->getStartingPos(color).y-0.6,0);
	fullTransform = glm::translate(mat4f(1.0f),pos);
	model.mesh = Meshes.get("brushidle0");
	velocity = vec3f(0.0f, JUMP_IMPULSE*0.65f, 0.0f);

	colliding = false;

	totalForce = vec3f(0.0f);
	animState = Player::JUMP;

	animCount = 0.0f;
	animTime = randomFloat(0.1f, 0.4f);

	anim = "idle";
	animIter = 0;

	scale = vec3f(0.30f/modelAabb.getRadius());
}
