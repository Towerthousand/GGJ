#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"
#include "Colors.hpp"

class DeferredContainer;
class Camera;

class Player : public GameObject {
public:

    Player(const std::string &playerName, const vec3f& pos = vec3f(0.0f), const vec3f& rot = vec3f(0.0f));
    virtual ~Player();

    void update(float deltaTime);
    void draw() const;

    vec3f pos;
    vec3f rot;
    vec3f scale;
	vec3f velocity;

    Color color;


private:
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
