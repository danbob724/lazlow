/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "playerCharacter.h"

PlayerCharacter::PlayerCharacter() {}; //default constructor

PlayerCharacter::PlayerCharacter(ShapeMaker* someShapeMaker, CameraPtr someCamera) {
	//set default values
	mCamera = someCamera;
	curSummon = 0;
	radius = 1.4f;
	mShapeMaker = someShapeMaker;
	curLocation = APoint(0.0f, 0.0f, 0.0f);
	mMesh = someShapeMaker->CreateTorus();
	summonCube = someShapeMaker->CreateDodecahedron();
	summonCube->LocalTransform.SetScale(APoint(0.5f, 0.5f, 0.5f));
}

void PlayerCharacter::setShotDir(AVector newDir) {
	//set currently aimed shot direction and shot direction indicator location
	newDir.Normalize();
	shotVector = newDir;
	shotVector *= 2;
	summonCube->LocalTransform.SetTranslate(curLocation + shotVector);
}

void PlayerCharacter::setLocation(APoint desiredLocation) {
	//adjust mesh translations
	curLocation = desiredLocation;
	mMesh->LocalTransform.SetTranslate(curLocation);
}

APoint PlayerCharacter::movePlayer(AVector moveVector) {
	//increment player location by move vector
	curLocation = curLocation + moveVector;
	mMesh->LocalTransform.SetTranslate(curLocation);
	summonCube->LocalTransform.SetTranslate(curLocation + shotVector);
	return (curLocation + AVector(0.0f, 15.f, -15.f));
}

APoint PlayerCharacter::getLocation() {
	return curLocation;
}