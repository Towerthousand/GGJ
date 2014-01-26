#include "Menu.hpp"
#include "SceneMain/SceneMain.hpp"

Menu::Menu()
{
	m.program = ShaderProgram::loadFromFile("data/shaders/quadlol.vert", "data/shaders/motionBlurPass.frag");
	std::vector<Vertex::Element> elems = {
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3))
	};
	std::vector<vec3f> data = {
		vec3f(1, -1, 0), vec3f(1, 1, 0), vec3f(-1, 1, 0),
		vec3f(-1, 1, 0), vec3f(-1, -1, 0), vec3f(1, -1, 0)
	};
	m.mesh = Mesh::loadEmpty(Vertex::Format(elems));
	m.mesh->setVertexData(&data[0], 6);
	m.mesh->setPrimitiveType(Mesh::TRIANGLES);

	tex = Texture2D::createFromFile("data/textures/menu.png");

	AudioManager::loadMusic("menuMusic", "data/music/menu1.ogg");
	AudioManager::getMusic("menuMusic")->setLoop(true);
	AudioManager::getMusic("menuMusic")->play();

}

Menu::~Menu() {
	delete m.mesh;
	delete m.program;
	AudioManager::getMusic("menuMusic")->stop();
	AudioManager::eraseMusic("menuMusic");

}

void Menu::update(float deltaTime) {
	if(Input::isKeyPressed(sf::Keyboard::Space)) {
		bool multiplayer = false;
		sf::TcpSocket* socket = nullptr;

		if(multiplayer)
		{
			std::string host = "192.168.1.147";

			socket = new sf::TcpSocket();
			if(socket->connect(host, 6174) != sf::Socket::Done) {
				VBE_ASSERT(false,"Can't connect to host.");
			}

			VBE_LOG("Connected!");
			sf::Packet p;
			while(socket->receive(p) == sf::Socket::Done)
			{
				int x;
				p>>x;
				if(x == 1)
					break;
				else {
					int a, b;
					p >> a >> b;
					VBE_LOG("WAITING FOR PLAYERS ("<<a<<" OF "<<b<<")");
				}
			}
		}
		SceneMain* sc = new SceneMain(socket);
		sc->addTo(getGame());
		this->removeAndDelete();
	}
}

void Menu::draw() const {
	m.program->uniform("alpha")->set(1.0f);
	m.program->uniform("tex1")->set(tex);
	m.program->uniform("MVP")->set(mat4f(1.0f));
	m.draw();
}
