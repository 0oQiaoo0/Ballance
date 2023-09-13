#include "UIState.h"


void UIState::SetHWndSize(int w, int h)
{
	hWndWidth = w;
	hWndHeight = h;
}

ImageInfo& UIState::GetImageByName(UISystem::ImageName name)
{
	return mContext->GetImageByName(name);
}

Scene* UIState::GetScene()
{
	return mContext->GetScene();
}

void UIState::FadeDark()
{
	if (is_fading) {
		fade_progress += lastDeltaTime / fade_time;
		ImVec4 color(0.0f, 0.0f, 0.0f, is_black ? fade_progress : 1.0f - fade_progress);
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, ImGui::ColorConvertFloat4ToU32(color));
	}
}

void UIState::FadeBright()
{
	if (!is_fading) return;

	fade_progress += lastDeltaTime / fade_time;

	if (fade_progress >= 1.0f) {
		fade_progress = 1.0f;
		is_fading = false;
	}

	ImVec4 color(0.0f, 0.0f, 0.0f, is_black ? fade_progress : 1.0f - fade_progress);
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, ImGui::ColorConvertFloat4ToU32(color));

	if (fade_progress >= 1.0f) {
		is_black = !is_black;
	}
}

void UIState::CursorMoveX(float dt)
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + dt);
}

void UIState::CursorMoveY(float dt)
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + dt);

}

inline void UIState::CursorMoveXY(float dx, float dy)
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + dx);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + dy);
}

