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

#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

WM5_WINDOW_APPLICATION(BumpMaps);

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
	Environment::InsertDirectory(WM5Path + "Data/Wmtf/");
	
    mUseTorus = true;
    mUseBumpMap = true;
	mShapeMaker = ShapeMaker(mUseBumpMap);

	thePlayer = PlayerCharacter(&mShapeMaker);
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
		projectiles[i].x_loc += projectiles[i].x_dir;
		projectiles[i].z_loc += projectiles[i].z_dir;
		projectiles[i].mesh->LocalTransform.SetTranslate(APoint(projectiles[i].x_loc, 0.0f, projectiles[i].z_loc));
		//mScene->AttachChild(projectiles[i]);
	}
	mScene->Update();
	UpdateBumpMap();
	mCuller.ComputeVisibleSet(mScene);

    if (MoveCamera())
    {
		//playerLocation = mCamera->GetPosition();
		//playerLocation += AVector(0.0f, -7.5f, 7.5f);
		//playerCharacter->LocalTransform.SetTranslate(playerLocation);
		thePlayer.mMesh->LocalTransform.SetTranslate(mCamera->GetPosition() + AVector(0.0f, -7.5f, 7.5f));
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

	//projectiles[cur_proj].x_loc = playerLocation[0];
	//projectiles[cur_proj].z_loc = playerLocation[2];
	projectiles[cur_proj].x_loc = thePlayer.getLocation()[0];
	projectiles[cur_proj].z_loc = thePlayer.getLocation()[2];

	projectiles[cur_proj].mesh->LocalTransform.SetTranslate(APoint(projectiles[cur_proj].x_loc, 0.0f, projectiles[cur_proj].z_loc));

	//mScene->AttachChild(projectiles[cur_proj]);

	projectiles[cur_proj].x_dir = -(float)((x - ((float)GetWidth() / 2)));
	projectiles[cur_proj].z_dir = -(float)((y - ((float)GetHeight() / 2)));
	
	float dir_magnitude = sqrt((projectiles[cur_proj].x_dir * projectiles[cur_proj].x_dir) + (projectiles[cur_proj].z_dir * projectiles[cur_proj].z_dir));
	projectiles[cur_proj].x_dir /= dir_magnitude;
	projectiles[cur_proj].z_dir /= dir_magnitude;

	projectiles[cur_proj].x_dir /= 5;
	projectiles[cur_proj].z_dir /= 5;

	if(++cur_proj >= NUM_PROJECTILES)
	{
		cur_proj = 0; 
	}

	if(num_proj < NUM_PROJECTILES)
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

	terrain = mShapeMaker.CreateCube();
	for(int i = 0; i < NUM_PROJECTILES; i++)
	{
		projectiles[i].x_loc = 0; 
		projectiles[i].z_loc = 0;
		projectiles[i].x_dir = 0;
		projectiles[i].z_dir = 0;

		projectiles[i].mesh = mShapeMaker.CreateSphere(); 
		projectiles[i].mesh->LocalTransform.SetScale(APoint(0.35f, 0.35f, 0.35f));

		projectiles[i].mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));

		mScene->AttachChild(projectiles[i].mesh);
	}
	
	//printf("Have the meshes...\n");

	/*
	for (int i = 0; i < 4; i++) {
		mesh[i]->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	}
	*/
	
	thePlayer.mMesh->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	//terrain->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	terrain->LocalTransform.SetScale(APoint(20.0f, 0.05f, 20.0f));
	
	//printf("Did the rotations...\n");

	/*
	mesh[0]->LocalTransform.SetTranslate(APoint(-6.0f, 100.0f, 0.0f));
	mesh[1]->LocalTransform.SetTranslate(APoint(0.0f, 0.0f, 0.0f));
	mesh[2]->LocalTransform.SetTranslate(APoint(2.0f, 100.0f, 0.0f));
	mesh[3]->LocalTransform.SetTranslate(APoint(6.0f, 100.0f, 0.0f));
	*/

	//playerLocation = mCamera->GetPosition();
	//playerLocation += AVector(0.0f, -7.5f, 7.5f);
	//playerCharacter->LocalTransform.SetTranslate(playerLocation);
	thePlayer.mMesh->LocalTransform.SetTranslate(mCamera->GetPosition() + AVector(0.0f, -7.5f, 7.5f));
	
	terrain->LocalTransform.SetTranslate(APoint(0.0f, -0.5f, 0.0f));
	
	//printf("Did the translation...\n");

	/*
	for(int i = 0; i < 4; i++) {
		mScene->AttachChild(mesh[i]);
	}
	*/
	
	mScene->AttachChild(thePlayer.mMesh);
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

		//SimpleBumpMapEffect::ComputeLightVectors(terrain, mLightDirection);
        //mRenderer->Update(terrain->GetVertexBuffer());

		//TODO: Re-implement this in a sane way.
    }
}
//----------------------------------------------------------------------------
