#pragma once
#include "../../Engine/Framework/Component.h"
class ModelScript : public Component
{
public:
	void Start() override;
	void LoadModel(std::string modelPath, bool _isStatic);
};