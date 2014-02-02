#include "Map.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "SceneMain.hpp"
#include "CubeCommon.hpp"

//std::string Map::models_textures[Map::OldCube::NUM_TYPES][Color::NUM_COLORS][2] = {
//	{ //AIR
//	  {"",""},
//	  {"",""},
//	  {"",""},
//	  {"",""}
//	},
//	{ //FLOOR
//	  {"cube","nullWhite"},
//	  {"cube","nullRed"},
//	  {"cube","nullGreen"},
//	  {"cube","nullBlue"}
//	},
//	{ //SAW
//	  {"sawCube","nullWhite"},
//	  {"sawCube","nullRed"},
//	  {"sawCube","nullGreen"},
//	  {"sawCube","nullBlue"}
//	},
//	{ //BUMP
//	  {"botCube","nullWhite"},
//	  {"botCube","nullRed"},
//	  {"botCube","nullGreen"},
//	  {"botCube","nullBlue"}
//	},
//	{ //START
//	  {"",""},
//	  {"pote","poteR"},
//	  {"pote","poteG"},
//	  {"pote","poteB"}
//	},
//	{ //FINISH
//	  {"canvas","canvasW"},
//	  {"",""},
//	  {"",""},
//	  {"",""}
//	}
//};

Map::Map() {
	setName("map");
}

Map::~Map() {
}

void Map::loadFromFile(const std::string& mapfile) {
	std::ifstream in(mapfile.c_str(), std::ifstream::in);
	VBE_ASSERT(in, "While parsing map: Cannot open " << mapfile );
	std::vector<std::vector<char> > inputMap = std::vector<std::vector<char> >(1,std::vector<char>());
	char c = 'x';
	int i = 0;
	while(1) {
		in >> std::noskipws >> c;
		if(c == '%') break;
		if(c == '\n') {
			inputMap.push_back(std::vector<char>());
			++i;
		}
		else {
			inputMap[i].push_back(c);
		}
	}
	std::reverse(inputMap.begin(),inputMap.end());
	map = std::vector<std::vector<Cube*> >(inputMap[0].size(), std::vector<Cube*>(inputMap.size()));
	for(unsigned int y = 0; y < inputMap.size(); y++) {
		for(unsigned int x = 0; x < inputMap[0].size(); x++) {
			map[x][y] = translate(inputMap[y][x],vec2i(x,y));
			if(map[x][y] == nullptr) continue;
			map[x][y]->addTo(this);
		}
	}
	for(int i = 0; i < 3; ++i) {
		startingPos[i] = vec2f(0,0);
	}
}

void Map::update(float deltaTime) {
	(void) deltaTime;
}

void Map::draw() const {
//	SceneMain* sc = (SceneMain*)getGame()->getObjectByName("SCENE");
//	Color playerColor = (Color)(sc->playerNum + 1);
//	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
//	if(renderer->getMode() == DeferredContainer::Deferred) {
//		for(int i = 0; i < (int)map.size(); ++i) {
//			for(int j = 0; j < (int)map[0].size(); ++j) {
//				OldCube current = map[i][j];
//				if(current.type == OldCube::FINISH) {
//					float rot = -30.0f;
//					mat4f mat = fullTransform;
//					mat = glm::translate(mat,vec3f(j,i,0));
//					mat = glm::rotate(mat,rot,vec3f(0,1.0f,0));
//					cube.program->uniform("MVP")->set(cam->projection*cam->view*mat);
//					cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
//					cube.program->uniform("V")->set(cam->view);
//					cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
//					cube.program->uniform("diffuseTex")->set(Textures2D.get(canvasTexture));
//					cube.draw();
//					continue;
//				}
//				else if(current.type == OldCube::START) {
//					cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.2)));
//					cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.2)));
//					cube.program->uniform("V")->set(cam->view);
//					cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
//					cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[current.type][current.color][1]));
//					cube.draw();
//					continue;
//				}
//				else if(current.type == OldCube::SAW) {
//					if(current.color == playerColor || current.color == Color::WHITE) {
//						cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.5)));
//						cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.5)));
//						cube.program->uniform("V")->set(cam->view);
//						cube.program->uniform("ambient")->set(0.5f);
//						cube.program->uniform("specular")->set(1.0f);
//						cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
//						cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[current.type][current.color][1]));
//						cube.draw();
//					}
//					if(current.color == playerColor || current.color == Color::WHITE  || current.deathColor != Color::WHITE) {
//						cube.program = Programs.get("deferredSaw");
//						cube.program->uniform("ambient")->set(0.5f);
//						cube.program->uniform("specular")->set(1.0f);
//						float rot = GLOBALCLOCK.getElapsedTime().asSeconds()*10000;
//						cube.program->uniform("MVP")->
//								set(cam->projection*cam->view*
//									glm::translate(glm::rotate(glm::translate(fullTransform,vec3f(j,i+1,0)),rot,vec3f(1,0,0)),vec3f(0,-1,0.5)));
//						cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0)));
//						cube.program->uniform("colorID")->set(current.deathColor);
//						cube.mesh = Meshes.get("saw");
//						cube.program->uniform("diffuseTex")->set(Textures2D.get("saw"));
//						cube.draw();
//						cube.program = Programs.get("deferredModel");
//					}
//					continue;
//				}
//				if(current.type == OldCube::AIR || (playerColor != current.color && current.color != Color::WHITE)) continue;
//				cube.program = Programs.get("deferredCubes");
//				cube.program->uniform("MVP")->set(cam->projection*cam->view*glm::translate(fullTransform,vec3f(j,i,0.5)));
//				cube.program->uniform("M")->set(glm::translate(fullTransform,vec3f(j,i,0.5)));
//				cube.program->uniform("V")->set(cam->view);
//				cube.program->uniform("ambient")->set(0.5f);
//				cube.program->uniform("specular")->set(1.0f);
//				cube.mesh = Meshes.get(models_textures[current.type][current.color][0]);
//				cube.program->uniform("diffuseTex")->set(Textures2D.get(models_textures[current.type][current.color][1]));
//				cube.program->uniform("normalsTex")->set(Textures2D.get("normalsCubes"));
//				cube.draw();
//				cube.program = Programs.get("deferredModel");
//			}
//		}
//	}
//	else if (renderer->getMode() == DeferredContainer::Forward) {
//		for(int i = 0; i < (int)map.size(); ++i) {
//			for(int j = 0; j < (int)map[0].size(); ++j) {
//				if(map[i][j].type == OldCube::AIR || map[i][j].type == OldCube::FINISH || map[i][j].type == OldCube::START) continue;
//				Model m;
//				m.mesh = Meshes.get("1x1WireCube");
//				m.program = Programs.get("lines");
//				m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
//				m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(glm::translate(mat4f(1.0f),vec3f(j+0.5,i+0.5,0)),vec3f(0.5f)));
//				m.draw();
//			}
//		}
//	}
}

bool Map::isColliding(const vec3f& pos, Color &color) const {
	Cube* c = getCube(pos);
	if(c != nullptr && c->isColliding(pos)) {
		color = c->getColor();
		return true;
	}
	return false;
}

bool Map::isColliding(const AABB& aabb, Color &color) const {
	int xmin = floor(aabb.getMin().x);
	int xmax = floor(aabb.getMax().x);
	int ymin = floor(aabb.getMin().y);
	int ymax = floor(aabb.getMax().y);

	for (int i = xmin; i <= xmax && i < (int)map.size(); i++) {
		if(i < 0) continue;
		for (int j = ymin; j <= ymax && j < (int)map[i].size(); j++) {
			if(j < 0) continue;
			Cube* c = getCube(i,j);
			if (c == nullptr) continue;
			if (c->isColliding(aabb))  {
				color = c->getColor();
				return true;
			}
		}
	}
	return false;
}

Cube* Map::translate(char c, vec2i pos) const {
	switch (c) {
		case 'W' : return new CubeCommon(WHITE, pos);
		case 'R' : return new CubeCommon(WHITE, pos);
		case 'G' : return new CubeCommon(WHITE, pos);
		case 'B' : return new CubeCommon(WHITE, pos);
		case '<' : return new CubeCommon(WHITE, pos);
		case 'Z' : return new CubeCommon(WHITE, pos);
		case 'X' : return new CubeCommon(WHITE, pos);
		case 'C' : return new CubeCommon(WHITE, pos);
		case 'J' : return new CubeCommon(WHITE, pos);
		case 'K' : return new CubeCommon(WHITE, pos);
		case 'L' : return new CubeCommon(WHITE, pos);
		case 'P' : return new CubeCommon(WHITE, pos);
		case 'A' : return new CubeCommon(WHITE, pos);
		case 'S' : return new CubeCommon(WHITE, pos);
		case 'D' : return new CubeCommon(WHITE, pos);
		case 'M' : return new CubeCommon(WHITE, pos);
		case ' ' : return (Cube*)nullptr;
		default: {VBE_ASSERT(false, "INVALID CHARACTER " << c);}
    }
}

Cube* Map::getCube(int x, int y) const {
	if(x < 0 || y < 0 || x >= (int)map.size() || y >= (int)map[0].size()) return nullptr;
	return map[x][y];
}

void Map::setCanvasTex(std::string tex) {
}

void Map::clipTrail(Color col, bool horizontal, int y, float &x1, float &x2) {
	float pos = 0.5f*(x1 + x2);
	int xdim = horizontal ? map.size() : map[0].size();
	int ipos = floor(glm::clamp(pos, 0.0f, float(xdim) - 0.1f));
	x1 = glm::clamp(x1,  0.0f, float(xdim) - 0.1f);
	int iini = floor(x1);
	x2 = glm::clamp(x2,  0.0f, float(xdim) - 0.1f);
	int iend = floor(x2);

	if (horizontal) {
		for (int i = ipos; i >= iini; i--) {
			Cube* c = getCube(i,y);
			if (c == nullptr || !c->isPaintable() || (c->getColor() != WHITE && c->getColor() != col)) {
				x1 = float(i + 1.0);
				break;
			}
		}
		for (int i = ipos; i <= iend; i++) {
			Cube* c = getCube(i,y);
			if (c == nullptr || !c->isPaintable() || (c->getColor() != WHITE && c->getColor() != col)) {
				x2 = float(i);
				break;
			}
		}
	}
	else {
		for (int i = ipos; i >= iini; i--) {
			Cube* c = getCube(y,i);
			if (c == nullptr || !c->isPaintable() || (c->getColor() != WHITE && c->getColor() != col)) {
				x1 = float(i + 1.0);
				break;
			}
		}
		for (int i = ipos; i <= iend; i++) {
			Cube* c = getCube(y,i);
			if (c == nullptr || !c->isPaintable() || (c->getColor() != WHITE && c->getColor() != col)) {
				x2 = float(i);
				break;
			}
		}
	}
}

void Map::dieAt(vec3f pos, Color col) {
	//if(pos.x < 0 || pos.y < 0 || pos.x >= map[0].size() || pos.y >= map.size() || getCube(pos) == nullptr) return;
	//getCube(pos)->deathColor = col;
}

