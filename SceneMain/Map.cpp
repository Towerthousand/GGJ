#include "Map.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "SceneMain.hpp"
#include "CubeCommon.hpp"

std::string Map::models_textures[Map::OldCube::NUM_TYPES][Color::NUM_COLORS][2] = {
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

Map::Map() : map(std::vector<std::vector<OldCube> >(1, std::vector<OldCube>())) {
	setName("map");

	cube.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
	canvasTexture = "canvasW";
	CubeCommon* c = new CubeCommon(RED,vec2i(0,1));
	c->addTo(this);
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
			map.push_back(std::vector<OldCube>());
			++i;
		}
		else {
			map[i].push_back(translate(c));
			if(translate(c).type == OldCube::START)
				startingPos[translate(c).color-1] = vec2f(map[i].size()-1+0.5,i);
		}
	}
	std::reverse(map.begin(),map.end());
	for(int i = 0; i < 3; ++i) {
		startingPos[i] = vec2f(0,0);
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
				OldCube current = map[i][j];
				if(current.type == OldCube::FINISH) {
					float rot = -30.0f;
					mat4f mat = fullTransform;
					mat = glm::translate(mat,vec3f(j,i,0));
					mat = glm::rotate(mat,rot,vec3f(0,1.0f,0));
					cube.program->uniform("MVP")->set(cam->projection*cam->view*mat);
					cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
					cube.program->uniform("V")->set(cam->view);
					cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
					cube.program->uniform("diffuseTex")->set(Textures2D.get(canvasTexture));
					cube.draw();
					continue;
				}
				else if(current.type == OldCube::START) {
					cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.2)));
					cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.2)));
					cube.program->uniform("V")->set(cam->view);
					cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
					cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[current.type][current.color][1]));
					cube.draw();
					continue;
				}
				else if(current.type == OldCube::SAW) {
					if(current.color == playerColor || current.color == Color::WHITE) {
						cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.5)));
						cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.5)));
						cube.program->uniform("V")->set(cam->view);
						cube.program->uniform("ambient")->set(0.5f);
						cube.program->uniform("specular")->set(1.0f);
						cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
						cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[current.type][current.color][1]));
						cube.draw();
					}
					if(current.color == playerColor || current.color == Color::WHITE  || current.deathColor != Color::WHITE) {
						cube.program = Programs.get("deferredSaw");
						cube.program->uniform("ambient")->set(0.5f);
						cube.program->uniform("specular")->set(1.0f);
						float rot = GLOBALCLOCK.getElapsedTime().asSeconds()*10000;
						cube.program->uniform("MVP")->
								set(cam->projection*cam->view*
									glm::translate(glm::rotate(glm::translate(fullTransform,vec3f(j,i+1,0)),rot,vec3f(1,0,0)),vec3f(0,-1,0.5)));
						cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
						cube.program->uniform("colorID")->set(current.deathColor);
						cube.mesh = Meshes.get("saw");
						cube.program->uniform("diffuseTex")->set(Textures2D.get("saw"));
						cube.draw();
						cube.program = Programs.get("deferredModel");
					}
					continue;
				}
				if(current.type == OldCube::AIR || (playerColor != current.color && current.color != Color::WHITE)) continue;
				cube.program = Programs.get("deferredCubes");
				cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.5)));
				cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.5)));
				cube.program->uniform("V")->set(cam->view);
				cube.program->uniform("ambient")->set(0.5f);
				cube.program->uniform("specular")->set(1.0f);
				cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
				cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[current.type][current.color][1]));
				cube.program->uniform("normalsTex")->set(Textures2D.get("normalsCubes"));
				cube.draw();
				cube.program = Programs.get("deferredModel");
			}
		}
	}
	else if (renderer->getMode() == DeferredContainer::Forward) {
		for(int i = 0; i < (int)map.size(); ++i) {
			for(int j = 0; j < (int)map[0].size(); ++j) {
				if(map[i][j].type == OldCube::AIR || map[i][j].type == OldCube::FINISH || map[i][j].type == OldCube::START) continue;
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

bool Map::isColliding(const vec3f& pos, Color &color) const {
	int x = floor(pos.x);
	int y = floor(pos.y);
	if (x < 0 || y < 0 || x >= int(map[0].size()) || y >= int(map.size())) return false;
	if (map[y][x].type == OldCube::AIR || map[y][x].type == OldCube::FINISH || map[y][x].type == OldCube::START) return false;
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
			if (map[i][j].type != OldCube::AIR && map[i][j].type != OldCube::FINISH && map[i][j].type != OldCube::START) {
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

Map::OldCube Map::translate(char c) {
	switch (c) {
		case 'W' : return OldCube(Color::WHITE	,OldCube::FLOOR);
		case 'R' : return OldCube(Color::RED	,OldCube::FLOOR);
		case 'G' : return OldCube(Color::GREEN	,OldCube::FLOOR);
		case 'B' : return OldCube(Color::BLUE	,OldCube::FLOOR);
		case '<' : return OldCube(Color::WHITE	,OldCube::SAW);
		case 'Z' : return OldCube(Color::RED	,OldCube::SAW);
		case 'X' : return OldCube(Color::GREEN	,OldCube::SAW);
		case 'C' : return OldCube(Color::BLUE	,OldCube::SAW);
		case 'J' : return OldCube(Color::WHITE	,OldCube::BUMP);
		case 'K' : return OldCube(Color::RED	,OldCube::BUMP);
		case 'L' : return OldCube(Color::GREEN	,OldCube::BUMP);
		case 'P' : return OldCube(Color::BLUE	,OldCube::BUMP);
		case 'A' : return OldCube(Color::RED	,OldCube::START);
		case 'S' : return OldCube(Color::GREEN	,OldCube::START);
		case 'D' : return OldCube(Color::BLUE	,OldCube::START);
		case 'M' : return OldCube(Color::WHITE	,OldCube::FINISH);
		case ' ' : return OldCube(Color::WHITE	,OldCube::AIR);
		default: {VBE_ASSERT(false, "INVALID CHARACTER " << c);}
    }
}

Map::OldCube Map::getCube(vec3f pos) {
	if(pos.x < 0 || pos.y < 0 || pos.x >= map[0].size() || pos.y >= map.size()) return OldCube(Color::WHITE,OldCube::AIR);
	return map[floor(pos.y)][floor(pos.x)];
}

void Map::setCanvasTex(std::string tex) {
	if(canvasTexture == "canvasW") canvasTexture = tex;
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
			if (map[y][i].type == OldCube::AIR || (map[y][i].color != Color::WHITE && map[y][i].color != col)) {
                x1 = float(i + 1.0);
                break;
            }
        }
        for (int i = ipos; i <= iend; i++) {
			if (map[y][i].type == OldCube::AIR || (map[y][i].color != Color::WHITE && map[y][i].color != col)) {
                x2 = float(i);
                break;
            }
        }
    }
    else {
        for (int i = ipos; i >= iini; i--) {
			OldCube::Type ctype = map[i][y].type;
			if (ctype == OldCube::AIR || ctype == OldCube::START || ctype == OldCube::FINISH
                || (map[i][y].color != Color::WHITE && map[i][y].color != col)) {
                x1 = float(i + 1.0);
                break;
            }
        }
        for (int i = ipos; i <= iend; i++) {
			OldCube::Type ctype = map[i][y].type;
			if (ctype == OldCube::AIR || ctype == OldCube::START || ctype == OldCube::FINISH
                || (map[i][y].color != Color::WHITE && map[i][y].color != col)) {
                x2 = float(i);
                break;
            }
        }
    }
}

void Map::dieAt(vec3f pos, Color col) {
	if(pos.x < 0 || pos.y < 0 || pos.x >= map[0].size() || pos.y >= map.size()) return;
    map[floor(pos.y)][floor(pos.x)].deathColor = col;
}

