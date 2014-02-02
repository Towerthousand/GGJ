#ifndef MAP_HPP
#define MAP_HPP
#include "commons.hpp"
#include "Colors.hpp"

class DeferredContainer;
class Map : public GameObject {
	public:
		class OldCube {
            public:

				enum Type {
					AIR = 0,
					FLOOR,
					SAW,
					BUMP,
                    START,
                    FINISH,
					NUM_TYPES
				};

				OldCube(Color c, Type t) : color(c), type(t), deathColor(Color::WHITE) {}

				Color color;
				Type type;
				Color deathColor;
		};

		Map();
		~Map();

		void loadFromFile(const std::string& mapfile);

		void update(float deltaTime);
		void draw() const;

        bool isColliding(const vec3f& aabb, Color &color) const;
        bool isColliding(const AABB& aabb,  Color &color) const;

		OldCube getCube(vec3f pos);
        vec2f getStartingPos(Color col) { return startingPos[col-1];}

        void setCanvasTex(std::string tex);

        void clipTrail(Color col, bool horizontal, int y, float &x1, float &x2);

		void dieAt(vec3f pos, Color col);

	private:
		static std::string models_textures[OldCube::NUM_TYPES][Color::NUM_COLORS][2];

		OldCube translate(char c);

		std::vector<std::vector <OldCube> > map;
		std::vector<std::vector <std::vector<bool> > > deaths;
		Model cube;
		DeferredContainer* renderer;

        vec2f startingPos[3];
        std::string canvasTexture;
};

#endif // MAP_HPP
