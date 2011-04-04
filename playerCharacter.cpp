/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "playerCharacter.h"
#include "Wm5Environment.h"

PlayerCharacter::PlayerCharacter() {};

PlayerCharacter::PlayerCharacter(ShapeMaker* someShapeMaker) {
	curSummon = 0;
	mShapeMaker = someShapeMaker;
	curLocation = APoint(0.0f, 0.0f, 0.0f);
	mMesh = someShapeMaker->CreateTorus();
}

void PlayerCharacter::setLocation(APoint desiredLocation) {
	curLocation = desiredLocation;
	mMesh->LocalTransform.SetTranslate(curLocation);
}

APoint PlayerCharacter::getLocation() {
	return curLocation;
}