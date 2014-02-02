#ifndef MAP_HPP
#define MAP_HPP
#include "commons.hpp"
#include "Colors.hpp"

class Cube;
class DeferredContainer;
class Map : public GameObject {
	public:
		Map();
		~Map();

		void loadFromFile(const std::string& mapfile);

		void update(float deltaTime);
		void draw() const;

        bool isColliding(const vec3f& aabb, Color &color) const;
        bool isColliding(const AABB& aabb,  Color &color) const;

		Cube* getCube(int x, int y) const;
		Cube* getCube(float x, float y) const {return getCube(int(floor(x)),int(floor(y)));}
		Cube* getCube(vec3f pos) const {return getCube(int(floor(pos.x)),int(floor(pos.y)));}
		Cube* getCube(vec2f pos) const {return getCube(int(floor(pos.x)),int(floor(pos.y)));}
		Cube* getCube(vec2i pos) const {return getCube(pos.x,pos.y);}
		vec2f getStartingPos(Color col) const { return startingPos[col-1];}

        void setCanvasTex(std::string tex);

        void clipTrail(Color col, bool horizontal, int y, float &x1, float &x2);

		void dieAt(vec3f pos, Color col);

	private:
		Cube* translate(char c, vec2i pos) const;

		std::vector<std::vector <Cube*> > map;

		vec2f startingPos[3];
};

#endif // MAP_HPP
