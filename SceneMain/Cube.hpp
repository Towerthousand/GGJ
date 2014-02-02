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
		virtual bool isColliding(vec3f point) const = 0;
		virtual bool isColliding(AABB box) const = 0;
		virtual bool isPaintable() const = 0;

		virtual Color getColor() const;
		vec2i getPos() const;

	protected:
		Color color;
		Model cubeModel;

		const vec2i pos;
		DeferredContainer* renderer;
};

#endif // CUBE_HPP
