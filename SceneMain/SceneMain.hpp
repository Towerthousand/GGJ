#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "commons.hpp"
#include "InputHandler.hpp"

class Chunk;
class Camera;
class Player;
class SceneMain : public GameObject {
	public:
		SceneMain(sf::TcpSocket* socket);
		~SceneMain();
		void update(float deltaTime);

	private:
        void loadResources();
		float debugCounter;
		int fpsCount;
		Camera* cam;
		sf::TcpSocket* socket;

		int playerNum;
		int playerCount;

		Player* players[32]; //32 players son suficientes :3

		void sendInputToServer();
		sf::Packet receivePacket();
		void receiveServerInfo();

		InputHandler input;

};

#endif // SCENEMAIN_HPP
