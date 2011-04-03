/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * 
 */

#ifndef _LAZENEMY_H_
#define _LAZENEMY_H_

#include "Wm5TriMesh.h"
#include "shapeMaker.h"

#define NUM_ENEMIES 5

using namespace Wm5;

class lazEnemy
{
	public:
		lazEnemy();		
		lazEnemy(ShapeMaker* someShapeMaker);
		int active();

		TriMesh* mesh;
		APoint loc;
		float x_dir, z_dir;
		int state; //0 = inactive, 1 = active
		void Update();
		
	protected:
		ShapeMaker* myShapeMaker;
		Environment* enemyEnv;

	private:

};

#endif // _LAZENEMY_H_
