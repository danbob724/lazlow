/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "playerCharacter.h"
#include "Wm5Environment.h"

PlayerCharacter::PlayerCharacter() {};

PlayerCharacter::PlayerCharacter(ShapeMaker* someShapeMaker, CameraPtr someCamera) {
	mCamera = someCamera;
	curSummon = 0;
	mShapeMaker = someShapeMaker;
	curLocation = APoint(0.0f, 0.0f, 0.0f);
	mMesh = someShapeMaker->CreateTorus();
}

void PlayerCharacter::setLocation(APoint desiredLocation) {
	curLocation = desiredLocation;
	mMesh->LocalTransform.SetTranslate(curLocation);
}

APoint PlayerCharacter::movePlayer(AVector moveVector) {
	curLocation = curLocation + moveVector;
	mMesh->LocalTransform.SetTranslate(curLocation);
	//mCamera->SetPosition(curLocation + AVector(0.0f, 15.f, -15.f));
	return (curLocation + AVector(0.0f, 15.f, -15.f));
}

APoint PlayerCharacter::getLocation() {
	return curLocation;
}