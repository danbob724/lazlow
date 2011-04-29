/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * Copyright (C)  2011 <>
 * 
 */

#ifndef _SHAPEMAKER_H_
#define _SHAPEMAKER_H_

#include "SimpleBumpMapEffect.h"
#include "Wm5TriMesh.h"
#include "Wm5Environment.h"
#include "Wm5StandardMesh.h"
#include "Wm5Texture2DEffect.h"

using namespace Wm5;

class ShapeMaker
{
	public:
		ShapeMaker();
		ShapeMaker(bool useBumps);
		void setBumpMap(bool useBumps);

		TriMesh* CreateTorus();
		TriMesh* CreateCylinder();
		TriMesh* CreateSphere();
		TriMesh* CreateCube();
		TriMesh* CreateRectangle();
		TriMesh* CreateDodecahedron();

		TriMesh* CreateTitle();
		TriMesh* CreateEnding();
		TriMesh* CreateStory();
		TriMesh* CreateControls();
		TriMesh* CreateJoke();

	protected:
		Environment* shapeMakerEnv;
		AVector mLightDirection;
		APoint mLightPosition;
		bool mUseBumpMap;

	private:

};

#endif // _SHAPEMAKER_H_
