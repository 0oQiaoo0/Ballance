#pragma once

#include "foundation/PxTransform.h"
#include "foundation/PxVec3.h"

using namespace physx;

class Camera2
{
private:
	static Camera2* instance;

	Camera2();
	Camera2(const physx::PxVec3& eye, const physx::PxVec3& dir);
	~Camera2() {}
	Camera2(const Camera2& other) {}
	const Camera2& operator=(const Camera2& other) {}


	physx::PxVec3	mEye;
	physx::PxVec3	mDir;
	int				mMouseX;
	int				mMouseY;
	//摄像机与小球的距离
	int				mDistance = 20;
public:
	static Camera2* getInstance();

	static void deleteInstance();

	void				updatePosition(physx::PxVec3 pos);

	void				handleMouse(int button, int state, int x, int y);

	void				handleMotion(int x, int y);

	physx::PxVec3		getEye()	const;

	physx::PxVec3		getDir()	const;
};