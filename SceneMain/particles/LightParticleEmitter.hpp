#ifndef LIGHTPARTICLEEMITTER_HPP
#define LIGHTPARTICLEEMITTER_HPP
#include "ParticleEmitter.hpp"

class LightParticleEmitter : public ParticleEmitter
{
	public:
        LightParticleEmitter(vec3f col, int index);
		virtual Particle makeParticle(float frameTimePassed, float deltaTime, vec3f position);

	private:
		vec3f col;
        int index;
};

#endif // LIGHTPARTICLEEMITTER_HPP
