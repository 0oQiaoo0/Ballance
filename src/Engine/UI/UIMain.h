#pragma once
#include "UIState.h"

class UIMain :public UIState {
public:
	UIMain();
	virtual void CreateUI() override;

private:
	bool show_levels = false;
	bool show_first = true;

	bool ShowLevels(bool);
	bool ShowFirst(bool);

	int btnNum = 2;
	int btnHeight = 192;
	int centerWndWidth = 512, centerWndHeight = 0;
	int btnWidth = 0;
	ImVec2 btnSize = ImVec2(0, 0);
};