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
            vec3f end;

            Segment() : ini(vec3f(0)), end(vec3f(0)) {}
            Segment(const vec3f& i, const vec3f& e) : ini(i), end(e) {}
    };


    Trails(const std::string& name);
    ~Trails();

    void update(float deltaTime);
    void draw() const;

    void addTrailSegment(Color color, Direction dir, const Segment& s);

private:

    Model                models[Color::NUM_COLORS][Direction::NUM_DIRECTIONS];
    std::vector<Segment> trails[Color::NUM_COLORS][Direction::NUM_DIRECTIONS];


};

#endif // TRAILS_H
