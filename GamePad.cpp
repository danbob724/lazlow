#include "GamePad.h"

SDL_Joystick *stick;
SDL_Event thing;

int minStickValue = 3000;
int maxStickValue = 32760;

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

        if( SDL_PollEvent( &thing ) )
        {

                if( thing.type == SDL_JOYAXISMOTION )
                {

                        leftX = -(float)(SDL_JoystickGetAxis(stick,0))/(float)maxStickValue;
                        leftY = -(float)(SDL_JoystickGetAxis(stick,1))/(float)maxStickValue;
                        rightX = -(float)(SDL_JoystickGetAxis(stick,4))/(float)maxStickValue;
                        rightY = -(float)(SDL_JoystickGetAxis(stick,3))/(float)maxStickValue;

                        if( leftX < ((float)minStickValue/maxStickValue) && leftX > 0-((float)minStickValue/maxStickValue) )
                                leftX = 0.0;

                        if( leftY < ((float)minStickValue/maxStickValue) && leftY > 0-((float)minStickValue/maxStickValue) )
                                leftY = 0.0;

                        if( rightX < ((float)minStickValue/maxStickValue) && rightX > 0-((float)minStickValue/maxStickValue) )
                                rightX = 0.0;

                        if( rightY < ((float)minStickValue/maxStickValue) && rightY > 0-((float)minStickValue/maxStickValue) )
                                rightY = 0.0;

                        leftStick = AVector( leftX , 0.0f , leftY );
                        rightStick = AVector( rightX , 0.0f , rightY );

                        //DO MOVING STUFF HERE!!1!

                }//if joyaxismotion
                else if( thing.type == SDL_JOYBUTTONDOWN )
                {

                        if( SDL_JoystickGetButton(stick,5) )
                        {

                                //DO SHOOTIN' STERFF HERE...!

                        }//if

                }//if joybuttondown

        }//if

}//poll

