/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * Copyright (C)  2011 <>
 * 
 */

#include "Wm5TriMesh.h"
#include "Wm5Camera.h"
#include "Wm5Environment.h"
#include "Wm5Texture2DEffect.h"

#ifndef _NARRATIVESLIDES_H_
#define _NARRATIVESLIDES_H_

using namespace Wm5;

class narrativeSlides
{
public:
	narrativeSlides();
	CameraPtr mScreenCamera;
	void buildCurSlide();
protected:
	TriMeshPtr mForePoly;
private:

};

#endif // _NARRATIVESLIDES_H_
