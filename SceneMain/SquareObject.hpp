#ifndef SQUAREOBJECT_HPP
#define SQUAREOBJECT_HPP
#include "commons.hpp"

class DeferredContainer;
class SquareObject : public GameObject {
    public:
        SquareObject();
        ~SquareObject();

        void update(float deltaTime);
        void draw() const;

        vec3f pos;

    private:
        Model quad;
        DeferredContainer* renderer;
};

#endif // SQUAREOBJECT_HPP
