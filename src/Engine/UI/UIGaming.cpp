#pragma once
#include "UIGaming.h"
#include "../../src/Game/Entities/CheckPoint.h"
#include "../../Game/Entities/Obstacle.h"
#include "../../Game/Entities/Model.h"
#include "../../src/Engine/Framework/Components/RigidbodyComponent.h"
#include "../../src/Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Game/Entities/PlayerBall.h"
#include "../../src/Game/Scripts/PlayerBallScript.h"
#include "../Audio/AudioSystem.h"
#include "../../src/Game/Scenes/TestScene.h"
#include "../../Game/Scripts/ObstacleScript.h"

void UIGaming::CreateUI() {
    // Show*() return false ˵��UI״̬�ı��ˣ��ضϺ����Show
    if (ShowFPS(show_fps) &&
        ShowMenu(show_menu) &&
        ShowIntro(show_intro) &&
        ShowSetting(show_setting) &&
        ShowEditor()&&
        ShowGameInfo())
    {
        FadeBright();
    }
    
}

void UIGaming::Tick(float DeltaTime)
{
    mFps = 1.0f / DeltaTime;
    mMspf = 1000.f / mFps;

    // ��E�л������ʾ/�ر�
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsKeyPressed(ImGuiKey_E))
    {
        if (UISystem::showCursor) {
            UISystem::HideUICursor();
        }
        else {
            UISystem::ShowUICursor();
        }
    }
}

void UIGaming::SetHWndSize(int w, int h)
{
    UIState::SetHWndSize(w, h);

    menuConfig.update(w, h);
    settingConfig.update(w, h);
    editorConfig.update(w, h);
}

void UIGaming::OnEscDown()
{
    show_menu = !show_menu;
    if (show_menu) {
        // ��ʾ���
        GetScene()->bPaused = true;
        UISystem::ShowUICursor();
    }
    else {
        // ���ع��
        GetScene()->bPaused = false;
        UISystem::HideUICursor();
    }
    // TODO: ��ͣ/������Ϸ
}

// ��ʾ֡��
bool UIGaming::ShowFPS(bool& show)
{
    if (!show) {
        return true;
    }
    static int infoWidth = 200, infoHeight = 25;
    ImGui::SetNextWindowPos(ImVec2(0, hWndHeight - infoHeight));
    ImGui::SetNextWindowSize(ImVec2(infoWidth, infoHeight));
    ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("Fps: %.1f, Mspf: %.1f ms", mFps, mMspf);
    ImGui::End();
    return true;
}

// ��ʾ�˵�ѡ��
bool UIGaming::ShowMenu(bool& show)
{
    if (!show) {
        return true;
    }
    bool ret = true;

    ImageInfo& menuImage = GetImageByName(UISystem::ImageName::IMG_GAME_MENU);
    ImageInfo& settingImage = GetImageByName(UISystem::ImageName::IMG_GAME_SETTING);
    ImageInfo& restartImage = GetImageByName(UISystem::ImageName::IMG_GAME_RESTART);
    ImageInfo& continueImage = GetImageByName(UISystem::ImageName::IMG_GMAE_CONTINUE);
    ImageInfo& exitImage = GetImageByName(UISystem::ImageName::IMG_GAME_EXIT);


    ImGui::SetNextWindowPos(menuConfig.nextPos);
    ImGui::SetNextWindowSize(ImVec2(menuConfig.menuWidth, menuConfig.menuHeight));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, menuConfig.bgColor);
    ImGui::Begin("Menu", nullptr, menuConfig.wndFlags);
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(menuConfig.childX, menuConfig.childY));
    ImGui::BeginChild("btns wnd", ImVec2(0, 0));   // �ڶ���������ֵΪ ImVec2(0,0)�����������Ĵ�С������Ӧ�����ݡ�

    // ��ť͸������
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.2f, 0.3f));  // ��ť��ͣ��ɫ
    ImGui::GetStyle().FrameRounding = 16.0f;    // Բ��
    // �˵�
    ImGui::Image((ImTextureID)menuImage.ImageId.ptr, menuConfig.btnSize);
    // ����
    if (ImGui::ImageButton((ImTextureID)settingImage.ImageId.ptr, menuConfig.btnSize))
    {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show_setting = true;
    }
    // ���¿�ʼ
    if (ImGui::ImageButton((ImTextureID)restartImage.ImageId.ptr, menuConfig.btnSize))
    {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show = false;
        GetScene()->bPaused = false;
        UISystem::HideUICursor();
        GetScene()->GetGameObject<PlayerBall>("PlayerBall")->GetComponent<PlayerBallScript>()->Restart();
    }
    // ������Ϸ
    if (ImGui::ImageButton((ImTextureID)continueImage.ImageId.ptr, menuConfig.btnSize)) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        // �������
        GetScene()->bPaused = false;
        UISystem::HideUICursor();
        show = false;
    }
    // �˳���Ϸ
    if (ImGui::ImageButton((ImTextureID)exitImage.ImageId.ptr, menuConfig.btnSize)) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        GetScene()->bPlaying = false;
        if (auto scene = dynamic_cast<TestScene*>(GetScene())) {
            scene->StopBgm();
        }
        mContext->ChangeState(new UIMain());
        ret = false;
    }
    ImGui::GetStyle().FrameRounding = 0.0f;

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();



    ImGui::EndChild();

    ImGui::End();
    return ret;

}

bool UIGaming::ShowIntro(bool&)
{
    return true;
}

bool UIGaming::ShowSetting(bool& show)
{
    if (!show) return true;
    ImageInfo& settingImage = GetImageByName(UISystem::ImageName::IMG_GAME_SETTING);
   

    ImGui::SetNextWindowPos(settingConfig.nextPos);
    ImGui::SetNextWindowSize(ImVec2(settingConfig.width, settingConfig.height));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, settingConfig.bgColor);
    ImGui::Begin("Setting", nullptr, settingConfig.wndFlags);
    ImGui::PopStyleColor();

    // ����
    ImGui::SetCursorPos(settingConfig.titlePos);
    ImGui::Image((ImTextureID)settingImage.ImageId.ptr, settingConfig.titleSize);

    ImGui::SetCursorPos(ImVec2(settingConfig.childX, settingConfig.childY));
    ImGui::BeginChild("items wnd", ImVec2(0, 0));

    ImGuiStyle oldStyle = ImGui::GetStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    style.GrabMinSize = 24;
    style.GrabRounding = 12.0f;
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 12.0f;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 80.0f);
    ImGui::Checkbox("Display FPS ", &show_fps);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50.0f);
    ImGui::Text("Lens Sensitivity");
    ImGui::SameLine();
    ImGui::SliderFloat("##1", &PlayerBallScript::lensSensitivity, 0.1f, 1.0f, "%.2f");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50.0f);
    ImGui::Text("Camera Distance ");
    ImGui::SameLine();
    ImGui::SliderFloat("##2", &PlayerBallScript::cameraLength, 5.f, 20.f,  "%.2f");

    style = oldStyle;

    ImageInfo& yesImage = GetImageByName(UISystem::ImageName::IMG_BTN_YES);
    ImageInfo& noImage = GetImageByName(UISystem::ImageName::IMG_BTN_NO);


    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 80.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 0.4f));  // ��ť��ͣ��ɫ
    if (ImGui::ImageButton((ImTextureID)yesImage.ImageId.ptr, settingConfig.btnSize)) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show = false;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + settingConfig.btnSpace);

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 0.4f));  // ��ť��ͣ��ɫ
    if (ImGui::ImageButton((ImTextureID)noImage.ImageId.ptr, settingConfig.btnSize)) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show = false;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::End();
    return true;

}

bool UIGaming::ShowEditor()
{
    ImGui::SetNextWindowPos(editorConfig.nextPos);
    ImGui::SetNextWindowSize(ImVec2(editorConfig.width, editorConfig.height));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, editorConfig.bgColor);
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::PopStyleColor();


    if (ImGui::CollapsingHeader("Operation"))
    {
        const char* objTypes[] = { "Obstacle", "Model" };
        static char buf[256] = { 0 }; // ��ʼ��һ������Ϊ 256 ���ַ�����
        static PxVec3 pos = { 0,0,0 };
        static PxVec3 rotate = { 0,0,0 };
        static PxVec3 scale = { 1,1,1 };

        // ����һ���������洢�û�ѡ���ѡ��
        static int selectedObjType = 0;

        // ��ImGui�����д���������
        ImGui::Text("New object's type:");
        ImGui::SameLine();
        if (ImGui::Combo("##actor_type", &selectedObjType, objTypes, IM_ARRAYSIZE(objTypes)))
        {
        }
        
        // ����ΪĬ��ֵ
        ImGui::Text("Set parameters for new actors:");
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            memset(buf, 0, sizeof(buf));
            pos = { 0,0,0 };
            rotate = { 0,0,0 };
            scale = { 1,1,1 };
        }



        // Name
        ImGui::Separator();
        ImGui::Columns(2);
        ImGui::Text("Name:"); ImGui::NextColumn();
        ImGui::InputText("##name", buf, sizeof(buf));
        ImGui::Columns(1);

        // transform����
        // position
        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("Position"); ImGui::NextColumn();
        ImGui::InputFloat("x##operation posx", &pos.x); ImGui::NextColumn();
        ImGui::InputFloat("y##operation posy", &pos.y); ImGui::NextColumn();
        ImGui::InputFloat("z##operation posz", &pos.z);
        ImGui::Columns(1);
        // rotation
        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("Rotation"); ImGui::NextColumn();
        ImGui::InputFloat("x##operation rotx", &rotate.x); ImGui::NextColumn();
        ImGui::InputFloat("y##operation rotx", &rotate.y); ImGui::NextColumn();
        ImGui::InputFloat("z##operation rotz", &rotate.z);
        ImGui::Columns(1);
        // scale
        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("Scale"); ImGui::NextColumn();
        ImGui::InputFloat("x##operation scalex", &scale.x); ImGui::NextColumn();
        ImGui::InputFloat("y##operation scalex", &scale.y); ImGui::NextColumn();
        ImGui::InputFloat("z##operation scalez", &scale.z);
        ImGui::Columns(1);
        ImGui::Separator();


        if (ImGui::Button("Add object to scene")) {
            std::string name(buf);
            // �����¶���
            GameObject* newObj = nullptr;
            switch (selectedObjType)
            {
            case 0:
                newObj = GetScene()->AddGameObject<Obstacle>(name);
                break;
            case 1:
                newObj = GetScene()->AddGameObject<Model>(name);
                break;
            default:
                break;
            }
            if (newObj) {
                // ����Ĭ����Ⱦ
                if (auto mesh = newObj->GetComponent<MeshRendererComponent>()) {
                    mesh->RenderDefaultShape(DefaultShape::Box);
                }
                // ���ó�ʼλ��
                if (auto tsf = newObj->GetComponent<TransformComponent>()) {
                    tsf->SetPosition(pos);
                    tsf->SetRotation(rotate);
                    tsf->SetScale(scale);
                }

            }
        }


    }


    if (ImGui::CollapsingHeader("Objects"))
    {
        // �����б�ĸ߶ȣ��ӹ�����
        ImGui::BeginChild("Table", ImVec2(0, 300), true);

        // ��ͷ������
        ImGui::Columns(2, "mycolumns"); // 4-ways, with border
        ImGui::Separator();
        ImGui::Text("Index"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Separator();
        // ����
        auto scene = GetScene();
        auto& objs = scene->GetAllGameObjects();
        static int selected = -1;
        int i = 0;
        for (auto &p: objs) {
            // �����ֵ��
            char label[32];
            sprintf(label, "%04d", i);

            if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns)) {
                selected = i;
                selectedObj = p.second.get();
            }

            ImGui::NextColumn();

            ImGui::Text(p.first.c_str()); ImGui::NextColumn();

            ++i;
        }
        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::EndChild();
    }


    if (ImGui::CollapsingHeader("Detail"))
    {   
        if (selectedObj) {
            // ���Component
            if (ImGui::TreeNode("Add Component")) {
                const char* componentTypes[] = { "Rigidbody Component", "MeshRenderer Component"};

                // ����һ���������洢�û�ѡ���ѡ��
                static int selectedComponent = 0;

                // ��ImGui�����д���������
                ImGui::Text("Select Component Type:");
                ImGui::SameLine();
                if (ImGui::Combo("##Component Type", &selectedComponent, componentTypes, IM_ARRAYSIZE(componentTypes)))
                {
                }

                if (ImGui::Button("Add component")) {
                    switch (selectedComponent)
                    {
                    case 0:
                        selectedObj->AddComponent<RigidbodyComponent>(); break;
                    case 1:
                        selectedObj->AddComponent<MeshRendererComponent>(); break;
                    default:break;
                    }
                }

                ImGui::TreePop();
            }
            

            selectedObj->ShowDetailUI();

            // ɾ����ť
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            if (ImGui::Button("Delete object")) {
                GetScene()->DeleteGameObject(selectedObj->name);//TODO
                selectedObj = nullptr;
            }
            ImGui::PopStyleColor();

        }

    }
    ImGui::End();

    return true;
}

bool UIGaming::ShowGameInfo(int life, float time)
{
    if (GetScene()->GetGameObject<PlayerBall>()) {
        auto ball = dynamic_cast<PlayerBall*>(GetScene()->GetGameObject<PlayerBall>(std::string("PlayerBall")));
        ball->ShowGameUI();
    }
    return true;
}
