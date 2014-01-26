#include "Colors.hpp"

vec3f colorValues[] = {
	vec3f(1, 1, 1),
	vec3f(1, 0, 0),
	vec3f(0, 1, 0),
	vec3f(0, 0, 1),
};

vec3f getColor(Color col)
{
	return colorValues[col];
}
