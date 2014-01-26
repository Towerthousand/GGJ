#include "Map.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "SceneMain.hpp"

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
	  {"botCube","nullWhite"},
	  {"botCube","nullRed"},
	  {"botCube","nullGreen"},
	  {"botCube","nullBlue"}
    },
    { //START
      {"",""},
      {"pote","poteR"},
      {"pote","poteG"},
      {"pote","poteB"}
    },
    { //FINISH
      {"canvas","canvasW"},
      {"",""},
      {"",""},
      {"",""}
    }
};

Map::Map() : map(std::vector<std::vector<Cube> >(1, std::vector<Cube>())) {
	setName("map");

	cube.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
    canvasTexture = "canvasW";
}

Map::~Map() {
}

void Map::loadFromFile(const std::string& mapfile) {
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
		else {
			map[i].push_back(translate(c));
			if(translate(c).type == Cube::START)
				startingPos[translate(c).color-1] = vec2f(map[i].size()-1+0.5,i);
		}
	}
	std::reverse(map.begin(),map.end());
	for(int i = 0; i < 3; ++i) {
		startingPos[i].y = (int)map.size() - startingPos[i].y;
	}
}

void Map::update(float deltaTime) {
	(void) deltaTime;
}

void Map::draw() const {
    SceneMain* sc = (SceneMain*)getGame()->getObjectByName("SCENE");
    Color playerColor = (Color)(sc->playerNum + 1);
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	if(renderer->getMode() == DeferredContainer::Deferred) {
		for(int i = 0; i < (int)map.size(); ++i) {
			for(int j = 0; j < (int)map[0].size(); ++j) {
                if(map[i][j].type == Cube::FINISH) {
                    float rot = -30.0f;
                    mat4f mat = fullTransform;
                    mat = glm::translate(mat,vec3f(j,i,0));
                    mat = glm::rotate(mat,rot,vec3f(0,1.0f,0));
                    cube.program->uniform("MVP")->set(cam->projection*cam->view*mat);
                    cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
                    cube.program->uniform("V")->set(cam->view);
                    cube.mesh = Meshes.get(models_textures[map[i][j].type][map[i][j].color][0]);
                    cube.program->uniform("diffuseTex")->set(Textures2D.get(canvasTexture));
                    cube.draw();
                    continue;
                }
                if(map[i][j].type == Cube::START) {
                    cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.2)));
                    cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.2)));
                    cube.program->uniform("V")->set(cam->view);
                    cube.mesh = Meshes.get(models_textures[map[i][j].type][map[i][j].color][0]);
                    cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[map[i][j].type][map[i][j].color][1]));
                    cube.draw();
                    continue;
                }
                if(map[i][j].type == Cube::AIR || (playerColor != map[i][j].color && map[i][j].color != Color::WHITE)) continue;
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
    /*else if (renderer->getMode() == DeferredContainer::Forward) {
		for(int i = 0; i < (int)map.size(); ++i) {
			for(int j = 0; j < (int)map[0].size(); ++j) {
                if(map[i][j].type == Cube::AIR || map[i][j].type == Cube::FINISH || map[i][j].type == Cube::START) continue;
				Model m;
				m.mesh = Meshes.get("1x1WireCube");
				m.program = Programs.get("lines");
				m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
                m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(glm::translate(mat4f(1.0f),vec3f(j+0.5,i+0.5,0)),vec3f(0.5f)));
				m.draw();
			}
		}
    }*/
}

bool Map::isColliding(const vec3f& pos, Color &color) const {
	int x = floor(pos.x);
    int y = floor(pos.y);
    if (x < 0 || y < 0 || x >= int(map[0].size()) || y >= int(map.size())) return false;
    if (map[y][x].type == Cube::AIR || map[y][x].type == Cube::FINISH || map[y][x].type == Cube::START) return false;
    color = map[y][x].color;
    return true;
}

bool Map::isColliding(const AABB& aabb, Color &color) const
{
	int xmin = floor(aabb.getMin().x);
	int xmax = floor(aabb.getMax().x);
	int ymin = floor(aabb.getMin().y);
	int ymax = floor(aabb.getMax().y);

	for (int i = ymin; i <= ymax && i < (int)map.size(); i++) {
		if(i < 0) continue;
		for (int j = xmin; j <= xmax && j < (int)map[i].size(); j++) {
			if(j < 0) continue;
            if (map[i][j].type != Cube::AIR && map[i][j].type != Cube::FINISH && map[i][j].type != Cube::START) {
				AABB tilebox(vec3f(j, i, -1), vec3f(j+1, i+1, 1));
				if (tilebox.overlap(aabb)) {
                    color = map[i][j].color;
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
        case '<' : return Cube(Color::WHITE	,Cube::SAW);
        case 'Z' : return Cube(Color::RED	,Cube::SAW);
        case 'X' : return Cube(Color::GREEN	,Cube::SAW);
        case 'C' : return Cube(Color::BLUE	,Cube::SAW);
        case 'J' : return Cube(Color::WHITE	,Cube::BUMP);
        case 'K' : return Cube(Color::RED	,Cube::BUMP);
        case 'L' : return Cube(Color::GREEN	,Cube::BUMP);
        case 'P' : return Cube(Color::BLUE	,Cube::BUMP);
        case 'A' : return Cube(Color::RED	,Cube::START);
        case 'S' : return Cube(Color::GREEN	,Cube::START);
        case 'D' : return Cube(Color::BLUE	,Cube::START);
        case 'M' : return Cube(Color::WHITE	,Cube::FINISH);
        case ' ' : return Cube(Color::WHITE	,Cube::AIR);
		default: {VBE_ASSERT(false, "INVALID CHARACTER " << c);}
	}
}

Map::Cube Map::getCube(vec3f pos) {
    if(pos.x < 0 || pos.y < 0 || pos.x >= map[0].size() || pos.y >= map.size()) return Cube(Color::WHITE,Cube::AIR);
    return map[floor(pos.y)][floor(pos.x)];
}

void Map::setCanvasTex(std::string tex) {
    canvasTexture = tex;
}

void Map::clipTrail(Color col, bool horizontal, int y, float &x1, float &x2)
{
    float pos = 0.5f*(x1 + x2);
    int xdim = horizontal ? map[0].size() : map.size();
    int ipos = floor(glm::clamp(pos, 0.0f, float(xdim) - 0.1f));
    x1 = glm::clamp(x1,  0.0f, float(xdim) - 0.1f);
    int iini = floor(x1);
    x2 = glm::clamp(x2,  0.0f, float(xdim) - 0.1f);
    int iend = floor(x2);

    if (horizontal) {
        for (int i = ipos; i >= iini; i--) {
            if (map[y][i].type == Cube::AIR || (map[y][i].color != Color::WHITE && map[y][i].color != col)) {
                x1 = float(i + 1.0);
                break;
            }
        }
        for (int i = ipos; i <= iend; i++) {
            if (map[y][i].type == Cube::AIR || (map[y][i].color != Color::WHITE && map[y][i].color != col)) {
                x2 = float(i);
                break;
            }
        }
    }
    else {
        for (int i = ipos; i >= iini; i--) {
            if (map[i][y].type == Cube::AIR || (map[i][y].color != Color::WHITE && map[i][y].color != col)) {
                x1 = float(i + 1.0);
                break;
            }
        }
        for (int i = ipos; i <= iend; i++) {
            if (map[i][y].type == Cube::AIR || (map[i][y].color != Color::WHITE && map[i][y].color != col)) {
                x2 = float(i);
                break;
            }
        }
    }
}

