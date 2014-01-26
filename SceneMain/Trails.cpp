#include "Trails.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

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

    renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
}

Trails::~Trails()
{
}

void Trails::update(float )
{
}

void Trails::draw() const
{
    if (renderer->getMode() != DeferredContainer::Forward)
        return;

    Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);

	for (int j = 0; j < Direction::NUM_DIRECTIONS; j++) {

		float hor, ver;
		vec2f side;
		std::string sside = "";
		if (j != HORIZONTAL) sside = "V";
		Texture2D* tex = Textures2D.get("trailR" + sside);
		switch (j) {
			case HORIZONTAL:     hor = 1.0f; ver = 0.0f; side = vec2f( 0, -1.0f); break;
			case VERTICAL_LEFT:  hor = 0.0f; ver = 1.0f; side = vec2f(-1.0f, 0);  break;
			case VERTICAL_RIGHT: hor = 0.0f; ver = 1.0f; side = vec2f( 1.0f, 0);  break;
		}
		models[j].program->uniform("MVP")->set(cam->projection*cam->view);
		models[j].program->uniform("tex")->set(tex);
		models[j].program->uniform("horTrail")->set(hor);
		models[j].program->uniform("verTrail")->set(ver);
		models[j].program->uniform("side")->set(side);
		models[j].draw();
	}

    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void Trails::addTrailSegment(Color color, Trails::Direction dir, const Trails::Segment &s)
{
    std::vector<Segment>& segs = trails[color][dir];
    bool merged = false;
    for (unsigned int i = 0; i < segs.size() && !merged; i++) {
        Segment& s2 = segs[i];
        if (dir == Direction::HORIZONTAL) {
            if (fabs(s2.ini.y - s.ini.y) < 0.1f) {
                if ((s.ini.x < s2.end.x + 0.01f && s.ini.x > s2.ini.x - 0.01f) || (s.end.x < s2.end.x + 0.01f && s.end.x > s2.ini.x - 0.01f)) {
                    s2.ini.x = std::min(s2.ini.x, s.ini.x);
                    s2.end.x = std::max(s2.end.x, s.end.x);
                    merged = true;
                }
            }
        }
        else {
            if (fabs(s2.ini.x - s.ini.x) < 0.1f) {
                if ((s.ini.y < s2.end.y + 0.01f && s.ini.y > s2.ini.y - 0.01f) || (s.end.y < s2.end.y + 0.01f && s.end.y > s2.ini.y - 0.01f)) {
                    s2.ini.y = std::min(s2.ini.y, s.ini.y);
                    s2.end.y = std::max(s2.end.y, s.end.y);
                    merged = true;
                }
            }
        }
    }
    if (!merged) segs.push_back(s);

    models[color][dir].mesh->setVertexData(&trails[color][dir][0], trails[color][dir].size()*2);
}
