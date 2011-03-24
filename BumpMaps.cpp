// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#include "BumpMaps.h"

//this nonsense is to get the current working directory
#include <stdio.h>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

WM5_WINDOW_APPLICATION(BumpMaps);

//TriMesh* mesh[4];
TriMesh* projectiles[5];
float x_loc[5], z_loc[5]; //save current position for collision detection
float x_dir[5], z_dir[5]; //save direction for movement on tick
int num_proj = 0;
int cur_proj = 0;

//----------------------------------------------------------------------------
BumpMaps::BumpMaps ()
    :
	WindowApplication3("Lazlow!!!", 0, 0, 640, 480,
        Float4(0.8f, 0.8f, 0.8f, 0.8f)),
        mTextColor(0.0f, 0.0f, 0.0f, 1.0f)
{
	//Application::ThePath = WM5Path + "MyApplications/lazlow/";
	Application::ThePath = getRealPath() + "/";
	Environment::InsertDirectory(ThePath + "Shaders/");
	
    mUseTorus = true;
    mUseBumpMap = true;
}
//----------------------------------------------------------------------------
bool BumpMaps::OnInitialize ()
{
    if (!WindowApplication3::OnInitialize())
    {
        return false;
    }

	float normalized_length = 1 / sqrt(2.0f);
	
    // Set up the camera.
    mCamera->SetFrustum(60.0f, GetAspectRatio(), 0.1f, 100.0f);
    APoint camPosition(0.0f, 7.5f, -7.5f);
    AVector camDVector(0.0f, -normalized_length, normalized_length); 
    AVector camUVector(0.0f, normalized_length, normalized_length);
    AVector camRVector = camDVector.Cross(camUVector);
    mCamera->SetFrame(camPosition, camDVector, camUVector, camRVector);

    CreateScene();

    // Initial update of objects.
    mScene->Update();
    UpdateBumpMap();

    // Initial culling of scene.
    mCuller.SetCamera(mCamera);
    mCuller.ComputeVisibleSet(mScene);

    //InitializeCameraMotion(0.01f, 0.001f);
	InitializeCameraMotion(0.01f, 0.001f, 0.01f, 0.001f);
    InitializeObjectMotion(mScene);
    return true;
}

std::string BumpMaps::getRealPath() {
	//stuff
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
		//we're fucked!
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '/0';

	return std::string(cCurrentPath);

}


//----------------------------------------------------------------------------
void BumpMaps::OnTerminate ()
{
    mScene = 0;

    WindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void BumpMaps::OnIdle ()
{
    MeasureTime();
	for(int i = 0; i < num_proj; i++)
	{
		//projectiles[i] = CreateSphere(); 
		x_loc[i] += x_dir[i];
		z_loc[i] += z_dir[i];
		projectiles[i]->LocalTransform.SetTranslate(APoint(x_loc[i], 0.0f, z_loc[i]));
		//mScene->AttachChild(projectiles[i]);
	}
	mScene->Update();
	UpdateBumpMap();
	mCuller.ComputeVisibleSet(mScene);

    if (MoveCamera())
    {
		playerLocation = mCamera->GetPosition();
		playerLocation += AVector(0.0f, -7.5f, 7.5f);
		playerCharacter->LocalTransform.SetTranslate(playerLocation);
		mCuller.ComputeVisibleSet(mScene);
    }

    if (MoveObject())
    {
        UpdateBumpMap();
        mScene->Update();
        mCuller.ComputeVisibleSet(mScene);
    }

    if (mRenderer->PreDraw())
    {
        mRenderer->ClearBuffers();
        mRenderer->Draw(mCuller.GetVisibleSet());
        DrawFrameRate(8, GetHeight()-8, mTextColor);
		if (mPickMessage[0])
        {
            mRenderer->Draw(8, 16, mTextColor, mPickMessage);
        }
		mRenderer->PostDraw();
        mRenderer->DisplayColorBuffer();
    }
	mScene->Update();
    UpdateFrameCount();
}

void BumpMaps::InitializeCameraMotion (float trnSpeed, float rotSpeed, float trnSpeedFactor, float rotSpeedFactor)
{
    mCameraMoveable = true;

    mTrnSpeed = trnSpeed;
    mRotSpeed = rotSpeed;
    mTrnSpeedFactor = trnSpeedFactor;
    mRotSpeedFactor = rotSpeedFactor;

	/*
	mWorldAxis[0] = mCamera->GetDVector();
    mWorldAxis[1] = mCamera->GetUVector();
    mWorldAxis[2] = mCamera->GetRVector();
	*/
	//AVector camDVector(0.0f, -normalized_length, normalized_length); 
	mWorldAxis[0] = AVector(0.0f, 0.0f, 1.0f);
	mWorldAxis[1] = AVector(0.0f, 1.0f, 0.0f);
	mWorldAxis[2] = AVector(1.0f, 0.0f, 0.0f);
}

//---------------------------------------------------------------------------
bool BumpMaps::OnKeyDown (unsigned char key, int x, int y)
{
	/*
	if (WindowApplication3::OnKeyDown(key, x, y))
    {
        return true;
    }
	*/

    switch (key)
    {
	
	case 'b':
    case 'B':
    {
		mUseBumpMap = !mUseBumpMap;

		///TriMesh* mesh[4];
		//mesh[0] = StaticCast<TriMesh>(mScene->GetChild(0));
		//mesh[1] = StaticCast<TriMesh>(mScene->GetChild(1));
		//mesh[2] = StaticCast<TriMesh>(mScene->GetChild(2));
		//mesh[3] = StaticCast<TriMesh>(mScene->GetChild(3));

		/*
		for (int i = 0; i < 4; i++) {
			Transform save = mesh[i]->LocalTransform;

			if (mesh[i]->GetName() == "Torus") {
				mesh[i] = CreateTorus();
			}
			else if (mesh[i]->GetName() == "Cylinder") {
				mesh[i] = CreateCylinder();
			}
			else if (mesh[i]->GetName() == "Sphere") {
				mesh[i] = CreateSphere();
			}
			else if (mesh[i]->GetName() == "Cube") {
				mesh[i] = CreateCube();
			}

			mesh[i]->LocalTransform = save;

			mScene->SetChild(i, mesh[i]);
		}
		*/
			
		//mScene->Update();
		//UpdateBumpMap();
		mCuller.ComputeVisibleSet(mScene);
		return true;
	}
	case 'a':
	case 'A':
	{
			 mInsertPressed = true;
			 return true;
	}
	case 'w':
	case 'W':
		{
			mUArrowPressed = true;
			return true;
		}
	case 's':
	case 'S':
		{
			mDArrowPressed = true;
			return true;
		}
	case 'd':
	case 'D':
		{
			mDeletePressed = true;
			return true;
		}
					
		/*
		mUseBumpMap = !mUseBumpMap;
        TriMesh* mesh = StaticCast<TriMesh>(mScene->GetChild(0));
        if (mUseTorus)
        {
            Transform save = mesh->LocalTransform;
            mesh = CreateTorus();
            mesh->LocalTransform = save;
        }
        else
        {
            mesh = CreateSquare();
        }
        mScene->SetChild(0, mesh);
        mScene->Update();
        UpdateBumpMap();
        mCuller.ComputeVisibleSet(mScene);
        return true;
    }
	*/
	}

    return false;
}

bool BumpMaps::OnKeyUp (unsigned char key, int x, int y) {
	//do the business
	switch(key) {
		case 'w':
		case 'W':
		{
			mUArrowPressed = false;
			return true;
		}
		case 'a':
		case 'A':
		{
			mInsertPressed = false;
			return true;
		}
		case 'd':
		case 'D':
		{
			mDeletePressed = false;
			return true;
		}
		case 's':
		case 'S':
		{
			mDArrowPressed = false;
			return true;
		}
	}

	return false;
}

bool BumpMaps::OnMouseClick(int button, int state, int x, int y, unsigned int)
{
	
	if (state != MOUSE_DOWN)
	{
		return false;
	}

	//projectiles[cur_proj] = CreateSphere(); 
	//projectiles[cur_proj]->LocalTransform.SetScale(APoint(0.35f, 0.35f, 0.35f));

	x_loc[cur_proj] = playerLocation[0];
	z_loc[cur_proj] = playerLocation[2];

	projectiles[cur_proj]->LocalTransform.SetTranslate(APoint(x_loc[cur_proj], 0.0f, z_loc[cur_proj]));

	//mScene->AttachChild(projectiles[cur_proj]);

	x_dir[cur_proj] = -(float)((x - ((float)GetWidth() / 2)));
	z_dir[cur_proj] = -(float)((y - ((float)GetHeight() / 2)));
	
	float dir_magnitude = sqrt((x_dir[cur_proj] * x_dir[cur_proj]) + (z_dir[cur_proj] * z_dir[cur_proj]));
	x_dir[cur_proj] /= dir_magnitude;
	z_dir[cur_proj] /= dir_magnitude;

	x_dir[cur_proj] /= 5;
	z_dir[cur_proj] /= 5;

	if(++cur_proj > 4)
	{
		cur_proj = 0; 
	}

	if(num_proj <= 4)
	{
		num_proj++;
	}

	//do a picking operation
	APoint pos;
	AVector dir;
	if (mRenderer->GetPickRay(x, GetHeight() - 1 - y, pos, dir))
	{
		mPicker.Execute(mScene, pos, dir, 0.0f, Mathf::MAX_REAL);
		if (mPicker.Records.size() > 0)
		{
			//Display the selected object's name
			const PickRecord& record = mPicker.GetClosestNonnegative();
			const Spatial* object = record.Intersected;
			sprintf(mPickMessage, "%s", object->GetName().c_str());
		}
		else
		{
			mPickMessage[0] = 0;
		}
	}
	return true;
}

//----------------------------------------------------------------------------
void BumpMaps::CreateScene ()
{
    mScene = new0 Node();
	
	//printf("New node...\n");
	
	//TriMesh* mesh[4];
	//mesh[0] = CreateTorus();
	//mesh[1] = CreateCylinder();
	//mesh[2] = CreateSphere();
	//mesh[3] = CreateCube();

	playerCharacter = CreateTorus();
	terrain = CreateSquare();
	for(int i = 0; i < 5; i++)
	{
		x_loc[i] = 0; 
		z_loc[i] = 0;
		x_dir[i] = 0;
		z_dir[i] = 0;

		projectiles[i] = CreateSphere(); 
		projectiles[i]->LocalTransform.SetScale(APoint(0.35f, 0.35f, 0.35f));

		projectiles[i]->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));

		mScene->AttachChild(projectiles[i]);
	}
	
	//printf("Have the meshes...\n");

	/*
	for (int i = 0; i < 4; i++) {
		mesh[i]->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	}
	*/
	
	playerCharacter->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	terrain->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	
	//printf("Did the rotations...\n");

	/*
	mesh[0]->LocalTransform.SetTranslate(APoint(-6.0f, 100.0f, 0.0f));
	mesh[1]->LocalTransform.SetTranslate(APoint(0.0f, 0.0f, 0.0f));
	mesh[2]->LocalTransform.SetTranslate(APoint(2.0f, 100.0f, 0.0f));
	mesh[3]->LocalTransform.SetTranslate(APoint(6.0f, 100.0f, 0.0f));
	*/

	playerLocation = mCamera->GetPosition();
	playerLocation += AVector(0.0f, -7.5f, 7.5f);
	playerCharacter->LocalTransform.SetTranslate(playerLocation);
	terrain->LocalTransform.SetTranslate(APoint(0.0f, -0.5f, 0.0f));
	
	//printf("Did the translation...\n");

	/*
	for(int i = 0; i < 4; i++) {
		mScene->AttachChild(mesh[i]);
	}
	*/
	
	mScene->AttachChild(playerCharacter);
	mScene->AttachChild(terrain);

	//printf("Attached children to scene...\n");
	
	/*
	if (mUseTorus)
    {
        mesh = CreateCube();
        mesh->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.25f*Mathf::PI));
    }
    else
    {
        mesh = CreateSquare();
    }
    mScene->AttachChild(mesh);
	*/
}
//----------------------------------------------------------------------------
TriMesh* BumpMaps::CreateTorus ()
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

TriMesh* BumpMaps::CreateCylinder ()
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

TriMesh* BumpMaps::CreateSphere ()
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
	mesh->SetName("Sphere");
    return mesh;
}

TriMesh* BumpMaps::CreateCube ()
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
	mesh->SetName("Cube");
    return mesh;
}

//----------------------------------------------------------------------------
TriMesh* BumpMaps::CreateSquare ()
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

    TriMesh* mesh = new0 TriMesh(vformat, vbuffer, ibuffer);

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
//----------------------------------------------------------------------------
void BumpMaps::UpdateBumpMap ()
{
    if (mUseBumpMap)
    {
        // The scene graph transformations have been updated, which means the
        // tangent-space light vectors need updating.
		/*
		TriMesh* mesh[4];
		for (int i = 0; i < 4; i++) {
			mesh[i] = StaticCast<TriMesh>(mScene->GetChild(i));
        	SimpleBumpMapEffect::ComputeLightVectors(mesh[i], mLightDirection);
        	mRenderer->Update(mesh[i]->GetVertexBuffer());
		}
		*/

		//TODO: Re-implement this in a sane way.
    }
}
//----------------------------------------------------------------------------
