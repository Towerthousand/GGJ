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

        int playerNum;
        int playerCount;

	private:
        void loadResources();
		float debugCounter;
		int fpsCount;
		Camera* cam;
		sf::TcpSocket* socket;



		Player* players[32]; //32 players son suficientes :3

		void sendInputToServer();
		sf::Packet receivePacket();
		void receiveServerInfo();

		InputHandler input;

		std::string mapPath;

};

#endif // SCENEMAIN_HPP
