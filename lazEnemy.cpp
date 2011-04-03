/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "lazEnemy.h"

lazEnemy::lazEnemy() {};

lazEnemy::lazEnemy(ShapeMaker* someShapeMaker) {
	myShapeMaker = someShapeMaker;
	mesh = myShapeMaker->CreateSphere();

	loc = APoint::ORIGIN; 
	x_dir = 0;
	z_dir = 0;
	
	//mesh->LocalTransform.SetScale(APoint(0.35f, 0.35f, 0.35f));
	mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
}

void lazEnemy::Update() {
	//do the movement updating
	loc = loc + AVector(x_dir, 0.0, z_dir);
	mesh->LocalTransform.SetTranslate(APoint(loc));
}

int lazEnemy::active() {
	return state; //0 = inactive, 1 = active
}