#pragma once
#include "../../Utils/Utils.h"
#include "../Render/Render.h"
#include "PhysicsManager.h"
#include "Camera2.h"
#include "Enums.h"
#include <ctype.h>

using namespace physx;
using namespace std;
class InputManager
{
private:
	static InputManager* instance;

	InputManager() {}
	~InputManager() {}
	InputManager(const InputManager& other) {}
	const InputManager& operator=(const InputManager& other) {}

	bool keyState[256] = {};
public:
	PxReal stackZ = 10.0f;

	static InputManager* getInstance();

	static void deleteInstance();

	void keyboardCallback(unsigned char key, int x, int y);

	void keyboardUpCallback(unsigned char key, int x, int y);

	void InputUpdate();

};
