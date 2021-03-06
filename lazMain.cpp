// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#include "lazMain.h"

WM5_WINDOW_APPLICATION(lazMain);

	int num_proj = 0;
	int cur_proj = 0;

//----------------------------------------------------------------------------
lazMain::lazMain ()
    :
	WindowApplication3("Lazlow!!!", 0, 0, 1024, 768,
        Float4(0.8f, 0.8f, 0.8f, 0.8f)),
        mTextColor(1.0f, 0.0f, 1.0f, 1.0f)//, mySlides()
{
	//Application::ThePath = WM5Path + "MyApplications/lazlow/";
	//Application::ThePath = getRealPath() + "/GCodeBase/";
	Application::ThePath = getRealPath() + "/";
	Environment::InsertDirectory(ThePath + "Shaders/");
	Environment::InsertDirectory(WM5Path + "Data/Wmtf/");
	
    mUseTorus = true;
    mUseBumpMap = true;
	mShapeMaker = ShapeMaker(mUseBumpMap);

	thePlayer = PlayerCharacter(&mShapeMaker, mCamera);
	currentPlayerMotion = AVector(0.0f, 0.0f, 0.0f);
	activeMines = 0;
	gameState = 2;
	cardCounter = 4;
	wPressed = false;
	aPressed = false;
	sPressed = false;
	dPressed = false;
	useGamepad = false;
	
}
//----------------------------------------------------------------------------
bool lazMain::OnInitialize ()
{
	clock0 = clock();
	clock1 = clock();
	shot_clock = clock();

	bossMode = false;
	if (!WindowApplication3::OnInitialize())
    {
        return false;
    }

	float normalized_length = 1 / sqrt(2.0f);
	
    // Set up the camera.
    mCamera->SetFrustum(60.0f, GetAspectRatio(), 0.1f, 100.0f);
    APoint camPosition(0.0f, 15.f, -15.f);
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

	liveEnemies = NUM_SPAWNERS;
	curSpawner = 0;
	thePlayer.setShotDir(AVector::UNIT_Z);

	//Audio stuff
	audio.enableAudio();

	//GamePad stuff
	controller.initialize();

    //InitializeCameraMotion(0.01f, 0.001f);
	InitializeCameraMotion(0.01f, 0.001f, 0.01f, 0.001f);
    InitializeObjectMotion(mScene);

	//mySlides.buildCurSlide();
	
    return true;
}

std::string lazMain::getRealPath() {
	//stuff
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
		//we're fucked!
	}
	return std::string(cCurrentPath);
}

//----------------------------------------------------------------------------
void lazMain::OnTerminate ()
{
    mScene = 0;

    WindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------

void lazMain::EnemyProjectileCollisionTest(lazEnemy testingEnemy[], lazProjectile testingProjectiles[], int testTarget, int numEnemies, int numProjectiles)
{	
	if(testingEnemy[testTarget].active() )
	{
		for(int i = 0; i < numProjectiles; i++)
		{
			if(testingProjectiles[i].active())
			{
				AVector projectileToEnemy = testingProjectiles[i].loc - testingEnemy[testTarget].loc;
				
				//check if projectile hit enemy
				if((projectileToEnemy.Length() - testingEnemy[testTarget].radius - testingProjectiles[i].radius) <= 0)
				{
					testingEnemy[testTarget].hit(1);
		
					if(testingProjectiles[i].type == 1)
					{
						testingProjectiles[i].radius += .05f;
						audio.playEffect(4);
					}
					else
					{
						audio.playEffect(1);
						testingProjectiles[i].state = 0;
						testingProjectiles[i].mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
					}

					if(testingEnemy[testTarget].getCurrentHealth() <= 0)
					{
						liveEnemies--;
						testingEnemy[testTarget].setState(0);
						testingEnemy[testTarget].mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
					}
					else
					{
						testingEnemy[testTarget].radius = testingEnemy[testTarget].getCurrentHealth() * 0.35f; //.7 is diameter
						testingEnemy[testTarget].mesh->LocalTransform.SetScale(APoint(testingEnemy[testTarget].getCurrentHealth() * 0.7f, testingEnemy[testTarget].getCurrentHealth() * 0.7f, 0.5f)); //cylinder is intially along the z axis
					}
				}
			}
		}
	}
}

AVector lazMain::EnemyMove(lazEnemy movingEnemy[], int moveTarget, AVector playerEnemyVector)
{	
	AVector projectilePath;

	switch(movingEnemy[moveTarget].behavior)
	{
		case 3:
		{	
			projectilePath = AVector::ZERO;
			break;
		}
		case 2:
		{
			projectilePath = -(playerEnemyVector.Cross(AVector::UNIT_Y));
			projectilePath = (playerEnemyVector * 0.75 + projectilePath) / 2;

			break;
		}
		case 1:
		{
			projectilePath = playerEnemyVector.Cross(AVector::UNIT_Y);
			projectilePath = (playerEnemyVector * 0.75 + projectilePath) / 2;
			break;
		}
		case 0:
		default:
		{
			projectilePath = playerEnemyVector;
			break;
		}
	}

	return projectilePath;
}

void lazMain::TimeBasedMove() {

	if (useGamepad) {
		setMotionFromGamepad();
	} else {
		setMotionFromKeyboard();
	}

	clock0 = clock();
	//do the movment-stuff here

//Projectiles
	for(int i = 0; i < NUM_PROJECTILES; i++)
	{
		//projectiles[i].loc = projectiles[i].loc + AVector(projectiles[i].x_dir, 0.0, projectiles[i].z_dir);
		//projectiles[i].mesh->LocalTransform.SetTranslate(APoint(projectiles[i].loc));
		if(projectiles[i].active())
		{
			projectiles[i].Update();
		}
	}

//Mines
	for(int i = 0; i < NUM_MINES; i++)
	{
		if(mines[i].active())
		{		
			if((thePlayer.getLocation() - mines[i].loc).Length() - thePlayer.radius - mines[i].radius <= 0 && mines[i].radius == 0.35f)
			{
				audio.playEffect(7);
				mines[i].radius += 3.1f;
			}

			if(mines[i].radius > .35)
			{
				mines[i].radius += .05f;
			}

			mines[i].mesh->LocalTransform.SetScale(APoint(mines[i].radius, 0.25f, mines[i].radius));
			mines[i].Update();

			if(mines[i].radius > 3.0)
			{
				mines[i].state = 0;
				mines[i].radius = 0.35f;
				mines[i].mesh->LocalTransform.SetScale(APoint(mines[i].radius, 0.25f, mines[i].radius));
				mines[i].mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
				activeMines--;
				sprintf(mPickMessage, "Mines Available: %d", NUM_MINES - activeMines);
			}	
		}
	}

//Enemies
	for(int j = 0; j < NUM_ENEMIES; j++)
	{	
		EnemyProjectileCollisionTest(enemies, projectiles, j, NUM_ENEMIES, NUM_PROJECTILES);
		EnemyProjectileCollisionTest(enemies, mines, j, NUM_ENEMIES, NUM_MINES);

		AVector playerToEnemy = thePlayer.getLocation() - enemies[j].loc;
		
		if(enemies[j].active()) //if enemy is still active, check for player death
		{
			if(playerToEnemy.Length() - thePlayer.radius - enemies[j].radius <= 0)
			{
				sprintf(mPickMessage, "Dead.");
				audio.playEffect(2);
				
				if(!bossMode)
				{
					for(int j = 0; j < NUM_ENEMIES; j++) //despawn all enemies
					{
						if(enemies[j].active())
						{
							liveEnemies--;
						}
						enemies[j].setState(0);
						enemies[j].mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
					}
				}
				else
				{
					//move bosses back
					if(enemies[0].active())
					{
						enemies[0].loc = APoint(60.f, 0.f, 0.f);
						enemies[0].mesh->LocalTransform.SetTranslate(enemies[0].loc);
					}
					
					if(enemies[1].active())
					{
						enemies[1].loc = APoint(-60.f, 0.f, 0.f);
						enemies[1].mesh->LocalTransform.SetTranslate(enemies[1].loc);
					}
					
					if(enemies[2].active())
					{
						enemies[2].loc = APoint(0.f, 0.f, 60.f);
						enemies[2].mesh->LocalTransform.SetTranslate(enemies[2].loc);
					}
					
					if(enemies[3].active())
					{
						enemies[3].loc = APoint(0.f, 0.f, -60.f);
						enemies[3].mesh->LocalTransform.SetTranslate(enemies[3].loc);
					}					

				}
				
				//currentPlayerMotion = APoint::ORIGIN - thePlayer.getLocation();
				currentPlayerMotion = AVector::ZERO;
				thePlayer.setLocation(APoint::ORIGIN);
			}
		}

		if(enemies[j].active()) //if enemy is still active, then move it
		{		
			playerToEnemy.Normalize();

			playerToEnemy = EnemyMove(enemies, j, playerToEnemy);
			//playerToEnemy.Normalize();
			
			if(enemies[j].x_dir == 0 && enemies[j].x_dir == 0)
			{
				enemies[j].x_dir = playerToEnemy.X();
				enemies[j].z_dir = playerToEnemy.Z();
			}
			else
			{
				enemies[j].x_dir = (enemies[j].x_dir + playerToEnemy.X()) / 2;
				enemies[j].z_dir = (enemies[j].z_dir + playerToEnemy.Z()) / 2;
			}

			//scaling enemy speed
			enemies[j].x_dir /= 15;
			enemies[j].z_dir /= 15;
			enemies[j].Update();
		}
	}

//Spawners
	for(int j = 0; j < NUM_SPAWNERS; j++)
	{	
		EnemyProjectileCollisionTest(spawners, projectiles, j, NUM_SPAWNERS, NUM_PROJECTILES);
		EnemyProjectileCollisionTest(spawners, mines, j, NUM_SPAWNERS, NUM_MINES);

		AVector playerToEnemy = thePlayer.getLocation() - spawners[j].loc;
		
		if(spawners[j].active()) //if enemy is still active, check for player death
		{
			if(playerToEnemy.Length() - thePlayer.radius - spawners[j].radius <= 0)
			{
				sprintf(mPickMessage, "Dead by spawner.");
				audio.playEffect(2);
				
				for(int j = 0; j < NUM_ENEMIES; j++) //despawn all enemies
				{
					if(enemies[j].active())
					{
						liveEnemies--;
					}
					enemies[j].setState(0);
					enemies[j].mesh->LocalTransform.SetTranslate(APoint(0.0f, 100.0f, 0.0f));
				}
				
				currentPlayerMotion = APoint::ORIGIN - thePlayer.getLocation();
			}
		}

		if(spawners[j].active() && j == curSpawner) //if spawner is still active, spawn someone
		{	
			for(int i = 0; i < NUM_ENEMIES; i++)
			{
				//find an inactive enemy and create it
				if(!enemies[i].active())
				{
					liveEnemies++;
					enemies[i].setState(1);
					enemies[i].setHealth(2);
					enemies[i].mesh->LocalTransform.SetScale(APoint(1.0f, 1.0f, 1.0f));
					enemies[i].loc = spawners[j].loc + AVector(0.0f, 0.5f, 0.0f);
					enemies[i].Update();
					break;
				}
			}
		}
	}

	if(++curSpawner >= NUM_SPAWNERS)
	{
		curSpawner = 0; 
	}

//Victory check

	if (bossMode && (liveEnemies <= 0)) {
		if(gameState == 0)
		{
			
			gameState = 1;
		}
		sprintf(mPickMessage, "Win!");
		currentPlayerMotion = APoint::ORIGIN - thePlayer.getLocation();
		mScene->AttachChild(endCard);
		audio.playEffect(6);
		gameState = 3;
	}
	
	if(!bossMode && (liveEnemies <= 0))
	{
		audio.playEffect(5);
		//sprintf(mPickMessage, "Win!"sm.SetScale(APoint(7.f, 7.f, 0.5f));
		enemies[0].setHealth(10);
		enemies[0].behavior = 0;
		enemies[0].radius = 7.f;
		enemies[1].mesh->LocalTransform.SetScale(APoint(7.f, 7.f, 0.5f));
		enemies[1].setHealth(10);
		enemies[1].behavior = 0;
		enemies[1].radius = 7.f;
		enemies[2].mesh->LocalTransform.SetScale(APoint(7.f, 7.f, 0.5f));
		enemies[2].setHealth(10);
		enemies[2].behavior = 0;
		enemies[2].radius = 7.f;
		enemies[3].mesh->LocalTransform.SetScale(APoint(7.f, 7.f, 0.5f));
		enemies[3].setHealth(10);
		enemies[3].behavior = 0;
		enemies[3].radius = 7.f;

		enemies[0].loc = APoint(60.f, 0.f, 0.f);
		enemies[0].mesh->LocalTransform.SetTranslate(enemies[0].loc);
		enemies[1].loc = APoint(-60.f, 0.f, 0.f);
		enemies[1].mesh->LocalTransform.SetTranslate(enemies[1].loc);
		enemies[2].loc = APoint(0.f, 0.f, 60.f);
		enemies[2].mesh->LocalTransform.SetTranslate(enemies[2].loc);
		enemies[3].loc = APoint(0.f, 0.f, -60.f);
		enemies[3].mesh->LocalTransform.SetTranslate(enemies[3].loc);

		liveEnemies += 4;
		enemies[0].setState(1);
		enemies[1].setState(1);
		enemies[2].setState(1);
		enemies[3].setState(1);
		bossMode = true;
	}

//Player Movement

	//check if player is in bounds
	APoint PlayerLocation = thePlayer.getLocation();

	if((PlayerLocation.X() + currentPlayerMotion.X()) < -50 + thePlayer.radius)
	{
		currentPlayerMotion.X() = -50 - PlayerLocation.X() + thePlayer.radius;
	}
	else if((PlayerLocation.X() + currentPlayerMotion.X()) > 50 - thePlayer.radius)
	{
		currentPlayerMotion.X() = 50 - PlayerLocation.X() - thePlayer.radius;
	}

	if((PlayerLocation.Z() + currentPlayerMotion.Z()) < -50 + thePlayer.radius)
	{
		currentPlayerMotion.Z() = -50 - PlayerLocation.Z() + thePlayer.radius;
	}
	else if((PlayerLocation.Z() + currentPlayerMotion.Z()) > 50 - thePlayer.radius)
	{
		currentPlayerMotion.Z() = 50 - PlayerLocation.Z() - thePlayer.radius;
	}

	mCamera->SetPosition(thePlayer.movePlayer(currentPlayerMotion));

	mScene->Update();
	UpdateBumpMap();
	mCuller.ComputeVisibleSet(mScene);

    if (MoveCamera())
    {
		//playerLocation = mCamera->GetPosition();
		//playerLocation += AVector(0.0f, -7.5f, 7.5f);
		//playerCharacter->LocalTransform.SetTranslate(playerLocation);
		//thePlayer.setLocation(mCamera->GetPosition() + AVector(0.0f, -15.f, 15.f));
		//thePlayer.mMesh->LocalTransform.SetTranslate(thePlayer.getLocation());
		
		mCuller.ComputeVisibleSet(mScene);
    }

    if (MoveObject())
    {
        UpdateBumpMap();
        mScene->Update();
        mCuller.ComputeVisibleSet(mScene);
    }
}

void lazMain::OnIdle ()
{
	MeasureTime();
	mRenderer->SetCamera(mCamera);
	controller.rightTrigger = 0.0f;
	controller.poll();
		if (controller.startButtonDown && useGamepad) {
			if (gameState == 0) {
				gameState = 1;
				sprintf(mPickMessage, "Game paused, press start button to continue.");
			} else {
				gameState = 0;
				sprintf(mPickMessage, "Unpaused.");
			}
		}
	switch(gameState)
	{
	case 0:
		//playing
		if (((float)(clock() - shot_clock) / CLOCKS_PER_SEC) > 0.05 && useGamepad) {
			shot_clock = clock();
			AVector shotDir = controller.rightStick;
			shotDir.Normalize();
			thePlayer.setShotDir(AVector(shotDir.Z(), 0.0f, shotDir.X()));
			
			if(controller.rightTrigger > 0.25f) {
			
				if(controller.bumperDown)
				{
					if(activeMines < 10)
					{
						for(int i = 0; i < NUM_MINES; i++)
						{
							//find an inactive mine and create it
							if(!mines[i].active())
							{				
								mines[i].type = 1;
								mines[i].state = 1;
								mines[i].loc = thePlayer.getLocation() + shotDir * 2;
								mines[i].x_dir = 0;
								mines[i].z_dir = 0;
								activeMines++;
								sprintf(mPickMessage, "Mines Available: %d", NUM_MINES - activeMines);
								break;
							}
						}				
					}
				}
				else
				{
					projectiles[cur_proj].loc = thePlayer.getLocation();
					projectiles[cur_proj].state = 1;
				
					projectiles[cur_proj].x_dir = shotDir.Z();
					projectiles[cur_proj].z_dir = shotDir.X();
				
					projectiles[cur_proj].x_dir /= 5;
					projectiles[cur_proj].z_dir /= 5;
				}
		
				if(++cur_proj >= NUM_PROJECTILES)
				{
					cur_proj = 0; 
				}
			}	
		}
		//AVector shotDir = AVector( -(x - ((float)GetWidth() / 2.0f)), 0, -(y - ((float)GetHeight() / 2.0f)) );
		
		clock1 = clock();
		if (((float)(clock1 - clock0) / CLOCKS_PER_SEC) > 0.005) {
			//call TimeBasedMove()
			TimeBasedMove();
		}
		break;
	case 1:
		//paused
		// Draw the foreground polygon last since it has transparency.
       // mRenderer->SetCamera(mySlides.mScreenCamera);
        //mRenderer->Draw(mySlides.mForePoly);
		break;
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

void lazMain::InitializeCameraMotion (float trnSpeed, float rotSpeed, float trnSpeedFactor, float rotSpeedFactor)
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

void lazMain::setMotionFromKeyboard() {
	currentPlayerMotion = AVector(0.0f, 0.0f, 0.0f);
	if (aPressed) {
		currentPlayerMotion += AVector(0.05f, 0.0f, 0.0f);
	}
	if (dPressed) {
		currentPlayerMotion += AVector(-0.05f, 0.0f, 0.0f);
	}
	if (wPressed) {
		currentPlayerMotion += AVector(0.0f, 0.0f, 0.05f);
	}
	if (sPressed) {
		currentPlayerMotion += AVector(0.0f, 0.0f, -0.05f);
	}
}
void lazMain::setMotionFromGamepad() {
	currentPlayerMotion = controller.leftStick;
	currentPlayerMotion /= 10;

	//reasonable code above this line
}

bool lazMain::OnSpecialKeyDown(int key, int x, int y) {
	//do nothing to intercept default keys that we're not using
	return false;
}

bool lazMain::OnSpecialKeyUp(int key, int x, int y) {
	//do nothing to intercept default keys that we're not using
	return false;
}

bool lazMain::OnKeyDown (unsigned char key, int x, int y)
{
	/*
	if (WindowApplication3::OnKeyDown(key, x, y))
    {
        return true;
    }
	*/

    switch (key)
    {
	case 'p':
    case 'P':
    {
		if(gameState == 0)
		{
			gameState = 1;
			sprintf(mPickMessage, "Paused, press \'p\' to unpause.");
		}
		else if(gameState == 1)
		{
			gameState = 0;
			sprintf(mPickMessage, "Unpaused");
		}
		else if(gameState == 2) {
			if (cardCounter == 4) {
				titleCard->LocalTransform.SetTranslate(APoint(0.0f, 100.f, 15.f));
				cardCounter--;
				return true;
			}
			else if (cardCounter == 3) {
				storyCard->LocalTransform.SetTranslate(APoint(0.0f, 100.f, 15.f));
				cardCounter--;
				return true;
			}
			else if (cardCounter == 2) {
				torusCard->LocalTransform.SetTranslate(APoint(0.0f, 100.f, 15.f));
				cardCounter--;
				return true;
			}
			else if (cardCounter == 1) {
				controlCard->LocalTransform.SetTranslate(APoint(0.0f, 100.f, 15.f));
				cardCounter--;
				return true;
			}
			else {
				gameState = 0;
				return true;
			}
		}

		return true;
	}
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
	case 'q':
	case 'Q':
	{
		/*
		for(int i = 0; i < NUM_ENEMIES; i++)
		{
			//find an inactive enemy and create it
			if(!enemies[i].active())
			{
				enemies[i].setState(1);
				enemies[i].loc = APoint((i - 2.0f) * 2.5f, 0.0f, 3.0f);
				enemies[i].Update();
				break;
			}
		}
		return true;
		*/
	}
	case 'a':
	case 'A':
	{
			//mInsertPressed = true;
			aPressed = true;
			return true;
	}
	case 'w':
	case 'W':
		{
			//mUArrowPressed = true;
			wPressed = true;
			return true;
		}
	case 's':
	case 'S':
		{
			//mDArrowPressed = true;
			sPressed = true;
			return true;
		}
	case 'k':
	case 'K':
		{
			useGamepad = false;
			return true;
		}
	case 'g':
	case 'G':
		{
			useGamepad = true;
			return true;
		}
			
	case 'd':
	case 'D':
		{
			//mDeletePressed = true;
			dPressed = true;
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

bool lazMain::OnKeyUp (unsigned char key, int x, int y) {
	//do the business
	switch(key) {
		case 'w':
		case 'W':
		{
			//mUArrowPressed = false;
			wPressed = false;
			return true;
		}
		case 'a':
		case 'A':
		{
			//mInsertPressed = false;
			aPressed = false;
			return true;
		}
		case 'd':
		case 'D':
		{
			//mDeletePressed = false;
			dPressed = false;
			return true;
		}
		case 's':
		case 'S':
		{
			//mDArrowPressed = false;
			sPressed = false;
			return true;
		}
	}

	return false;
}

bool lazMain::OnMouseClick(int button, int state, int x, int y, unsigned int)
{
	AVector shotDir;
	
	switch(gameState)
	{
	case 0:
	
		if (state != MOUSE_DOWN)
		{
			return false;
		}
		/*
			APoint pos, newPos;
			AVector dir, shotDir;
			mRenderer->GetPickRay(x, GetHeight() - 1 - y, pos, dir);
			
			float temp = pos.Y() / dir.Y();
			
			newPos = APoint(pos.X() + dir.X() * temp, 0, pos.Z() + dir.Z() * temp);
			
			shotDir = APoint::ORIGIN - newPos;
			shotDir.Normalize();
		*/
		
		shotDir = AVector( -(x - ((float)GetWidth() / 2.0f)), 0, (-(y - ((float)GetHeight() / 2.0f)) * sqrt(2.0f)) );	
		shotDir.Normalize();

		if(button == 2)
		{
			if(activeMines < 10)
			{
				for(int i = 0; i < NUM_MINES; i++)
				{
					//find an inactive mine and create it
					if(!mines[i].active())
					{				
						audio.playEffect(3);
						mines[i].type = 1;
						mines[i].state = 1;
						mines[i].loc = thePlayer.getLocation() + shotDir * 2;
						mines[i].x_dir = 0;
						mines[i].z_dir = 0;
						activeMines++;	
						sprintf(mPickMessage, "Mines Available: %d", NUM_MINES - activeMines);
						break;
					}
				}				
			}
		}
		else
		{
			audio.playEffect(0);

			projectiles[cur_proj].loc = thePlayer.getLocation() + shotDir * 2;
			projectiles[cur_proj].state = 1;
			projectiles[cur_proj].type = 0;

			projectiles[cur_proj].x_dir = shotDir.X();
			projectiles[cur_proj].z_dir = shotDir.Z();

			projectiles[cur_proj].x_dir /= 5;
			projectiles[cur_proj].z_dir /= 5;

			if(++cur_proj >= NUM_PROJECTILES)
			{
				cur_proj = 0; 
			}
		}

		thePlayer.setShotDir(shotDir);

		/*
		float dir_magnitude = sqrt((projectiles[cur_proj].x_dir * projectiles[cur_proj].x_dir) + (projectiles[cur_proj].z_dir * projectiles[cur_proj].z_dir));
		projectiles[cur_proj].x_dir /= dir_magnitude;
		projectiles[cur_proj].z_dir /= dir_magnitude;

		projectiles[cur_proj].x_dir /= 5;
		projectiles[cur_proj].z_dir /= 5;

		if(++cur_proj >= NUM_PROJECTILES)
		{
			cur_proj = 0; 
		}
		*/


		/*
		if(num_proj < NUM_PROJECTILES)
		{
			num_proj++;
		}*/

		//do a picking operation
		
		
		/*
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
				//sprintf(mPickMessage, "%s", object->GetName().c_str());
			}
			else
			{
				mPickMessage[0] = 0;
			}
		}*/
		return true;
	default:
		return true;
	}
}

//----------------------------------------------------------------------------
void lazMain::CreateScene ()
{
    mScene = new0 Node();

	for(int i = 0; i < NUM_PROJECTILES; i++)
	{
		projectiles[i] = lazProjectile(&mShapeMaker);
		mScene->AttachChild(projectiles[i].mesh);
	}
	
	activeMines = 0;

	for(int i = 0; i < NUM_MINES; i++)
	{
		mines[i] = lazProjectile(&mShapeMaker, 1);
		mines[i].mesh->LocalTransform.SetScale(APoint(0.35f, 0.25f, 0.35f));
		mines[i].type = 1;
		mScene->AttachChild(mines[i].mesh);
	}

	for(int i = 0; i < NUM_ENEMIES; i++)
	{
		enemies[i] = lazEnemy(&mShapeMaker);
		enemies[i].radius = 0.5;
		enemies[i].behavior = i % 3;
		enemies[i].setHealth(2);

		mScene->AttachChild(enemies[i].mesh);
	}

	for(int i = 0; i < NUM_SPAWNERS; i++)
	{
		spawners[i] = lazEnemy(&mShapeMaker); 
		spawners[i].radius = 1;
		spawners[i].setHealth(5);
		spawners[i].setState(1);
		spawners[i].behavior = 3;
		

		spawners[i].mesh->LocalTransform.SetScale(APoint(3.5f, 3.5f, 0.5f)); //cylinder is intially along the z axis

		mScene->AttachChild(spawners[i].mesh);
	}

	spawners[0].loc = APoint(15.0f, 0.0f, 15.0f);
	spawners[1].loc = APoint(15.0f, 0.0f, -15.0f);
	spawners[2].loc = APoint(-15.0f, 0.0f, 15.0f);
	spawners[3].loc = APoint(-15.0f, 0.0f, -15.0f);

	spawners[0].Update();
	spawners[1].Update();
	spawners[2].Update();
	spawners[3].Update();
	
	thePlayer.mMesh->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	thePlayer.mMesh->LocalTransform.SetTranslate(mCamera->GetPosition() + AVector(0.0f, -15.f, 15.f));
	mScene->AttachChild(thePlayer.mMesh);
	thePlayer.summonCube->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	thePlayer.summonCube->LocalTransform.SetTranslate(mCamera->GetPosition() + AVector(0.0f, -15.f, 15.f));
	mScene->AttachChild(thePlayer.summonCube);

	terrain = mShapeMaker.CreateCube();
	terrain->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, -0.5f*Mathf::PI));
	terrain->LocalTransform.SetScale(APoint(100.0f, 100.0f, 0.1f));
	terrain->LocalTransform.SetTranslate(APoint(0.0f, -0.5f, 0.0f));
	mScene->AttachChild(terrain);

	titleCard = mShapeMaker.CreateTitle();
	titleCard->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, -0.75f*Mathf::PI));
	titleCard->LocalTransform.SetScale(APoint(2.5f, 2.5f, 0.1f));
	titleCard->LocalTransform.SetTranslate(APoint(0.0f, 12.f, -12.0f));
	mScene->AttachChild(titleCard);

	storyCard = mShapeMaker.CreateStory();
	storyCard->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, -0.75f*Mathf::PI));
	storyCard->LocalTransform.SetScale(APoint(2.5f, 2.5f, 0.1f));
	storyCard->LocalTransform.SetTranslate(APoint(0.0f, 11.9f, -11.9f));
	mScene->AttachChild(storyCard);

	torusCard = mShapeMaker.CreateJoke();
	torusCard->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, -0.75f*Mathf::PI));
	torusCard->LocalTransform.SetScale(APoint(2.5f, 2.5f, 0.1f));
	torusCard->LocalTransform.SetTranslate(APoint(0.0f, 11.85f, -11.85f));
	mScene->AttachChild(torusCard);

	controlCard = mShapeMaker.CreateControls();
	controlCard->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, -0.75f*Mathf::PI));
	controlCard->LocalTransform.SetScale(APoint(2.5f, 2.5f, 0.1f));
	controlCard->LocalTransform.SetTranslate(APoint(0.0f, 11.8f, -11.8f));
	mScene->AttachChild(controlCard);

	endCard = mShapeMaker.CreateEnding();
	endCard->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, -0.75f*Mathf::PI));
	endCard->LocalTransform.SetScale(APoint(2.5f, 2.5f, 0.1f));
	endCard->LocalTransform.SetTranslate(APoint(0.0f, 12.f, -12.0f));

	//printf("New node...\n");
	
	//TriMesh* mesh[4];
	//mesh[0] = CreateTorus();
	//mesh[1] = CreateCylinder();
	//mesh[2] = CreateSphere();
	//mesh[3] = CreateCube();

	//printf("Have the meshes...\n");
	/*
	for (int i = 0; i < 4; i++) {
		mesh[i]->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	}
	*/
	
	//terrain->LocalTransform.SetRotate(HMatrix(AVector::UNIT_X, 0.5f*Mathf::PI));
	
	
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
	
	
	//printf("Did the translation...\n");

	/*
	for(int i = 0; i < 4; i++) {
		mScene->AttachChild(mesh[i]);
	}
	*/

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

	gameState = 2;
	sprintf(mPickMessage, "Press \'p\' to unpause and begin the game.");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void lazMain::UpdateBumpMap ()
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
