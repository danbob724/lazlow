#ifndef GAMEPAD_H
#define GAMEPAD_H

#ifdef WIN32
    #include <direct.h>
	#include <SDL.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
	#include <SDL/SDL.h>
    #define GetCurrentDir getcwd
#endif

#include "Wm5AVector.h"
using namespace Wm5;

class GamePad
{

protected:
	//float leftX;
	//float leftY;
	//float rightX;
	//float rightY;

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
		float rightTrigger;
		bool bumperDown;
		bool startButtonDown;
};

#endif

