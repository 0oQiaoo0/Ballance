#include <ctype.h>

#include "../Common/Print.h"

#include "../../Utils/Utils.h"

#include<vector>
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"

using namespace physx;

/*
* \brief Setup collision filter for the shape.
* \param shape The shape.
* \param filterGroup The filter that the shape belongs.
* \param filterMask The filters thar the shape should collisions.
*/

////按键设置
//void keyPress(unsigned char key, const PxTransform& camera)
//{
//	switch (toupper(key))
//	{
//	case 'B':	createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f); break;
//		//PxSphereGeometry Transform,geometry,velocity（速度）
//	case ' ':	createDynamic(10, camera, camera.rotate(PxVec3(0, 0, -1)) * 200); break;
//	}
//}

void keyboardCallbackWrapper(unsigned char key, int x, int y) {
	InputManager::getInstance()->keyboardCallback(key, x, y);
}

void keyboardUpCallbackWrapper(unsigned char key, int x, int y) {
	InputManager::getInstance()->keyboardUpCallback(key, x, y);
}

void idleCallbackWrapper() {
	glutPostRedisplay();
	InputManager::getInstance()->InputUpdate();
}

void renderCallbackWrapper()
{
	PhysicsManager::getInstance()->stepPhysics();

	Snippets::startRender(Camera2::getInstance()->getEye(), Camera2::getInstance()->getDir());

	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		Snippets::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);
	}
	Camera2::getInstance()->updatePosition(PhysicsManager::getInstance()->getPlayerBall()->getPosition());

	Snippets::finishRender();
}

void mouseCallbackWrapper(int button, int state, int x, int y)
{
	Camera2::getInstance()->handleMouse(button, state, x, y);
}

void motionCallbackWrapper(int x, int y)
{
	Camera2::getInstance()->handleMotion(x, y);
}

void exitCallbackWrapper()
{
	PhysicsManager::getInstance()->cleanupPhysics();
}
//main
int demoMain(int, const char* const*)
{
	PhysicsManager::getInstance()->initPhysics();

	Snippets::setupDefaultWindow("BallGame");
	Snippets::setupDefaultRenderState();
	glutKeyboardFunc(keyboardCallbackWrapper);
	glutKeyboardUpFunc(keyboardUpCallbackWrapper);
	glutIdleFunc(idleCallbackWrapper);
	glutDisplayFunc(renderCallbackWrapper);
	glutMouseFunc(mouseCallbackWrapper);
	glutMotionFunc(motionCallbackWrapper);
	//motionCallback(0, 0);
	atexit(exitCallbackWrapper);

	glutMainLoop();

	return 0;
}
