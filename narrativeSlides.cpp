/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * Copyright (C)  2011 <>
 * 
 */

#include "narrativeSlides.h"


narrativeSlides::narrativeSlides() {
	//setup the private "screen camera"
	// The screen camera is designed to map (x,y,z) in [0,1]^3 to (x',y,'z')
	// in [-1,1]^2 x [0,1].
	mScreenCamera = new0 Camera(false);
	mScreenCamera->SetFrustum(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	mScreenCamera->SetFrame(APoint::ORIGIN, AVector::UNIT_Z, AVector::UNIT_Y, AVector::UNIT_X);
}

void narrativeSlides::buildCurSlide() {
	std::string path;
	Texture2D* texture;
	VertexFormat* vformat = VertexFormat::Create(2, VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0, VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    int vstride = vformat->GetStride();

    VertexBuffer* vbuffer;
    VertexBufferAccessor vba(vformat, vbuffer);
	vbuffer = new0 VertexBuffer(5, vstride);

	IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
    int* indices = (int*)ibuffer->GetData();

	vba.ApplyTo(vformat, vbuffer);
    vba.Position<Float3>(0) = Float3(0.0f,  0.0f,  0.0f);
    vba.Position<Float3>(1) = Float3(0.5f,  0.0f,  0.0f);
    vba.Position<Float3>(2) = Float3(0.75f, 0.5f,  0.0f);
    vba.Position<Float3>(3) = Float3(0.5f,  0.75f, 0.0f);
    vba.Position<Float3>(4) = Float3(0.0f,  0.5f,  0.0f);
    vba.TCoord<Float2>(0, 0) = Float2(0.0f,  0.0f);
    vba.TCoord<Float2>(0, 1) = Float2(0.67f, 0.0f);
    vba.TCoord<Float2>(0, 2) = Float2(1.0f,  0.67f);
    vba.TCoord<Float2>(0, 3) = Float2(0.67f, 1.0f);
    vba.TCoord<Float2>(0, 4) = Float2(0.0f,  0.67f);

    ibuffer = new0 IndexBuffer(9, sizeof(int));
    indices = (int*)ibuffer->GetData();
    indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
    indices[3] = 0;  indices[4] = 2;  indices[5] = 3;
    indices[6] = 0;  indices[7] = 3;  indices[8] = 4;

    mForePoly = new0 TriMesh(vformat, vbuffer, ibuffer);
    path = Environment::GetPathR("Flower.wmtf");
    Texture2DEffect* foreEffect = new0 Texture2DEffect(Shader::SF_LINEAR);
    texture = Texture2D::LoadWMTF(path);
    mForePoly->SetEffectInstance(foreEffect->CreateInstance(texture));

    // Make the foreground semitransparent.
    foreEffect->GetAlphaState(0, 0)->BlendEnabled = true;
}