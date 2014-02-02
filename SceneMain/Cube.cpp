#include "Cube.hpp"
#include "DeferredContainer.hpp"

Cube::Cube(vec2i pos) : color(WHITE), pos(pos), renderer(nullptr) {
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
}

Cube::~Cube() {
}

Color Cube::getColor() const {
	return color;
}

vec2i Cube::getPos() const {
	return pos;
}
