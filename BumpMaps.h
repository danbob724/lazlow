// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef BUMPMAPS_H
#define BUMPMAPS_H

#include "Wm5WindowApplication3.h"
#include "SimpleBumpMapEffect.h"
#include "shapeMaker.h"
#include "playerCharacter.h"
using namespace Wm5;

struct Projectile
{
	Wm5::TriMesh * mesh;
	float x_loc, z_loc, x_dir, z_dir;
} ;
#define NUM_PROJECTILES 5


class BumpMaps : public WindowApplication3
{
    WM5_DECLARE_INITIALIZE;
    WM5_DECLARE_TERMINATE;

public:
    BumpMaps ();

    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnIdle ();
    virtual bool OnKeyDown (unsigned char key, int x, int y);
	virtual bool OnKeyUp (unsigned char key, int x, int y);
	virtual bool OnMouseClick (int button, int state, int x, int y, unsigned int modifiers);
	virtual void InitializeCameraMotion (float trnSpeed, float rotSpeed, float trnSpeedFactor, float rotSpeedFactor);

protected:
    void CreateScene ();
    void UpdateBumpMap ();

	ShapeMaker mShapeMaker;
	std::string getRealPath();
    NodePtr mScene;
    Culler mCuller;
    AVector mLightDirection;
	APoint mLightPosition;
    bool mUseTorus;
    bool mUseBumpMap;
    Float4 mTextColor;
//////////////////////
	PlayerCharacter thePlayer;
	TriMesh* terrain;
	//APoint playerLocation;
	Projectile projectiles[NUM_PROJECTILES];

//////////////////////////
	Picker mPicker;
	char mPickMessage[1024];
};

WM5_REGISTER_INITIALIZE(BumpMaps);
WM5_REGISTER_TERMINATE(BumpMaps);

#endif
