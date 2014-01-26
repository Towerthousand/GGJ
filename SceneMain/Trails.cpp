#include "Trails.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

Trails::Trails(const std::string &name)
{
	this->setName(name);

	std::vector<Vertex::Element> elems;
	elems.push_back(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3));
	elems.push_back(Vertex::Element(Vertex::Attribute::Color, Vertex::Element::Float, 3));
	Vertex::Format format = Vertex::Format(elems);

	for (int j = 0; j < Direction::NUM_DIRECTIONS; j++) {
		models[j].mesh = Mesh::loadEmpty(format, Mesh::DYNAMIC, false);
		models[j].mesh->setPrimitiveType(Mesh::LINES);
		models[j].program = Programs.get("trails");
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

void Trails::addTrailSegment(Color color, Trails::Direction dir, float x1, float x2, int y)
{/*
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

	*/


	if(x2 < x1) std::swap(x1, x2);
	std::map<float, Color>& mp = segments[dir][y];

	auto left = mp.lower_bound(x1);
	Color leftc = Color::WHITE;
	if(left != mp.begin())
	{
		auto left2 = left;
		left2--;
		leftc = left2->second;
	}

	auto right = mp.upper_bound(x2);
	Color rightc = Color::WHITE;
	if(right != mp.begin())
	{
		auto right2 = right;
		right2--;
		rightc = right2->second;
	}

	while(left != right)
	{
		auto left2 = left;
		left2++;
		mp.erase(left);
		left = left2;
	}

	if(leftc != color)
		mp[x1] = color;

	if(rightc != color)
		mp[x2] = rightc;

	updateMeshes();
}

void Trails::updateMeshes() {
	for (int dir = 0; dir < Direction::NUM_DIRECTIONS; dir++) {
		std::vector<Segment> mesh;
		for(std::map<int, std::map<float, Color> >::iterator it = segments[dir].begin(); it != segments[dir].end(); it++)
		{
			Color last = Color::WHITE;
			float lastx = 0;

			for(std::map<float, Color>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
			{
				if(last != Color::WHITE)
					if(dir == HORIZONTAL)
						mesh.push_back(Segment(vec3f(lastx, it->first, 0.5f), vec3f(it2->first, it->first, 0.5f), getColor(last)));
					else
						mesh.push_back(Segment(vec3f(it->first, lastx, 0.5f), vec3f(it->first, it2->first, 0.5f), getColor(last)));
				last = it2->second;
				lastx = it2->first;

			}

		}
		models[dir].mesh->setVertexData(&mesh[0], mesh.size()*2);
	}
}
