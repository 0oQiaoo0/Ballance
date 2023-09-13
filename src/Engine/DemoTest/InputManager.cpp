#include"InputManager.h"

InputManager* InputManager::instance = new (std::nothrow) InputManager();

InputManager* InputManager::getInstance()
{
	return instance;
}

void InputManager::deleteInstance()
{
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

void InputManager::keyboardCallback(unsigned char key, int x, int y)
{
	PX_UNUSED(x);
	PX_UNUSED(y);
	key = toupper(key);
	keyState[key] = true;
}

void InputManager::keyboardUpCallback(unsigned char key, int x, int y)
{
	PX_UNUSED(x);
	PX_UNUSED(y);
	key = toupper(key);
	keyState[key] = false;
}

void InputManager::InputUpdate()
{
	int force = 200;

	const PxVec3& cameraEye = Camera2::getInstance()->getEye();
	PxVec3 forward = PxVec3(cameraEye.x, 0, cameraEye.z).getNormalized();
	PxVec3 up = PxVec3(0, 1, 0);
	PxVec3 left = up.cross(forward).getNormalized();

	PxRigidDynamic* rd = PhysicsManager::getInstance()->getPlayerBall()->getRigidDynamic();

	if (keyState['W']) {
		rd->addForce(-force * forward);
	}
	if (keyState['S']) {
		rd->addForce(force * forward);
	}
	if (keyState['A']) {
		rd->addForce(-force * left);
	}
	if (keyState['D']) {
		rd->addForce(force * left);
	}
	if (keyState['B']) {
		PhysicsManager::getInstance()->createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
	}
}