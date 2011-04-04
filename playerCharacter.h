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

using namespace Wm5;

class PlayerCharacter
{
	public:
		PlayerCharacter();
		PlayerCharacter(ShapeMaker* someShapeMaker, CameraPtr someCamera);
		void setLocation(APoint desiredLocation);
		TriMesh* mMesh;
		APoint getLocation();
		APoint movePlayer(AVector moveVector);

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
