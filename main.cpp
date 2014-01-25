#include "SceneMain/SceneMain.hpp"
#include "SFML/Network.hpp"

using namespace std;

int main() {

	string host = "192.168.1.147";
	//cout<<"Enter host to connect:";
	//getline(cin, host);

	sf::TcpSocket connSocket;
	if(connSocket.connect(host, 6174) != sf::Socket::Done)
	{
		cerr<<"Can't connect to host."<<endl;
		return 1;
	}

	cerr<<"Connected!"<<endl;
	sf::Packet p;
	while(connSocket.receive(p) == sf::Socket::Done)
	{
		int x;
		p>>x;
		if(x == 1)
			break;
		else
		{
			int a, b;
			p >> a >> b;
			cout << "WAITING FOR PLAYERS ("<<a<<" OF "<<b<<")";
		}
	}

	cerr<<"STARTING GAME!"<<endl;

	WINDOW_TITLE = "Deferred Test";
	ZNEAR = 0.01f;
	ZFAR = 1000.0f;
	Game* game = new Game();
	SceneMain* sc = new SceneMain(&connSocket);
	sc->addTo(game);
	game->run();
	delete game;
	return 42;
}
