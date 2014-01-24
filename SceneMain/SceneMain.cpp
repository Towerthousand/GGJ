#include "SceneMain.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "BlurContainer.hpp"
#include "SquareObject.hpp"
#include "DeferredLight.hpp"

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

    Camera* cam = new Camera("playerCam");
    cam->projection = glm::perspective(FOV, float(SCRWIDTH)/float(SCRHEIGHT), ZNEAR, ZFAR);
    cam->addTo(this);

    SquareObject* sq = new SquareObject();
    sq->pos = vec3f(0,0,-10);
    sq->addTo(renderer);

    DeferredLight* dl = new DeferredLight();
    dl->pos = vec3f(-5,5,-5);
    dl->addTo(renderer);
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
    Meshes.add("monkey", Mesh::loadFromFile("data/meshes/monkey.obj"));

	//textures
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
