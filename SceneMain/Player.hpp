#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"
#include "Colors.hpp"
#include "InputHandler.hpp"

class DeferredContainer;
class Camera;
class LightParticleEmitter;

class Player : public GameObject {
public:

    Player(int playerNum, const vec3f& rot = vec3f(0.0f), Color col = Color::RED);
    virtual ~Player();

    void update(float deltaTime);
    void draw() const;

    vec3f pos;
    vec3f rot;
    vec3f scale;
	vec3f velocity;

    Color color;

	InputHandler input;

private:
    void checkMapStatus();
    void die();
	LightParticleEmitter* emitter;

	int playerNum;

    Model model;
    DeferredContainer* renderer;

    bool colliding;

    mat4f modelOffset;
    vec3f totalForce;

    enum Animation {
        IDLE,
        RUN,
        JUMP,
        WALL
    };

    Animation animState;
    float animCount;
    float animTime;

    std::string anim;
    int animIter;
    AABB modelAabb;


    bool prevOnfloor;
    bool prevOnside;

};

#endif // PLAYER_HPP
