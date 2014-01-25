#include "ParticleSystem.hpp"
#include "SceneMain/Camera.hpp"
#include "SceneMain/DeferredContainer.hpp"

ParticleSystem::ParticleSystem() : textureCount(0), textureSheet(nullptr) {

	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");

	std::vector<Vertex::Element> elements;
	elements.push_back(Vertex::Element(Vertex::Attribute::Position   , Vertex::Element::Float, 3));
	elements.push_back(Vertex::Element(Vertex::Attribute::get("a_vel"), Vertex::Element::Float, 3));
	elements.push_back(Vertex::Element(Vertex::Attribute::Color      , Vertex::Element::Float, 4));
	elements.push_back(Vertex::Element(Vertex::Attribute::get("a_size"), Vertex::Element::Float, 1));
	elements.push_back(Vertex::Element(Vertex::Attribute::get("a_texIndex"), Vertex::Element::Int, 1));

	model.mesh = Mesh::loadEmpty(Vertex::Format(elements), Mesh::STREAM);
	model.mesh->setPrimitiveType(Mesh::POINTS);
	model.program = ShaderProgram::loadFromFile("data/shaders/particles.vert", "data/shaders/particles.geom", "data/shaders/particles.frag");
	setName("particleSystem");
	setUpdatePriority(-100);
	setDrawPriority(100);
}

ParticleSystem::~ParticleSystem() {
	delete model.mesh;
	delete model.program;
}

void ParticleSystem::update(float deltaTime) {
	for(std::list<Particle>::iterator it = particles.begin(); it != particles.end();) {
		it->update(deltaTime);
		if(!it->isAlive())
			it = particles.erase(it);
		else
			++it;
	}
	std::vector<Particle::Vertex> vtxs;
	vtxs.resize(particles.size());
	int i = 0;
	for(std::list<Particle>::const_iterator it = particles.begin(); it != particles.end(); it++) {
		it->render(vtxs[i]);
		i++;
	}
	model.mesh->setVertexData(&vtxs[0], particles.size());

}

void ParticleSystem::draw() const {
	if(particles.size() == 0)
		return;
	VBE_ASSERT(textureSheet != nullptr, "Cannot draw textureless particles, give particleSystem a textureSheet before emitting particles");

	if(renderer->getMode() != DeferredContainer::Forward)
		return;

	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	if(!Input::isKeyDown(sf::Keyboard::P)) {
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model.program->uniform("modelViewMatrix")->set(cam->view*fullTransform);
		model.program->uniform("projectionMatrix")->set(cam->projection);
		model.program->uniform("texSize")->set(1.0f/float(textureCount));
		model.program->uniform("textureSheet")->set(textureSheet);
		model.program->uniform("depth")->set(renderer->getDepth());
		model.program->uniform("invResolution")->set(vec2f(1.0f/SCRWIDTH, 1.0f/SCRHEIGHT));
		model.program->uniform("invProj")->set(glm::inverse(cam->projection));
		model.draw();
		glDepthMask(GL_TRUE);
	}
}

void ParticleSystem::addParticle(const Particle& p) {
	particles.push_back(p);
}

void ParticleSystem::setTextureSheet(Texture2D* textureSheet, unsigned int textureCount) {
	this->textureSheet = textureSheet;
	this->textureCount = textureCount;
}
