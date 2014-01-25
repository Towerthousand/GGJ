#include <iostream>
#include "InputHandler.hpp"

InputHandler::InputHandler()
{
	KeyMap[PLAYER_UP] = sf::Keyboard::Up;
	KeyMap[PLAYER_DOWN] = sf::Keyboard::Down;
	KeyMap[PLAYER_LEFT] = sf::Keyboard::Left;
	KeyMap[PLAYER_RIGHT] = sf::Keyboard::Right;

	for (int i = 0; i < K_SIZE; ++i)
		KeyState[i] = KeyStateOld[i] = false;

	firstUpdate = true;
}

std::string InputHandler::encodeToString() {

	std::string encoded(K_SIZE, ' ');

	for (int i = 0; i < K_SIZE; ++i)
		encoded[i] = KeyState[i] ? '1' : '0';

	return encoded;
}

void InputHandler::decodeFromString(std::string encoded) {

	for (int i = 0; i < K_SIZE; ++i)
	{
		KeyStateOld[i] = KeyState[i];
		KeyState[i] = encoded[i] == '1';
	}

	if(firstUpdate)
		for (int i = 0; i < K_SIZE; ++i)
			KeyStateOld[i] = KeyState[i];
	firstUpdate = false;
}

void InputHandler::Update()
{
	for (int i = 0; i < K_SIZE; i++)
	{
		KeyStateOld[i] = KeyState[i];
		KeyState[i] = sf::Keyboard::isKeyPressed(KeyMap[i]);
	}

	if(firstUpdate)
		for (int i = 0; i < K_SIZE; ++i)
			KeyStateOld[i] = KeyState[i];

	firstUpdate = false;
}

bool InputHandler::getKeyState(int key) {
	return KeyState[key];
}

bool InputHandler::getKeyUp(int key) {
	return KeyStateOld[key] && !KeyState[key];
}

bool InputHandler::getKeyDown(int key) {
	return !KeyStateOld[key] && KeyState[key];
}
