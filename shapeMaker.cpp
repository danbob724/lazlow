/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lazlow
 * Copyright (C)  2011 <>
 * 
 */

#include "shapeMaker.h"

ShapeMaker::ShapeMaker() {
	mUseBumpMap = false;
	mLightDirection = AVector::UNIT_Z;
}

ShapeMaker::ShapeMaker(bool useBumps) {
	mUseBumpMap = useBumps;
	mLightDirection = AVector::UNIT_Z;
}

void ShapeMaker::setBumpMap(bool useBumps) {
	mUseBumpMap = useBumps;
}

Wm5::TriMesh* ShapeMaker::CreateDodecahedron() {
	//stuff here
	VertexFormat* vformat;

	
	if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }
	
	//vformat = VertexFormat::Create(2, VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0, VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
	
    TriMesh* mesh = StandardMesh(vformat).Dodecahedron();
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;

		if (mUseBumpMap)
        {
            Float2& tcoord1 = vba.TCoord<Float2>(1, i);
            tcoord1[0] *= 4.0f;
            tcoord1[1] *= 4.0f;
        }
		
    }
	
    std::string baseName = Environment::GetPathR("Water.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();
	

    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector::UNIT_Z;
		//mLightDirection = camDvector;
		//mLightPosition = camPosition;
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
    }
	
	/*
    Texture2DEffect* foreEffect = new0 Texture2DEffect(Shader::SF_LINEAR);
    mesh->SetEffectInstance(foreEffect->CreateInstance(baseTexture));

    // Make the foreground semitransparent.
    foreEffect->GetAlphaState(0, 0)->BlendEnabled = true;
	*/
	
	mesh->SetName("Dodecahedron");
    return mesh;
}

Wm5::TriMesh* ShapeMaker::CreateTorus ()
{
    VertexFormat* vformat;
    if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }

    TriMesh* mesh = StandardMesh(vformat).Torus(32, 32, 1.0f, 0.4f);
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;
        if (mUseBumpMap)
        {
            Float2& tcoord1 = vba.TCoord<Float2>(1, i);
            tcoord1[0] *= 4.0f;
            tcoord1[1] *= 4.0f;
        }
    }

    std::string baseName = Environment::GetPathR("Bricks.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();

	
    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector::UNIT_Z;
		//mLightDirection = camDvector;
		//mLightPosition = camPosition;
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
    }
	mesh->SetName("Torus");
    return mesh;
}

Wm5::TriMesh* ShapeMaker::CreateCylinder ()
{
    VertexFormat* vformat;
    if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }

    TriMesh* mesh = StandardMesh(vformat).Cylinder(32, 32, 0.5f, 2.0f, false);
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;
        if (mUseBumpMap)
        {
            Float2& tcoord1 = vba.TCoord<Float2>(1, i);
            tcoord1[0] *= 4.0f;
            tcoord1[1] *= 4.0f;
        }
    }

    std::string baseName = Environment::GetPathR("Bricks.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();

    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector::UNIT_Z;
		//mLightDirection = camDvector;
		//mLightPosition = camPosition;
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
    }
	mesh->SetName("Cylinder");
    return mesh;
}

Wm5::TriMesh* ShapeMaker::CreateSphere ()
{
    VertexFormat* vformat;
    if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }

    TriMesh* mesh = StandardMesh(vformat).Sphere(32, 32, 1.0f);
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;
        if (mUseBumpMap)
        {
            Float2& tcoord1 = vba.TCoord<Float2>(1, i);
            tcoord1[0] *= 4.0f;
            tcoord1[1] *= 4.0f;
        }
    }

    std::string baseName = Environment::GetPathR("SunFire.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();

    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector::UNIT_Z;
		//mLightDirection = camDvector;
		//mLightPosition = camPosition;
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
    }
	mesh->SetName("Sphere");
    return mesh;
}

Wm5::TriMesh* ShapeMaker::CreateCube ()
{
    VertexFormat* vformat;
    if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }

    TriMesh* mesh = StandardMesh(vformat).Box(0.5f, 0.5f, 0.5f);
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;
        if (mUseBumpMap)
        {
            Float2& tcoord1 = vba.TCoord<Float2>(1, i);
            tcoord1[0] *= 4.0f;
            tcoord1[1] *= 4.0f;
        }
    }

    std::string baseName = Environment::GetPathR("Grass.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();
/*
    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector::UNIT_Z;
		//mLightDirection = camDvector;
		//mLightPosition = camPosition;
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
	*/
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
 //   }
	mesh->SetName("Cube");
    return mesh;
}

//----------------------------------------------------------------------------
/*
Wm5::TriMesh* ShapeMaker::CreateSquare ()
{
    VertexFormat* vformat;
    if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }
    int vstride = vformat->GetStride();

	TriMesh* mesh = StandardMesh(vformat).Rectangle(50, 50, 1.0f, 1.0f);
    VertexBuffer* vbuffer = new0 VertexBuffer(4, vstride);
    VertexBufferAccessor vba(vformat, vbuffer);
    vba.Position<Vector3f>(0) = Vector3f(-1.0f, -1.0f, 0.0f);
    vba.Position<Vector3f>(1) = Vector3f(+1.0f, -1.0f, 0.0f);
    vba.Position<Vector3f>(2) = Vector3f(+1.0f, +1.0f, 0.0f);
    vba.Position<Vector3f>(3) = Vector3f(-1.0f, +1.0f, 0.0f);

    if (mUseBumpMap)

    {
        vba.Normal<Vector3f>(0) = Vector3f::UNIT_Z;
        vba.Normal<Vector3f>(1) = Vector3f::UNIT_Z;
        vba.Normal<Vector3f>(2) = Vector3f::UNIT_Z;
        vba.Normal<Vector3f>(3) = Vector3f::UNIT_Z;

        vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
        vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
        vba.TCoord<Float2>(0, 2) = Float2(1.0f, 1.0f);
        vba.TCoord<Float2>(0, 3) = Float2(0.0f, 1.0f);

        vba.TCoord<Float2>(1, 0) = Float2(0.0f, 0.0f);
        vba.TCoord<Float2>(1, 1) = Float2(1.0f, 0.0f);
        vba.TCoord<Float2>(1, 2) = Float2(1.0f, 1.0f);
        vba.TCoord<Float2>(1, 3) = Float2(0.0f, 1.0f);
    }
    else
    {
        vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
        vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
        vba.TCoord<Float2>(0, 2) = Float2(1.0f, 1.0f);
        vba.TCoord<Float2>(0, 3) = Float2(0.0f, 1.0f);
    }

    IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
    int* indices = (int*)ibuffer->GetData();
    indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
    indices[3] = 0;  indices[4] = 2;  indices[5] = 3;

    //TriMesh* mesh = new0 TriMesh(vformat, vbuffer, ibuffer);

    std::string baseName = Environment::GetPathR("Bricks.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();

    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector(-1.0f, -1.0f, -1.0f);
        mLightDirection.Normalize();
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
    }

    return mesh;
}
*/

Wm5::TriMesh* ShapeMaker::CreateRectangle ()
{
    VertexFormat* vformat;
    if (mUseBumpMap)
    {
        vformat = VertexFormat::Create(5,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
    }
    else
    {
        vformat = VertexFormat::Create(2,
            VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
            VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    }

    TriMesh* mesh = StandardMesh(vformat).Rectangle(10, 10, 50.0f, 50.0f);
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;
        if (mUseBumpMap)
        {
            Float2& tcoord1 = vba.TCoord<Float2>(1, i);
            tcoord1[0] *= 4.0f;
            tcoord1[1] *= 4.0f;
        }
    }

    std::string baseName = Environment::GetPathR("Bricks.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();

    if (mUseBumpMap)
    {
        std::string effectFile = Environment::GetPathR("SimpleBumpMap.wmfx");
        SimpleBumpMapEffect* effect = new0 SimpleBumpMapEffect(effectFile);

        std::string normalName = Environment::GetPathR("BricksNormal.wmtf");
        Texture2D* normalTexture = Texture2D::LoadWMTF(normalName);
        normalTexture->GenerateMipmaps();

        mesh->SetEffectInstance(effect->CreateInstance(baseTexture,
            normalTexture));

        mLightDirection = AVector::UNIT_Z;
		//mLightDirection = camDvector;
		//mLightPosition = camPosition;
        SimpleBumpMapEffect::ComputeLightVectors(mesh, mLightDirection);
    }
    else
    {
	
        mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
            baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
            Shader::SC_REPEAT));
    }
	mesh->SetName("Rectangle");
    return mesh;
}