/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#ifndef _PLAYERCHARACTER_H_
#define _PLAYERCHARACTER_H_

#include "shapeMaker.h"
#include "Wm5TriMesh.h"
#include "Wm5APoint.h"
#include "Wm5AVector.h"
#include "Wm5Camera.h"
#include "Wm5Environment.h"
#include "Wm5Node.h"

using namespace Wm5;

class PlayerCharacter
{
	public:
		PlayerCharacter();
		PlayerCharacter(ShapeMaker* someShapeMaker, CameraPtr someCamera);
		void setLocation(APoint desiredLocation);
		TriMesh* mMesh;
		NodePtr thePlayer;
		NodePtr theSummon;
		TriMesh* summonCube;
		APoint getLocation();
		APoint movePlayer(AVector moveVector);
		void setShotDir(AVector shootVector);

		float radius;

	protected:
		ShapeMaker* mShapeMaker;
		CameraPtr mCamera;
		unsigned char curSummon;
		APoint curLocation;
		Environment* playerCharEnv;

	private:
		
};

#endif // _PLAYERCHARACTER_H_
