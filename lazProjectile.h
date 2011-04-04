/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#ifndef _LAZPROJECTILE_H_
#define _LAZPROJECTILE_H_

#include "Wm5TriMesh.h"
#include "shapeMaker.h"

using namespace Wm5;

class lazProjectile
{
	public:
		lazProjectile();
		lazProjectile(ShapeMaker* someShapeMaker);
		int active();

		TriMesh* mesh;
		APoint loc;
		int state; //0 = inactive, 1 = active
		float x_dir, z_dir, radius;
		void Update();
		
	protected:
		ShapeMaker* myShapeMaker;
		Environment* projectileEnv;

	private:

};

#endif // _LAZPROJECTILE_H_
