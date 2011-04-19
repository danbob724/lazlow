/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "playerCharacter.h"

PlayerCharacter::PlayerCharacter() {};

PlayerCharacter::PlayerCharacter(ShapeMaker* someShapeMaker, CameraPtr someCamera) {
	mCamera = someCamera;
	curSummon = 0;
	radius = 1.4f;
	mShapeMaker = someShapeMaker;
	curLocation = APoint(0.0f, 0.0f, 0.0f);
	mMesh = someShapeMaker->CreateTorus();
	summonCube = someShapeMaker->CreateDodecahedron();
	//thePlayer->AttachChild(mMesh);
	//thePlayer->AttachChild(theSummon);
	//theSummon->AttachChild(summonCube);	
	//theSummon->LocalTransform.SetScale(APoint(0.5f, 0.5f, 0.5f));
	summonCube->LocalTransform.SetScale(APoint(0.5f, 0.5f, 0.5f));
}

void PlayerCharacter::setShotDir(AVector newDir) {
	newDir.Normalize();
	shotVector = newDir;
	shotVector *= 2;
	summonCube->LocalTransform.SetTranslate(curLocation + shotVector);
}

void PlayerCharacter::setLocation(APoint desiredLocation) {
	curLocation = desiredLocation;
	mMesh->LocalTransform.SetTranslate(curLocation);
}

APoint PlayerCharacter::movePlayer(AVector moveVector) {
	curLocation = curLocation + moveVector;
	mMesh->LocalTransform.SetTranslate(curLocation);
	summonCube->LocalTransform.SetTranslate(curLocation + shotVector);
	//mCamera->SetPosition(curLocation + AVector(0.0f, 15.f, -15.f));
	return (curLocation + AVector(0.0f, 15.f, -15.f));
}

APoint PlayerCharacter::getLocation() {
	return curLocation;
}