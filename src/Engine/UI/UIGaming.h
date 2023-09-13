#pragma once
#include "UIState.h"
#include "UIMain.h"

class UIGaming :public UIState {
private:
	struct MenuConfig {
		int btnNum = 5;
		ImGuiWindowFlags wndFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		int menuWidth = 700, menuHeight = 1000;				// 大窗口大小
		int childX = 0, childY = 0;							// 子容器，为了居中显示，X 由计算得出
		ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);	// 背景颜色
		ImVec2 nextPos = ImVec2(0, 0);						// 大窗口开始位置
		ImVec2 btnSize = ImVec2(480, 160);					// 按钮大小
		void update(int w, int h) {
			// 大窗口居中
			nextPos = ImVec2((w - menuWidth) * 0.5f, (h - menuHeight) * 0.5f);
			// 子窗口在大窗口居中
			childX = (menuWidth - btnSize.x) * 0.5f;
			childY = (menuHeight - btnSize.y * btnNum) * 0.5f;
		}
	} menuConfig;

	struct SettingConfig {
		int itemNum = 4;
		ImGuiWindowFlags wndFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		int width = 600, height = 700;				// 大窗口大小
		int childX = 0, childY = 0;							// 子容器，为了居中显示，X 由计算得出
		ImVec4 bgColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);	// 背景颜色
		ImVec2 nextPos = ImVec2(0, 0);						// 大窗口开始位置
		ImVec2 titleSize = ImVec2(300, 100);
		ImVec2 titlePos = ImVec2(0, 0);

		int btnNum = 2, btnSpace = 50;
		ImVec2 btnSize = ImVec2(200, 100);
		void update(int w, int h) {
			// 大窗口居中
			nextPos = ImVec2((w - width) * 0.5f, (h - height) * 0.5f);
			// 子窗口在大窗口居中
			childX = width * 0.1f;
			childY = height * 0.2f;
			titlePos = ImVec2((width - titleSize.x) * 0.5f, (height - titleSize.y) * 0.1f);
			int btnTotalWidth = btnNum * btnSize.x + btnSpace * (btnNum - 1);
		}
	} settingConfig;

	struct EditorConfig {
		int width = 600, height = 800;
		ImVec4 bgColor = ImVec4(0.0f, 0.0f, 0.0f, 0.2f);
		ImVec2 nextPos = ImVec2(0, 0);
		void update(int w, int h) {
			width = w * 0.3f;
			height = h;
			nextPos = ImVec2(w - width, 0);
		}
	} editorConfig ;
public:

	UIGaming() {
		// 隐藏光标
		while (ShowCursor(FALSE) >= 0);
		// 鼠标保持在中心
		UISystem::cursorInCenter = true;
		UISystem::showCursor = false;
	}
	virtual void CreateUI() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetHWndSize(int, int) override;
	virtual void OnEscDown() override;
protected:
	bool show_fps = true;
	bool show_menu = false;
	bool show_intro = false;
	bool show_setting = false;

	bool ShowFPS(bool&);
	bool ShowMenu(bool&);
	bool ShowIntro(bool&);
	bool ShowSetting(bool&);
	bool ShowEditor();
	bool ShowGameInfo(int life = 3, float time = 100);
private:
	float mFps = 0;
	float mMspf = 0;

	GameObject* selectedObj = nullptr;
};