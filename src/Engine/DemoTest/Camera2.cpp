#include "Camera2.h"

Camera2* Camera2::instance = new Camera2(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f, -0.2f, -0.7f));

Camera2::Camera2()
{
	mMouseX = 0;
	mMouseY = 0;
}

Camera2::Camera2(const PxVec3& eye, const PxVec3& dir)/*定义眼睛位置、视线方向*/
{
	mEye = eye;
	mDir = dir.getNormalized();
	mMouseX = 0;
	mMouseY = 0;
}

Camera2* Camera2::getInstance()
{
	return instance;
}

void Camera2::deleteInstance()
{
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

/*
\brief 根据小球位置更新相机位置
*/
void Camera2::updatePosition(physx::PxVec3 ballPos)
{
	mEye = ballPos - mDistance * mDir;
}

void Camera2::handleMouse(int button, int state, int x, int y)
{
	PX_UNUSED(state);
	PX_UNUSED(button);
	mMouseX = x;
	mMouseY = y;
}

void Camera2::handleMotion(int x, int y)
{
	int dx = mMouseX - x;/*鼠标每次移动带来的视角变换*/
	int dy = mMouseY - y;
	float sensitivity = 0.3;/*鼠标灵敏度[0,1]即可*/

	PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();

	PxQuat qx(PxPi * dx * sensitivity / 180.0f, PxVec3(0, 1, 0));
	mDir = qx.rotate(mDir);
	PxQuat qy(PxPi * dy * sensitivity / 180.0f, viewY);
	mDir = qy.rotate(mDir);

	mDir.normalize();

	mMouseX = x;
	mMouseY = y;
}

PxVec3 Camera2::getEye() const
{
	return mEye;
}

PxVec3 Camera2::getDir() const
{
	return mDir;
}
