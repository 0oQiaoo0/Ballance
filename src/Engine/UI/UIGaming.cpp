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
    // Show*() return false 说明UI状态改变了，截断后面的Show
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

    // 按E切换鼠标显示/关闭
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
        // 显示光标
        GetScene()->bPaused = true;
        UISystem::ShowUICursor();
    }
    else {
        // 隐藏光标
        GetScene()->bPaused = false;
        UISystem::HideUICursor();
    }
    // TODO: 暂停/继续游戏
}

// 显示帧率
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

// 显示菜单选项
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
    ImGui::BeginChild("btns wnd", ImVec2(0, 0));   // 第二个参数的值为 ImVec2(0,0)，则子容器的大小将自适应其内容。

    // 按钮透明背景
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.2f, 0.3f));  // 按钮悬停颜色
    ImGui::GetStyle().FrameRounding = 16.0f;    // 圆角
    // 菜单
    ImGui::Image((ImTextureID)menuImage.ImageId.ptr, menuConfig.btnSize);
    // 设置
    if (ImGui::ImageButton((ImTextureID)settingImage.ImageId.ptr, menuConfig.btnSize))
    {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show_setting = true;
    }
    // 重新开始
    if (ImGui::ImageButton((ImTextureID)restartImage.ImageId.ptr, menuConfig.btnSize))
    {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show = false;
        GetScene()->bPaused = false;
        UISystem::HideUICursor();
        GetScene()->GetGameObject<PlayerBall>("PlayerBall")->GetComponent<PlayerBallScript>()->Restart();
    }
    // 继续游戏
    if (ImGui::ImageButton((ImTextureID)continueImage.ImageId.ptr, menuConfig.btnSize)) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        // 隐藏鼠标
        GetScene()->bPaused = false;
        UISystem::HideUICursor();
        show = false;
    }
    // 退出游戏
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

    // 标题
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
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 0.4f));  // 按钮悬停颜色
    if (ImGui::ImageButton((ImTextureID)yesImage.ImageId.ptr, settingConfig.btnSize)) {
        AudioSystem::Play(AudioName::btn_clicked, false);
        show = false;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + settingConfig.btnSpace);

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 0.4f));  // 按钮悬停颜色
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
        static char buf[256] = { 0 }; // 初始化一个长度为 256 的字符数组
        static PxVec3 pos = { 0,0,0 };
        static PxVec3 rotate = { 0,0,0 };
        static PxVec3 scale = { 1,1,1 };

        // 定义一个变量来存储用户选择的选项
        static int selectedObjType = 0;

        // 在ImGui窗口中创建下拉框
        ImGui::Text("New object's type:");
        ImGui::SameLine();
        if (ImGui::Combo("##actor_type", &selectedObjType, objTypes, IM_ARRAYSIZE(objTypes)))
        {
        }
        
        // 重置为默认值
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

        // transform参数
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
            // 创建新对象
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
                // 设置默认渲染
                if (auto mesh = newObj->GetComponent<MeshRendererComponent>()) {
                    mesh->RenderDefaultShape(DefaultShape::Box);
                }
                // 设置初始位置
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
        // 限制列表的高度，加滚动条
        ImGui::BeginChild("Table", ImVec2(0, 300), true);

        // 表头，两列
        ImGui::Columns(2, "mycolumns"); // 4-ways, with border
        ImGui::Separator();
        ImGui::Text("Index"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Separator();
        // 内容
        auto scene = GetScene();
        auto& objs = scene->GetAllGameObjects();
        static int selected = -1;
        int i = 0;
        for (auto &p: objs) {
            // 输出键值对
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
            // 添加Component
            if (ImGui::TreeNode("Add Component")) {
                const char* componentTypes[] = { "Rigidbody Component", "MeshRenderer Component"};

                // 定义一个变量来存储用户选择的选项
                static int selectedComponent = 0;

                // 在ImGui窗口中创建下拉框
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

            // 删除按钮
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
