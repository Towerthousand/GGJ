#ifndef CUBECOMMON_HPP
#define CUBECOMMON_HPP
#include "Cube.hpp"

class CubeCommon : public Cube {
	public:
		CubeCommon(Color color, vec2i pos);
		~CubeCommon();

		void update(float deltaTime);
		void draw() const;

		bool isColliding(vec3f p) const;
		bool isColliding(AABB box) const;
		bool isPaintable() const;

		static const std::string textures[NUM_COLORS];
};

#endif // CUBECOMMON_HPP
