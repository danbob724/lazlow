#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "SDL.h"
#include "Wm5AVector.h"
using namespace Wm5;

class GamePad
{

public:

	GamePad(void);

	//Initializes SDL and connects joystick
	bool initialize();

	//Check if gamepads are enabled and connected.
	bool isEnabled();
	
	//Closes the joystick and quits SDL.
	void closeStick();

	//Polls the joystick, updating leftStick and rightStick,
	//then shooting/moving as necessary.
	void poll();

	AVector leftStick;
	AVector rightStick;
};

#endif