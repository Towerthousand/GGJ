#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "commons.hpp"

class Chunk;
class Camera;
class SceneMain : public GameObject {
	public:
		SceneMain(sf::Socket* socket);
		~SceneMain();
		void update(float deltaTime);

	private:
        void loadResources();
		float debugCounter;
		int fpsCount;
		Camera* cam;
		sf::Socket* socket;
};

#endif // SCENEMAIN_HPP
