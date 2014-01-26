#ifndef TRAILS_H
#define TRAILS_H

#include "commons.hpp"
#include "Player.hpp"
#include "Colors.hpp"

class DeferredContainer;

class Trails : public GameObject
{
public:

    enum Direction {
        HORIZONTAL,
        VERTICAL_LEFT,
        VERTICAL_RIGHT,
        NUM_DIRECTIONS
    };

    class Segment {
        public:
			vec3f ini;
			vec3f color1;
			vec3f end;
			vec3f color2;

            Segment() : ini(vec3f(0)), end(vec3f(0)) {}
			Segment(const vec3f& i, const vec3f& e) : ini(i), end(e) {}
			Segment(const vec3f& i, const vec3f& e, const vec3f& col) : ini(i), color1(col), end(e), color2(col) {}
	};


    Trails(const std::string& name);
    ~Trails();

    void update(float deltaTime);
    void draw() const;

    void addTrailSegment(Color color, Direction dir, float x1, float x2, int y, float width);

private:

	void updateMeshes();
	Model models[Direction::NUM_DIRECTIONS];
	std::map<int, std::map<float, Color> > segments[NUM_DIRECTIONS];

    DeferredContainer* renderer;


};

#endif // TRAILS_H
