#pragma once
#include "UIState.h"

class UILoading :public UIState {
public:
	virtual void CreateUI() override;
	
private:
	float mProgress = 0.0f;
};