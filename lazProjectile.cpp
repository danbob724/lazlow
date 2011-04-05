/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#include "lazProjectile.h"

lazProjectile::lazProjectile() {};

lazProjectile::lazProjectile(ShapeMaker* someShapeMaker) {
	myShapeMaker = someShapeMaker;
	mesh = myShapeMaker->CreateSphere();

	loc = APoint::ORIGIN; 
	radius = 0.35f;
	x_dir = 0;
	z_dir = 0;
	
	mesh->LocalTransform.SetScale(APoint(radius, radius, radius));
	mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
}

void lazProjectile::Update() {
	//do the movement updating
	loc = loc + AVector(x_dir, 0.0, z_dir);
	mesh->LocalTransform.SetTranslate(APoint(loc));
}

int lazProjectile::active() {
	return state; //0 = inactive, 1 = active
}