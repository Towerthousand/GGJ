#include "Map.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"

Map::Map(const std::string &mapfile) : map(std::vector<std::vector<Cube> >(1, std::vector<Cube>())) {
	setName("map");
	std::ifstream in(mapfile.c_str(), std::ifstream::in);
	VBE_ASSERT(in, "While parsing map: Cannot open " << mapfile );
	char c = 'x';
	int i = 0;
	while(1) {
		in >> std::noskipws >> c;
		if(c == '%') break;
		if(c == '\n') {
			map.push_back(std::vector<Cube>());
			++i;
		}
		else map[i].push_back(translate(c));
	}
	std::reverse(map.begin(),map.end());
	cube.mesh = Meshes.get("Cube");
	cube.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
}

Map::~Map() {
}

void Map::update(float deltaTime) {
	(void) deltaTime;
}

void Map::draw() const {
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	if(renderer->getMode() == DeferredContainer::Deferred) {
		for(int i = 0; i < (int)map.size(); ++i) {
			for(int j = 0; j < (int)map[0].size(); ++j) {
				if(map[i][j].type == Cube::AIR) continue;
				cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0)));
				cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
				cube.program->uniform("V")->set(cam->view);
				cube.program->uniform("ambient")->set(0.5f);
				cube.program->uniform("specular")->set(1.0f);
				switch(map[i][j].color) {
					case Cube::WHITE: cube.program->uniform("diffuseTex")->set(Textures2D.get("nullWhite")); break;
					case Cube::RED: cube.program->uniform("diffuseTex")->set(Textures2D.get("nullRed")); break;
					case Cube::GREEN: cube.program->uniform("diffuseTex")->set(Textures2D.get("nullGreen")); break;
					case Cube::BLUE: cube.program->uniform("diffuseTex")->set(Textures2D.get("nullBlue")); break;
				}
				cube.draw();
			}
		}
	}
	else if (renderer->getMode() == DeferredContainer::Forward) {
		for(int i = 0; i < (int)map.size(); ++i) {
			for(int j = 0; j < (int)map[0].size(); ++j) {
				Model m;
				m.mesh = Meshes.get("1x1WireCube");
				m.program = Programs.get("lines");
				m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
				m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(glm::translate(mat4f(1.0f),vec3f(j+0.5,i+0.5,-0.5)),vec3f(0.5f)));
				m.draw();
			}
		}
	}
}

bool Map::isColliding(const AABB &aabb) const
{
	int xmin = aabb.getMin().x;
	int xmax = aabb.getMax().x;
	int ymin = aabb.getMin().y;
	int ymax = aabb.getMax().y;

	for (int i = ymin; i <= ymax && i < map.size(); i++) {
		if(i < 0) continue;
		for (int j = xmin; j <= xmax && j < map[0].size(); j++) {
			if(j < 0) continue;
			if (map[i][j].type != Cube::AIR) {
				AABB tilebox(vec3f(j, i, -1), vec3f(j+1, i+1, 1));
				if (tilebox.overlap(aabb)) return true;
			}
		}
	}
	return false;
}

Map::Cube Map::translate(char c) {
	switch (c) {
		case 'W' : return Cube(Cube::WHITE	,Cube::FLOOR);
		case 'R' : return Cube(Cube::RED	,Cube::FLOOR);
		case 'G' : return Cube(Cube::GREEN	,Cube::FLOOR);
		case 'B' : return Cube(Cube::BLUE	,Cube::FLOOR);
		case ' ' : return Cube(Cube::WHITE	,Cube::AIR);
		default: {VBE_ASSERT(false, "INVALID CHARACTER " << c);}
	}
}
