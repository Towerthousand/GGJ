#ifndef CUBE_HPP
#define CUBE_HPP
#include "commons.hpp"
#include "Colors.hpp"

class DeferredContainer;
class Cube : public GameObject {
	public:
		Cube(vec2i pos);
		virtual ~Cube();

		virtual void update(float deltaTime) = 0;
		virtual void draw() const = 0;
		virtual bool isColliding(vec3f point) = 0;
		virtual bool isColliding(AABB box) = 0;

		virtual Color getColor();
		vec2i getPos();

	protected:
		Color color;
		Model cubeModel;

		const vec2i pos;
		DeferredContainer* renderer;
};

#endif // CUBE_HPP
