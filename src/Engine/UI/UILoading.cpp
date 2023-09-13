#pragma once
#include "UIMain.h"
#include "UIGaming.h"
#include "UILoading.h"
void UILoading::CreateUI() {
    // ����
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(hWndWidth + 64, hWndHeight + 64));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // ��͸������
    ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleColor();


    mProgress += 0.02f;
    if (mProgress >= 1.0f) {
        mContext->ChangeState(new UIMain());
    }

    // ������ʾ������
    float barWidth = ImGui::GetWindowWidth() * 0.5f;
    float barHeight = 50.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - barWidth) * 0.5f);
    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - barHeight) * 0.5f);
    ImGui::ProgressBar(mProgress, ImVec2(barWidth, barHeight), "Loading...");

    ImGui::End();
    
}
