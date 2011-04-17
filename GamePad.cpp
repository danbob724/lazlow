#include "GamePad.h"

SDL_Joystick *stick;
SDL_Event thing;

int minStickValue = 9000;
int maxStickValue = 32760;

int minTriggerValue = -32760;
int maxTriggerValue =  32760;

GamePad::GamePad(void)
{
        //leftStick = new AVector();
        //rightStick = new AVector();   

}//constructor

//--------------------------------------------------    
bool GamePad::initialize()
{

        if( SDL_Init(SDL_INIT_EVERYTHING) )
                return false;

        stick = SDL_JoystickOpen(0);

        if( stick == NULL )
                return false;

        return true;

}//initialize

//--------------------------------------------------
bool GamePad::isEnabled()
{

        if( stick == NULL )
                return false;

        if( !SDL_NumJoysticks() )
                return false;

        return true;

}//isEnabled    
//--------------------------------------------------
void GamePad::closeStick()
{
        SDL_JoystickClose(stick);
        SDL_Quit();

}//closeStick

//--------------------------------------------------
void GamePad::poll()
{

        if( !isEnabled() )
                return;

        float leftX, leftY, rightX, rightY;
		bumperDown = false;
		leftX = leftY = rightX = rightY = rightTrigger = 0.0f;

        if( SDL_PollEvent( &thing ) )
        {

                if( thing.type == SDL_JOYAXISMOTION )
                {

						leftX = SDL_JoystickGetAxis(stick,0);
						leftY = SDL_JoystickGetAxis(stick,1);
						if ((abs(leftX) < minStickValue) && (abs(leftY) < minStickValue)) {
							leftX = 0.0f;
							leftY = 0.0f;
						} else {
							leftX = -(float)(SDL_JoystickGetAxis(stick,0))/(float)maxStickValue;
							leftY = -(float)(SDL_JoystickGetAxis(stick,1))/(float)maxStickValue;
						}
						rightX = SDL_JoystickGetAxis(stick, 4);
						rightY = SDL_JoystickGetAxis(stick, 3);
						if ((abs(rightX) < minStickValue) && (abs(rightY) < minStickValue)) {
							rightX = 0.0f;
							rightY = 0.0f;
						} else {
							rightX = -(float)(SDL_JoystickGetAxis(stick,4))/(float)maxStickValue;
                        	rightY = -(float)(SDL_JoystickGetAxis(stick,3))/(float)maxStickValue;
						}

						rightTrigger = (float)(SDL_JoystickGetAxis(stick, 5));
						rightTrigger -= minTriggerValue;
						if (rightTrigger <= 30) {
							rightTrigger = 0.0f;
						}
						rightTrigger /= (2*maxStickValue);
						if (rightTrigger > 1) {
							rightTrigger = 1.0f;
						}

						/*
						if( leftX < ((float)minStickValue/maxStickValue) && leftX > 0-((float)minStickValue/maxStickValue) )
                                leftX = 0.0;

                        if( leftY < ((float)minStickValue/maxStickValue) && leftY > 0-((float)minStickValue/maxStickValue) )
                                leftY = 0.0;

                        if( rightX < ((float)minStickValue/maxStickValue) && rightX > 0-((float)minStickValue/maxStickValue) )
                                rightX = 0.0;

                        if( rightY < ((float)minStickValue/maxStickValue) && rightY > 0-((float)minStickValue/maxStickValue) )
                                rightY = 0.0;
						

						*/
						
                        leftStick = AVector( leftX , 0.0f , leftY );
                        rightStick = AVector( rightX , 0.0f , rightY );

                        //DO MOVING STUFF HERE!!1!

                }//if joyaxismotion
                else if( thing.type == SDL_JOYBUTTONDOWN )
                {

                        if( SDL_JoystickGetButton(stick,5) )
                        {

                          //leftY = -(float)(SDL_JoystickGetAxis(stick,1))/(float)maxStickValue;      //DO SHOOTIN' STERFF HERE...!
						  bumperDown = true;

                        }//if

                }//if joybuttondown

        }//if

}//poll

