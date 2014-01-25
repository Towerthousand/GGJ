#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "Particle.hpp"

class DeferredContainer;
class ParticleSystem : public GameObject {
	public:
		ParticleSystem();
		~ParticleSystem();
		void update(float deltaTime);
		void draw() const;
		void addParticle(const Particle& p);
		int getParticleCount() const { return particles.size(); }
		void setTextureSheet(Texture2D* textureSheet, unsigned int textureCount);
	private:
		DeferredContainer* renderer;
		std::list<Particle> particles;
		Model model;
		unsigned int textureCount;
		Texture2D* textureSheet;
};

#endif // PARTICLESYSTEM_HPP
