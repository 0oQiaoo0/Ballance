#pragma once
#include "UISystem.h"
#include "../Render/Pass/UIPass.h"
#include "../Input/InputSystem.h"

/*
	状态转换：
	UILoading ---加载完毕---> UIMain ---开始游戏---> UIGaming
									 <---退出游戏---	
*/



class UISystem;
struct ImageInfo;

class UIState {
public:
	void setContext(UISystem* context) {
		this->mContext = context;
	}
	virtual void CreateUI() = 0;
	virtual void Tick(float DeltaTime) {
		lastDeltaTime = DeltaTime;
	};
	virtual void OnEscDown() {};
	virtual void SetHWndSize(int, int);
	virtual ~UIState() {};

	ImageInfo& GetImageByName(UISystem::ImageName name);
	Scene* GetScene();
protected:
	bool is_black = false;
	bool is_fading = true;
	float fade_time = 0.5f;			// 过渡时长
	float fade_progress = 0.0f;
	// 渐渐变暗，！未完成！
	void FadeDark();
	// 渐渐变亮
	void FadeBright();

	inline void CursorMoveX(float dt);
	inline void CursorMoveY(float dt);
	inline void CursorMoveXY(float dx, float dy);

protected:
	UISystem* mContext;
	int hWndWidth = 0;
	int hWndHeight = 0;
	float lastDeltaTime = 0.016f;
};