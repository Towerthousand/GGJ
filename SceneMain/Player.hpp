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

    Color color;


private:
    Model model;
    DeferredContainer* renderer;
    Camera* cam;

    vec3f velocity;
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

    bool prevOnfloor;
    bool prevOnside;

};

#endif // PLAYER_HPP
