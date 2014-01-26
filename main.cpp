#include "SceneMain/SceneMain.hpp"
#include "SFML/Network.hpp"
#include "Menu.hpp"
using namespace std;

int main() {
	WINDOW_TITLE = "Deferred Test";
	ZNEAR = 0.01f;
	ZFAR = 1000.0f;
	Game* game = new Game();
	game->getWindow().setVerticalSyncEnabled(true);
    game->setFixedFramerate(60);
    Menu* menu = new Menu();
    menu->addTo(game);
	game->run();
	delete game;
	return 42;
}
