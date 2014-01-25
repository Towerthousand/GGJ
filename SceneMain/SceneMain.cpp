#include "SceneMain.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "DeferredContainer.hpp"
#include "BlurContainer.hpp"
#include "SquareObject.hpp"
#include "DeferredLight.hpp"
#include "Map.hpp"
#include "particles/ParticleSystem.hpp"
#include "particles/LightParticleEmitter.hpp"
#include "Trails.hpp"

SceneMain::SceneMain() : debugCounter(0.0), fpsCount(0) {
	this->setName("SCENE");

	loadResources();
	srand(GLOBALCLOCK.getElapsedTime().asMilliseconds());

	//GL stuff..:
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE); //enable backface culling
	glCullFace(GL_BACK);

	//getGame()->getWindow().setVerticalSyncEnabled(true);

	BlurContainer* blur = new BlurContainer();
	blur->addTo(this);

	DeferredContainer* renderer = new DeferredContainer();
	renderer->addTo(blur);

    Player* pla = new Player("playerTest");
    pla->pos = vec3f(3,5,0);
    pla->addTo(renderer);

	Map* map = new Map("data/maps/map0.map");
	map->addTo(renderer);

    Trails* trails = new Trails("trails");
    trails->addTo(renderer);

    DeferredLight* dl = new DeferredLight();
	dl->pos = vec3f(10,5,10);
	dl->color = vec3f(1);
    dl->addTo(renderer);

	ParticleSystem* sys = new ParticleSystem();
	sys->addTo(renderer);
	sys->setTextureSheet(Textures2D.get("particleSheet"), 2);

    ParticleEmitter* emitter1 = new LightParticleEmitter(vec3f(1.0, 0.1, 0.03));
	emitter1->addTo(pla);
}

SceneMain::~SceneMain() {
	Textures2D.clear();
	Meshes.clear();
	Programs.clear();
	AudioManager::clear();
}

void SceneMain::loadResources() {

	//meshes
	std::vector<Vertex::Element> elems = {
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3))
	};
	std::vector<vec3f> data = {
		vec3f(1, -1, 0), vec3f(1, 1, 0), vec3f(-1, 1, 0),
		vec3f(-1, 1, 0), vec3f(-1, -1, 0), vec3f(1, -1, 0)
	};
	Mesh* quad = Mesh::loadEmpty(Vertex::Format(elems));
	quad->setVertexData(&data[0], 6);
	quad->setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad", quad);
	std::vector<vec3f> cubeVertices = {
		vec3f(-1.0, -1.0, 1.0),
		vec3f(1.0, -1.0, 1.0),
		vec3f(-1.0, 1.0, 1.0),
		vec3f(1.0, 1.0, 1.0),
		vec3f(-1.0, -1.0, -1.0),
		vec3f(1.0, -1.0, -1.0),
		vec3f(-1.0, 1.0, -1.0),
		vec3f(1.0, 1.0, -1.0),
	};
	std::vector<unsigned int> cubeIndices = {
		0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
	};
	Mesh* cube = Mesh::loadEmpty(Vertex::Format(elems),Mesh::STATIC,true);
	cube->setPrimitiveType(Mesh::TRIANGLE_STRIP);
	cube->setVertexData(&cubeVertices[0],cubeVertices.size());
	cube->setVertexIndices(&cubeIndices[0],cubeIndices.size());
	Meshes.add("1x1Cube",cube);


	std::vector<unsigned int> wireIndices = {
		0, 1, 1, 3, 3, 2, 2, 0,     // front
		4, 5, 5, 7, 7, 6, 6, 4,     // back
		0, 4, 2, 6, 1, 5, 3, 7      // side edges
	};
	Mesh* wirecube = Mesh::loadEmpty(Vertex::Format(elems),Mesh::STATIC,true);
	wirecube->setPrimitiveType(Mesh::LINES);
	wirecube->setVertexData(&cubeVertices[0],cubeVertices.size());
	wirecube->setVertexIndices(&wireIndices[0],wireIndices.size());
	Meshes.add("1x1WireCube",wirecube);
    Meshes.add("brush", Mesh::loadFromFile("data/meshes/brush2.obj"));
	Meshes.add("cube", Mesh::loadFromFile("data/meshes/cube.obj"));
	Meshes.add("saw", Mesh::loadFromFile("data/meshes/saw.obj"));
	Meshes.add("sawCube", Mesh::loadFromFile("data/meshes/cubeS.obj"));
	Meshes.add("botCube", Mesh::loadFromFile("data/meshes/rebota1.obj"));

    //textures
	Textures2D.add("saw", Texture2D::createFromFile("data/textures/sawW.png"));
	Textures2D.add("botCubeW", Texture2D::createFromFile("data/textures/cubebotW.png"));
	Textures2D.add("botCubeR", Texture2D::createFromFile("data/textures/cubebotR.png"));
	Textures2D.add("botCubeG", Texture2D::createFromFile("data/textures/cubebotG.png"));
	Textures2D.add("botCubeB", Texture2D::createFromFile("data/textures/cubebotB.png"));
	Textures2D.add("sawCubeW", Texture2D::createFromFile("data/textures/cubeSW.png"));
	Textures2D.add("sawCubeR", Texture2D::createFromFile("data/textures/cubeSR.png"));
	Textures2D.add("sawCubeG", Texture2D::createFromFile("data/textures/cubeSG.png"));
	Textures2D.add("sawCubeB", Texture2D::createFromFile("data/textures/cubeSB.png"));
	Textures2D.add("elevatorW", Texture2D::createFromFile("data/textures/elevatorW.png"));
	Textures2D.add("elevatorR", Texture2D::createFromFile("data/textures/elevatorR.png"));
	Textures2D.add("elevatorG", Texture2D::createFromFile("data/textures/elevatorG.png"));
	Textures2D.add("elevatorB", Texture2D::createFromFile("data/textures/elevatorB.png"));
    Textures2D.add("brushR", Texture2D::createFromFile("data/textures/brushR.png"));
    Textures2D.add("brushG", Texture2D::createFromFile("data/textures/brushG.png"));
    Textures2D.add("brushB", Texture2D::createFromFile("data/textures/brushB.png"));
    Textures2D.add("particleSheet", Texture2D::createFromFile("data/textures/particleSheet.png"));
	char pixels[4] = {char(200), char(20), char(20), char(255)};
	Textures2D.add("nullRed", Texture2D::createFromRaw(pixels, 1, 1));
	char pixels2[4] = {char(20), char(200), char(20), char(255)};
	Textures2D.add("nullGreen", Texture2D::createFromRaw(pixels2, 1, 1));
	char pixels3[4] = {char(20), char(20), char(200), char(255)};
	Textures2D.add("nullBlue", Texture2D::createFromRaw(pixels3, 1, 1));
	char pixels4[4] = {char(70), char(30), char(80), char(255)};
	Textures2D.add("nullBlack", Texture2D::createFromRaw(pixels4, 1, 1));
	char pixels5[4] = {char(255), char(255), char(255), char(255)};
    Textures2D.add("nullWhite", Texture2D::createFromRaw(pixels5, 1, 1));


    //program
    Programs.add("deferredLight", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/light.frag"));
    Programs.add("deferredModel", ShaderProgram::loadFromFile("data/shaders/standardDeferred.vert", "data/shaders/standardDeferred.frag"));
	Programs.add("ambientPass", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/ambientPass.frag"));
	Programs.add("blurPassVertical", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/blurPassVertical.frag"));
	Programs.add("blurPassHoritzontal", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/blurPassHoritzontal.frag"));
	Programs.add("textureToScreen", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/quad.frag"));
	Programs.add("blurMaskPass", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/blurMaskPass.frag"));
    Programs.add("depthShader", ShaderProgram::loadFromFile("data/shaders/depth.vert","data/shaders/depth.frag"));
    Programs.add("lines", ShaderProgram::loadFromFile("data/shaders/quad.vert","data/shaders/lines.frag"));
    Programs.add("trails", ShaderProgram::loadFromFile("data/shaders/trails.vert", "data/shaders/trails.geom", "data/shaders/trails.frag"));
}

void SceneMain::update(float deltaTime) {
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		VBE_LOG("FPS: " << fpsCount);
		debugCounter--;
		fpsCount = 0;
	}
}
