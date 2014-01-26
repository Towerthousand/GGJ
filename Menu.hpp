#ifndef MENU_HPP
#define MENU_HPP
#include "commons.hpp"
/*
cargar modelo quad
quad.program
poner textura*/
class Menu : public GameObject {
	public:
		Menu();
		virtual ~Menu();
		void update(float deltaTime);
		void draw() const;

	private:
		void startGame();
		Model m;
		Texture2D* tex1;
		Texture2D* tex2;
		sf::TcpSocket* socket;
};

#endif // MENU_HPP
