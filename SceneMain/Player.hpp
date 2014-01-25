#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"

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

private:
    Model model;
    DeferredContainer* renderer;
    Camera* cam;

    vec3f velocity;
    bool colliding;

    vec3f totalForce;

    enum Animation {
        IDLE,
        RUN,
        JUMP,
        WALL
    };

    Animation animState;


};

#endif // PLAYER_HPP
