#pragma once
#include "UIMain.h"
#include "UIGaming.h"
#include "../../src/Engine/Audio/AudioSystem.h"
#include "../src/Engine/GameEngine.h"
#include "../../Game/Entities/PlayerBall.h"
#include "../../src/Game/Scripts/PlayerBallScript.h"
UIMain::UIMain()
{
    AudioSystem::Play(AudioName::main_bgm, true);
}

void UIMain::CreateUI() {

    centerWndHeight = (btnHeight + ImGui::GetStyle().WindowPadding.y * 4) * btnNum;
    btnWidth = centerWndWidth - ImGui::GetStyle().WindowPadding.x * 2;
    btnSize = ImVec2(btnWidth, btnHeight);
    // ����
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(hWndWidth + 64, hWndHeight + 64));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.0f)); // ͸������
    ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleColor();
    ImageInfo& backgroundImage = GetImageByName(UISystem::ImageName::IMG_MAIN_BACKROUND);
    ImGui::Image((ImTextureID)backgroundImage.ImageId.ptr, ImVec2(hWndWidth, hWndHeight));
    ImGui::End();


    // �м䴰��
    ImGui::SetNextWindowPos(ImVec2((hWndWidth - centerWndWidth) * 0.5f, hWndHeight * 0.4f));
    ImGui::SetNextWindowSize(ImVec2(centerWndWidth, 0));
    ImGui::Begin("Center Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);

    if(ShowFirst(show_first) && ShowLevels(show_levels)){}
    
    ImGui::End();

    FadeBright();
}

bool UIMain::ShowLevels(bool show)
{
    if (!show) return true;

    bool ret = true;

    // ���� + ����
    ImageInfo& returnImage = GetImageByName(UISystem::ImageName::IMG_MAIN_LEFT_ARROW);
    ImageInfo& levelImage = GetImageByName(UISystem::ImageName::IMG_MAIN_CHOOSE_LEVEL);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // ��ʱ����ť�ı�����ɫ����Ϊ͸��ɫ
    if (ImGui::ImageButton((ImTextureID)returnImage.ImageId.ptr, ImVec2(140, 70))) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show_levels = false;
        show_first = true;
    }
    ImGui::PopStyleColor(); // �ָ�ԭʼ����ɫֵ

    static float titleWidth = 200, titleHeight = 90;
    ImGui::SameLine((ImGui::GetContentRegionAvail().x - titleWidth) / 2.0f);
    ImGui::Image((ImTextureID)levelImage.ImageId.ptr, ImVec2(titleWidth, titleHeight));



    ImGui::BeginChild("ChildR", ImVec2(0, 300), true, ImGuiWindowFlags_NoTitleBar  | ImGuiTableColumnFlags_NoResize |
    ImGuiWindowFlags_NoBackground);
    


    // ��ť
    if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
    {

        //��ʽ
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

        // todo �����ͬ��ť���벻ͬ�ؿ�
        for (int i = 1; i <= 1; i++)
        {
            char buf[32];
            sprintf(buf, "Level %01d", i);
            ImGui::TableNextColumn();
            if (ImGui::Button(buf, ImVec2(-FLT_MIN, 32.0f))) {
                AudioSystem::Play(AudioName::btn_clicked, false);
                AudioSystem::Stop(AudioName::main_bgm);
                // ����playing
                GetScene()->bPaused = false;
                GetScene()->bPlaying = true;
                GetScene()->GetGameObject<PlayerBall>("PlayerBall")->GetComponent<PlayerBallScript>()->Restart();
                mContext->ChangeState(new UIGaming());  // ���� UIMain
                ret = false;
            }
        }


        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::EndTable();
    }


    ImGui::EndChild();

    return ret;
}

bool UIMain::ShowFirst(bool show)
{
    if (!show) return true;

    ImageInfo& playImage = GetImageByName(UISystem::ImageName::IMG_MAIN_PLAY);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.1f)); // ��ť����͸��
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.2f, 0.3f));  // ��ť��ͣ��ɫ
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    if (ImGui::ImageButton((ImTextureID)playImage.ImageId.ptr, btnSize))
    {
        AudioSystem::Play(AudioName::btn_clicked, false);
       

        show_first = false;
        show_levels = true;

    }
    ImGui::NewLine();
    ImageInfo& exitImage = GetImageByName(UISystem::ImageName::IMG_MAIN_EXIT);
    if (ImGui::ImageButton((ImTextureID)exitImage.ImageId.ptr, btnSize,
        ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)))
    {
        AudioSystem::Play(AudioName::btn_clicked, false);
        // Signal - Exit the program
        PostMessage(InputSystem::GetHwnd(), WM_CLOSE, 0, 0);
    }
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
    return false;
}
