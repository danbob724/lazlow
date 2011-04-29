/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "lazEnemy.h"

lazEnemy::lazEnemy() {};

lazEnemy::lazEnemy(ShapeMaker* someShapeMaker) {
	myShapeMaker = someShapeMaker;
	mesh = myShapeMaker->CreateCylinder();

	loc = APoint::ORIGIN; 
	health = 0;
	radius = 0.5;
	x_dir = 0;
	z_dir = 0;
	state = 0;
	behavior = 0;
	
	//mesh->LocalTransform.SetScale(APoint(0.35f, 0.35f, 0.35f));
	mesh->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
}

//This updates the enemy location
void lazEnemy::Update() {
	//do the movement updating
	loc = loc + AVector(x_dir, 0.0f, z_dir);
	mesh->LocalTransform.SetTranslate(APoint(loc));
}

//This checks if the enemy is active
int lazEnemy::active() {
	return state; //0 = inactive, 1 = active
}

//This sets the enemy active state
void lazEnemy::setState(int newState) {
	state = newState;
}

//This adjusts the enemy health on a spell hit
void lazEnemy::hit(int hitValue) {
	health -= hitValue;
	if (health <= 0) {
		state = 0;
	}
}

//Returns enemy health
int lazEnemy::getCurrentHealth() {
	return health;
}

//Sets enemy health
void lazEnemy::setHealth(int healthIn){
	health = healthIn;
}

