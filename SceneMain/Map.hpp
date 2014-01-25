#ifndef MAP_HPP
#define MAP_HPP
#include "commons.hpp"

class DeferredContainer;
class Map : public GameObject {
	public:
		class Cube {
			public:
				enum Color {
					WHITE = 0,
					RED,
					GREEN,
					BLUE,
					NUM_COLORS
				};

				enum Type {
					AIR = 0,
					FLOOR,
					SAW,
					BUMP,
					NUM_TYPES
				};

				Cube(Color c, Type t) : color(c), type(t) {}

			private:
				Color color;
				Type type;
				friend class Map;
		};

		Map(const std::string& mapfile);
		~Map();

		void update(float deltaTime);
		void draw() const; //while(1) fork;

		bool isColliding(const vec3f& aabb) const;
		bool isColliding(const AABB& aabb) const;

	private:
		static std::string models_textures[Cube::NUM_TYPES][Cube::NUM_COLORS][2];

		Cube translate(char c);

		std::vector<std::vector <Cube> > map;
		Model cube;
		DeferredContainer* renderer;
};

#endif // MAP_HPP
