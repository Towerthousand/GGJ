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
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	for(int i = 0; i < (int)map.size(); ++i) {
		for(int j = 0; j < (int)map[0].size(); ++j) {
			if(map[i][j].type == Cube::AIR) continue;
			Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
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

bool Map::checkCollisions(const AABB &aabb, vec3f &pos, vec3f &nor) const
{
    pos = aabb.getCenter();
    nor = vec3f(0);
    int xmin = glm::clamp(int(aabb.getMin().x), 0, int(map.size())-1);
    int xmax = glm::clamp(int(aabb.getMax().x), 0, int(map.size())-1);
    int ymin = glm::clamp(int(aabb.getMin().y), 0, int(map[0].size())-1);
    int ymax = glm::clamp(int(aabb.getMax().y), 0, int(map[0].size())-1);

    VBE_LOG(aabb.getCenter().x << " " << aabb.getCenter().y);

    for (int i = ymin; i <= ymax; i++) {
        for (int j = xmin; j <= xmax; j++) {
            if (map[i][j].type != Cube::AIR) {
                AABB tilebox(vec3f(j, i, -1), vec3f(j+1, i+1, 1));
                if (tilebox.overlap(aabb)) {
                    return true;
                    /*vec3f cdir = aabb.getCenter() - tilebox.getCenter();
                    if (abs(cdir.x) > abs(cdir.y)) {
                        if (cdir.x > 0) {   // coll from tile right
                            nor += vec3f(1, 0, 0);
                            pos.x = std::min(pos.x, float(j+1));
                        }
                        else {              // coll from tile left
                            nor += vec3f(-1, 0, 0);
                            pos.x = std::max(pos.x, float(j));
                        }
                    }
                    else {
                        if (cdir.y > 0) {   // coll from tile top
                            nor += vec3f(0, 1, 0);
                            pos.y = std::min(pos.y, float(i+1));
                        }
                        else {              // coll from tile bottom
                            nor += vec3f(0, -1, 0);
                            pos.y = std::max(pos.y, float(i));
                        }
                    }
                    collision = true;*/
                }
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
