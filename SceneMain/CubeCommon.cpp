#include "CubeCommon.hpp"
#include "Camera.hpp"
#include "DeferredContainer.hpp"
#include "SceneMain.hpp"

const std::string CubeCommon::textures[NUM_COLORS] = {
	"nullWhite",
	"nullRed",
	"nullGreen",
	"nullBlue"
};

CubeCommon::CubeCommon(Color color, vec2i pos) : Cube(pos) {
	this->color = color;
	cubeModel.mesh = Meshes.get("cube");
	cubeModel.program = Programs.get("deferredCubes");
}

CubeCommon::~CubeCommon() {
}

void CubeCommon::update(float deltaTime) {
	(void) deltaTime;
	transform = glm::translate(mat4f(1.0f),vec3f(pos.x,pos.y,0.5));
}

void CubeCommon::draw() const {
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	SceneMain* sc = (SceneMain*)getGame()->getObjectByName("SCENE");
	Color playerColor = (Color)(sc->playerNum + 1);
#ifdef __DEBUG
	if (renderer->getMode() == DeferredContainer::Forward) { //draw bounding Box
		AABB aabb = cubeModel.mesh->getBoundingBox();
		aabb = AABB(vec3f(fullTransform*vec4f(aabb.getMin(),1)),vec3f(fullTransform*vec4f(aabb.getMax(),1)));
		Model m;
		m.mesh = Meshes.get("1x1WireCube");
		m.program = Programs.get("lines");
		m.program->uniform("MVP")->set(cam->projection*cam->view*glm::scale(glm::translate(mat4f(1.0f),aabb.getCenter()),aabb.getDimensions()/2.0f));
		m.program->uniform("lineColor")->set(vec4f(1, 0, 0, 1));
		m.draw();
	}
#endif
	if(renderer->getMode() == DeferredContainer::Deferred && (playerColor == color || color == WHITE)) { //draw cube itself
		cubeModel.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform);
		cubeModel.program->uniform("M")->set(fullTransform);
		cubeModel.program->uniform("V")->set(cam->view);
		cubeModel.program->uniform("ambient")->set(0.5f);
		cubeModel.program->uniform("specular")->set(1.0f);
		cubeModel.program->uniform("diffuseTex")->set(Textures2D.get(textures[color]));
		cubeModel.program->uniform("normalsTex")->set(Textures2D.get("normalsCubes"));
		cubeModel.draw();
	}
}

bool CubeCommon::isColliding(vec3f p) const {
	return (floor(p.x) == pos.x || floor(p.y) == pos.y);
}

bool CubeCommon::isColliding(AABB box) const {
	AABB aabb = cubeModel.mesh->getBoundingBox();
	aabb = AABB(vec3f(fullTransform*vec4f(aabb.getMin(),1)),vec3f(fullTransform*vec4f(aabb.getMax(),1)));
	return aabb.overlap(box);
}

bool CubeCommon::isPaintable() const {
	return true;
}
