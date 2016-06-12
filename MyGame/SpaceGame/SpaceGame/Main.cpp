/*****************************************************************************************
* Desc: Tutorial 02 IND_Surface
*****************************************************************************************/

#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Animation.h"

/*
==================
Main
==================
*/

// ----- IndieLib intialization -----
CIndieLib *mI = CIndieLib::instance();

const float PLAYER_WALK_ANIMATION_SPEED = 0.15f;

//Time
float timeDelta = 0;	//the time difference between frames in secconds

//vars
IND_Vector2 playerPos = IND_Vector2(140, 200);	//the current position of the player
float health = 100;
float maxHealth = 100;
float playerWalkSpeed = 100; //the walk speed of the player in pixel/sec
float playerRunSpeed = 200;	 //the run speed of the player in pixel/sec
float playerSequence;		 //used to control the player animation based on the distance moved
const char* playerBoundsArea = "player";
const char* wallsBoundsArea = "walls";

//controls the player movement and animation
//It applies the player input velocity to the player's position vector
void ManagePlayerMovement(IND_Entity2d *mPlayer, IND_Animation *animation)
{
	IND_Vector2 playerVelocity = IND_Vector2(0,0);
	//this controls if the player "sprints" when left shift is pressed
	float playerSpeed = mI->_input->isKeyPressed(IND_LSHIFT) ? playerRunSpeed : playerWalkSpeed;

	if (mI->_input->isKeyPressed(IND_W))
	{
		playerVelocity._y -= playerSpeed * timeDelta;
	}

	if (mI->_input->isKeyPressed(IND_S))
	{
		playerVelocity._y += playerSpeed * timeDelta;
	}

	if (mI->_input->isKeyPressed(IND_D))
	{
		playerVelocity._x += playerSpeed * timeDelta;
		mPlayer->setMirrorX(true);
	}

	if (mI->_input->isKeyPressed(IND_A))
	{
		playerVelocity._x -= playerSpeed * timeDelta;
		mPlayer->setMirrorX(false);
	}

	playerPos += playerVelocity;
	playerSequence += playerVelocity.length() * PLAYER_WALK_ANIMATION_SPEED;

	//did some custon collision checking, because the indilib one dosn't work for me.
	animation->setActualFramePos(0, ((int)playerSequence) % animation->getNumFrames(0));
	playerPos._x = min(max(0, playerPos._x),mI->_window->getWidth() - animation->getHighWidth(0));
	playerPos._y = min(max(0, playerPos._y), mI->_window->getHeight() - animation->getHighHeight(0));
	mPlayer->setPosition(playerPos._x, playerPos._y, 0);
	if (playerPos._x == 0 || playerPos._x == mI->_window->getWidth() - animation->getHighWidth(0) || playerPos._y == 0 || playerPos._y == mI->_window->getHeight() - animation->getHighHeight(0))
	{
		health -= 10 * timeDelta;
	}
}

void DrawHealthBar(IND_Entity2d *healthBarFill, IND_Entity2d *healthBarBG)
{
	healthBarFill->setRectangle(mI->_window->getWidth() - 200, 10, mI->_window->getWidth() - 20 - (180 * (1 - (health / maxHealth))), 30);
	healthBarBG->setRectangle(mI->_window->getWidth() - 205, 5, mI->_window->getWidth() - 15, 35);
}

int IndieLib()
{
	if (!mI->init()) return 0;
	
	// ----- Surface loading -----

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../SpaceGame/resources/Stars_Background.png", IND_OPAQUE, IND_32)) return 0;

	// ----- Animations loading -----

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter1 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter1, "../SpaceGame/resources/animations/character1.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter2 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter2, "../SpaceGame/resources/animations/character2.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Dust animation, we apply a color key of (255, 0, 255)
	IND_Animation *mAnimationDust = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationDust, "../SpaceGame/resources/animations/dust.xml", IND_ALPHA, IND_16, 255, 0, 255)) return 0;

	// ----- Set the surface and animations into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBack);					// Entity adding
	mBack->setSurface(mSurfaceBack);					// Set the surface into the entity

	//health bar
	IND_Entity2d *healthBarFill = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(healthBarFill);
	healthBarFill->setPrimitive2d(IND_FILL_RECTANGLE);
	healthBarFill->setTint(255, 0, 0);
	healthBarFill->setFilter(IND_FILTER_POINT);
	healthBarFill->setPosition(0, 0, 100);
	
	//healthBarFill->setSurface(mSurfaceHealthBarFill);

	IND_Entity2d *healthBarBG = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(healthBarBG);
	healthBarBG->setPrimitive2d(IND_FILL_RECTANGLE);
	healthBarBG->setTint(0, 0, 0);
	healthBarBG->setFilter(IND_FILTER_POINT);
	healthBarBG->setPosition(0, 0, 90);


	// Character 1
	IND_Entity2d *mPlayer1 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer1);					// Entity adding
	mPlayer1->setAnimation(mAnimationCharacter1);				// Set the animation into the entity

	// Character 2
	IND_Entity2d *mPlayer2 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer2);					// Entity adding
	mPlayer2->setAnimation(mAnimationCharacter2);				// Set the animation into the entity

	// Dust explosion
	IND_Entity2d *mDust = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mDust);					// Entity adding
	mDust->setAnimation(mAnimationDust);					// Set the animation into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Player 1
	mPlayer1->setSequence(0);						// Choose sequence
	mPlayer1->setMirrorX(1);						// Horizontal mirroring
	mPlayer1->setBoundingRectangle(playerBoundsArea, 0, 0, 100, 100);
	mPlayer1->showCollisionAreas(true);

	// Dust explosion
	mDust->setPosition(360, 250, 0);

	// Player 2
	mPlayer2->setSequence(0);						// Choose sequence
	mPlayer2->setPosition(550, 200, 0);
	mPlayer2->setNumReplays(3);						// The animation will be displayed 3 times
	mPlayer2->setBoundingRectangle(playerBoundsArea, 0, 0, 100, 100);
	mPlayer1->showCollisionAreas(true);

	// ----- Main Loop -----

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit() && health > 0)
	{
		timeDelta = mI->_render->getFrameTime() / 1000.0f;
		DrawHealthBar(healthBarFill,healthBarBG);
		mI->_input->update();
		ManagePlayerMovement(mPlayer1,mAnimationCharacter1);

		//the collision isn't working even with 2 defined Bounding Rectangles
		if (mI->_entity2dManager->isCollision(mPlayer1, playerBoundsArea, mPlayer2, playerBoundsArea))
		{
			mPlayer2->setPosition(0, 0, 0);
		}

		// Toogle full screen when pressing "space"
		//if (mI->_input->onKeyPress(IND_SPACE)) mI->_render->toggleFullScreen();
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_entity2dManager->renderCollisionAreas(255, 0, 0, 255);		//collisions do not render even if they are set.
		mI->_render->endScene();
		
	}

	// ----- Free -----

	mI->end();

	return 0;
}