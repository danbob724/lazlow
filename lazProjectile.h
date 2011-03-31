/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#ifndef _LAZPROJECTILE_H_
#define _LAZPROJECTILE_H_

#include "Wm5TriMesh.h"
#include "shapeMaker.h"

#define NUM_PROJECTILES 5

using namespace Wm5;

class lazProjectile
{
	public:
		lazProjectile();
		lazProjectile(ShapeMaker* someShapeMaker);
		TriMesh* mesh;
		APoint loc;
		float x_dir, z_dir;
		void Update();
		
	protected:
		ShapeMaker* myShapeMaker;
		Environment* projectileEnv;

	private:

};

#endif // _LAZPROJECTILE_H_
