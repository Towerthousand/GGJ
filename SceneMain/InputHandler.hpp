#ifndef InputHandlerINE_H
#define InputHandlerINE_H

#include <SFML/Graphics.hpp>

class InputHandler
{
public:
	enum Key
	{
		PLAYER_UP, PLAYER_DOWN, PLAYER_LEFT, PLAYER_RIGHT,
		K_SIZE
	};

	InputHandler();

	void Update();

	bool getKeyState(int key);
	bool getKeyUp(int key);
	bool getKeyDown(int key);

    std::string encodeToString();
    void decodeFromString(std::string str);

    bool firstUpdate;

private:

    bool KeyState[K_SIZE];
    bool KeyStateOld[K_SIZE];

    sf::Keyboard::Key KeyMap[K_SIZE];
};

#endif // InputHandlerINE_H
