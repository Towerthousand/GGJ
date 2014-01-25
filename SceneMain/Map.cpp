#include "Map.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"

std::string Map::models_textures[Map::Cube::NUM_TYPES][Color::NUM_COLORS][2] = {
	{ //AIR
	  {"",""},
	  {"",""},
	  {"",""},
	  {"",""}
	},
	{ //FLOOR
	  {"cube","nullWhite"},
	  {"cube","nullRed"},
	  {"cube","nullGreen"},
	  {"cube","nullBlue"}
	},
	{ //SAW
	  {"sawCube","nullWhite"},
	  {"sawCube","nullRed"},
	  {"sawCube","nullGreen"},
	  {"sawCube","nullBlue"}
	},
	{ //BUMP
	  {"botCube","botCubeW"},
	  {"botCube","botCubeR"},
	  {"botCube","botCubeG"},
	  {"botCube","botCubeB"}
	}
};

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
                cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.5)));
                cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.5)));
				cube.program->uniform("V")->set(cam->view);
				cube.program->uniform("ambient")->set(0.5f);
				cube.program->uniform("specular")->set(1.0f);
				cube.mesh = Meshes.get(models_textures[map[i][j].type][map[i][j].color][0]);
				cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[map[i][j].type][map[i][j].color][1]));
				cube.draw();
				if(map[i][j].type == Cube::SAW) {
					float rot = GLOBALCLOCK.getElapsedTime().asSeconds()*10000;
					cube.program->uniform("MVP")->
							set(cam->projection*cam->view*
                                glm::translate(glm::rotate(glm::translate(fullTransform,vec3f(j,i+1,0)),rot,vec3f(1,0,0)),vec3f(0,-1,0.5)));
					cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
					cube.mesh = Meshes.get("saw");
					cube.program->uniform("diffuseTex")->set(Textures2D.get("saw"));
					cube.draw();
				}
			}
		}
	}
	else if (renderer->getMode() == DeferredContainer::Forward) {
		for(int i = 0; i < (int)map.size(); ++i) {
			for(int j = 0; j < (int)map[0].size(); ++j) {
				if(map[i][j].type == Cube::AIR) continue;
				Model m;
				m.mesh = Meshes.get("1x1WireCube");
				m.program = Programs.get("lines");
				m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
                m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(glm::translate(mat4f(1.0f),vec3f(j+0.5,i+0.5,0)),vec3f(0.5f)));
				m.draw();
			}
		}
	}
}

bool Map::isColliding(const vec3f& pos) const {
	int x = floor(pos.x);
	int y = floor(pos.y);
    return !(x < 0 || y < 0 || x >= int(map[0].size()) || y >= int(map.size()) || map[y][x].type == Cube::AIR);
}

bool Map::isColliding(const AABB& aabb) const
{
	int xmin = floor(aabb.getMin().x);
	int xmax = floor(aabb.getMax().x);
	int ymin = floor(aabb.getMin().y);
	int ymax = floor(aabb.getMax().y);

	for (int i = ymin; i <= ymax && i < (int)map.size(); i++) {
		if(i < 0) continue;
		for (int j = xmin; j <= xmax && j < (int)map[i].size(); j++) {
			if(j < 0) continue;
			if (map[i][j].type != Cube::AIR) {
				AABB tilebox(vec3f(j, i, -1), vec3f(j+1, i+1, 1));
				if (tilebox.overlap(aabb)) {
					return true;
				}
			}
		}
	}
	return false;
}

Map::Cube Map::translate(char c) {
	switch (c) {
        case 'W' : return Cube(Color::WHITE	,Cube::FLOOR);
        case 'R' : return Cube(Color::RED	,Cube::FLOOR);
        case 'G' : return Cube(Color::GREEN	,Cube::FLOOR);
        case 'B' : return Cube(Color::BLUE	,Cube::FLOOR);
        case 'Z' : return Cube(Color::RED	,Cube::SAW);
        case 'X' : return Cube(Color::GREEN	,Cube::SAW);
        case 'C' : return Cube(Color::BLUE	,Cube::SAW);
        case ' ' : return Cube(Color::WHITE	,Cube::AIR);
		default: {VBE_ASSERT(false, "INVALID CHARACTER " << c);}
	}
}
