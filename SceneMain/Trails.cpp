#include "Trails.hpp"

Trails::Trails(const std::string &name)
{
    this->setName(name);

    std::vector<Vertex::Element> elems(1, Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3)));
    Vertex::Format format = Vertex::Format(elems);
    for (int i = 0; i < Color::NUM_COLORS; i++) {
        for (int j = 0; j < Direction::NUM_DIRECTIONS; j++) {
            models[i][j].mesh = Mesh::loadEmpty(format, Mesh::DYNAMIC, false);
            models[i][j].mesh->setPrimitiveType(Mesh::LINES);
            models[i][j].program = Programs.get("trails");
        }
    }
}

Trails::~Trails()
{
}

void Trails::update(float )
{
}

void Trails::draw() const
{
    for (int i = 0; i < Color::NUM_COLORS; i++) {
        Texture2D* tex;
        switch (i) {
            case RED:   tex = Textures2D.get("trailR"); break;
            case GREEN: tex = Textures2D.get("trailG"); break;
            case BLUE:  tex = Textures2D.get("trailB"); break;
        }
        for (int j = 0; j < Direction::NUM_DIRECTIONS; j++) {

            float hor, ver;
            vec2f side;
            switch (j) {
                case HORIZONTAL:     hor = 1.0f; ver = 0.0f; side = vec2f(0, 0.25f);  break;
                case VERTICAL_LEFT:  hor = 0.0f; ver = 1.0f; side = vec2f(-0.25f, 0); break;
                case VERTICAL_RIGHT: hor = 0.0f; ver = 1.0f; side = vec2f( 0.25f, 0); break;
            }
            models[i][j].program->uniform("tex")->set(tex);
            models[i][j].program->uniform("horTrail")->set(hor);
            models[i][j].program->uniform("verTrail")->set(ver);
            models[i][j].program->uniform("side")->set(side);
            models[i][j].draw();
        }
    }
}

void Trails::addTrailSegment(Color color, Trails::Direction dir, const Trails::Segment &s)
{
    trails[color][dir].push_back(s);
    //TODO: merge with existing segments
}
