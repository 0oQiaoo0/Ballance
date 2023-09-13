#include "PlayerBallScript.h"

#include <iostream>

#include "../../Engine/Input/InputSystem.h"
#include "../../Engine/UI/UISystem.h"
#include "../../Engine/Audio/AudioSystem.h"
#include "../../Engine/Framework/Scene.h"

#include "../Entities/CameraEntity.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/CameraComponent.h"

#include "../Utils.h"
#include "../../Engine/Particle/ParticleEmitterRate.h"


#include "../../src/Engine/Audio/AudioSystem.h"
#include "../Entities/ParticleEntity.h"

// 初始化静态变量
float PlayerBallScript::cameraLength = 10.0f;
float PlayerBallScript::lensSensitivity = 0.5f;

void PlayerBallScript::Start()
{
    Component::Start();

    transformComponent = GetComponent<TransformComponent>();
    meshRendererComponent = GetComponent<MeshRendererComponent>();
    rigidbodyComponent = GetComponent<RigidbodyComponent>();

    camera = GetScene()->GetGameObject<CameraEntity>()->GetComponent<CameraComponent>()->camera.get();

    //InputSystem::ShowWinCursor(false);
    //bind an event ,must use lambda
    InputSystem::BindMouseEvent(WinMouseEventType::MouseMove, &PlayerBallScript::OnMouseMove, this);
    //InputSystem::BindKeyUpEvent('v', &PlayerBallScript::LogTest, this);

    //设置物理木球并加入物理场景
    transformComponent->SetPosition(startPosition);
    SetBallType(Utils::BallType::Wood);
    //渲染
    meshRendererComponent->RenderDefaultShape(DefaultShape::Sphere);
}

void PlayerBallScript::Restart()
{
    UISystem::HideUICursor();
    leaveLife = 2;
    leaveTime = 300.f;
    score = 0;
    levelBonus = 100;
    timePoints = 0;
    lifePoints = 0;
    bFail = false;
    bVictory = false;
    acceptInput = true;
    transformComponent->SetPosition(startPosition);
    ChangeResetPosition({ startPosition.x,startPosition.y + 5.0f,startPosition.z });
    rigidbodyComponent->SetLinearVelocity(PxVec3(0, 0, 0));
    rigidbodyComponent->SetAngularVelocity(PxVec3(0, 0, 0));
    SetBallType(Utils::BallType::Wood);
    GetScene()->Restart();
}

void PlayerBallScript::Revive()
{
    transformComponent->SetPosition(resetPosition);
    rigidbodyComponent->SetLinearVelocity(PxVec3(0, 0, 0));
    rigidbodyComponent->SetAngularVelocity(PxVec3(0, 0, 0));
    SetBallType(Utils::BallType::Wood);
    if (--leaveLife < 0) {
        Fail();
    }
}

void PlayerBallScript::GetMoreTime(float time)
{
    leaveTime += time;
}

void PlayerBallScript::GetMoreLife()
{
    leaveLife++;
}

void PlayerBallScript::Fail()
{
    bFail = true;
}



void PlayerBallScript::Update(float DeltaTime)
{
    // 游戏暂停也更新摄像机，因为主界面场景摄像机在转
    UpdateCamera();

    // 游戏暂停了不计时、不响应玩家输入
    if (GetScene()->bPaused) {
        return;
    }

    Component::Update(DeltaTime);

    lastDeltaTime = DeltaTime;


    CheckFall();

    if (acceptInput)
    {
        InputCheck();
    }
    else if(!bVictory)
    {
        ResetNewMaterialBall();
    }
    else if (bVictory)
    {
        RollToTerminal();
    }

    if (!bFail && !bVictory) {
        leaveTime -= DeltaTime;
    }
    if (leaveTime <= 0 && !bFail && !bVictory) {
        leaveTime = 0;
        Fail();
    }
}

void PlayerBallScript::SetBallType(Utils::BallType playerBallType)
{
    this->playerBallType = playerBallType;
    switch (playerBallType)
    {
    case Utils::BallType::Paper:
    {
        force = paperForce;
        torque = paperTorque;
        rigidbodyComponent->SetDynamic(PxSphereGeometry(radius), transformComponent->GetWorldTransform(),
            paperStaticFriction, paperDynamicFriction, paperRestitution,
            paperMass, paperLinearDamping, paperAngularDamping,
            startLinearVelocity, startAngularVelocity);
        if (meshRendererComponent->renderItem.IsValid())
        {
            meshRendererComponent->renderItem.Material = DefaultMaterial::Paper;
            meshRendererComponent->renderItem.Update();
        }
        break;
    }
    case Utils::BallType::Wood:
    {
        force = woodForce;
        torque = woodTorque;
        rigidbodyComponent->SetDynamic(PxSphereGeometry(radius), transformComponent->GetWorldTransform(),
            woodStaticFriction, woodDynamicFriction, woodRestitution,
            woodMass, woodLinearDamping, woodAngularDamping,
            startLinearVelocity, startAngularVelocity);
        if (meshRendererComponent->renderItem.IsValid())
        {
            meshRendererComponent->renderItem.Material = DefaultMaterial::Wood;
            meshRendererComponent->renderItem.Update();
        }
        break;
    }
    case Utils::BallType::Stone:
    {
        force = stoneForce;
        torque = stoneTorque;
        rigidbodyComponent->SetDynamic(PxSphereGeometry(radius), transformComponent->GetWorldTransform(),
            stoneStaticFriction, stoneDynamicFriction, stoneRestitution,
            stoneMass, stoneLinearDamping, stoneAngularDamping,
            startLinearVelocity, startAngularVelocity);
        if (meshRendererComponent->renderItem.IsValid())
        {
            meshRendererComponent->renderItem.Material = DefaultMaterial::Tile;
            meshRendererComponent->renderItem.Update();
        }
    }
    break;
    default:break;
    }
}

void PlayerBallScript::UpdateCamera()
{
    if (!camera) return;
    if (GetScene()->bPlaying) {
        // 摄像机相对球的变换
        XMMATRIX transformToBall = XMMatrixTranslation(0, cameraLength, 0) * XMMatrixRotationRollPitchYaw(cameraPitch, cameraRotationY, 0);
        // 当前球的位置
        PxVec3 currentBallPos = transformComponent->GetPosition();

        // 用于取出矩阵位移部分
        XMFLOAT4 tempZeroPos(0.0f, 0.0f, 0.0f, 1.0f);
        XMVECTOR zeroPos = XMLoadFloat4(&tempZeroPos);

        // 插值获得的球的位置
        PxVec3 tempLerpBallPos = lastBallPos + (currentBallPos - lastBallPos) * followSpeed;
        XMVECTOR lerpBallPos = XMVectorSet(tempLerpBallPos.x, tempLerpBallPos.y, tempLerpBallPos.z, 1.0f);

        // 摄像机位置 = 插值的球的位置 + 摄像机相对球的位置
        XMVECTOR cameraPos = XMVector4Transform(zeroPos, transformToBall) + lerpBallPos;

        camera->LookAt(cameraPos, lerpBallPos, worldUp);

        lastBallPos = tempLerpBallPos;
    }
    else {
        theta += 0.003f;
        // 看的点的位置
        XMVECTOR point = XMVectorSet(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, 1.0f);
        // 摄像机相对点的变换
        XMMATRIX transform = XMMatrixTranslation(0, 20, 0) * XMMatrixRotationRollPitchYaw(alpha, theta, 0);

        // 用于取出矩阵位移部分
        XMFLOAT4 tempZeroPos(0.0f, 0.0f, 0.0f, 1.0f);
        XMVECTOR zeroPos = XMLoadFloat4(&tempZeroPos);

        // 摄像机位置 = 点+相对点的变换
        XMVECTOR cameraPos = XMVector4Transform(zeroPos, transform) + point;

        camera->LookAt(cameraPos, point, worldUp);
    }
}

void PlayerBallScript::CheckFall()
{
    PxVec3 BallPosition = transformComponent->GetPosition();
    if(BallPosition.y <= -4.0f)
        //掉落复活前放声音
        AudioSystem::Play(AudioName::fall_down, false);
    if (BallPosition.y <= -25.0f)
    {
        //Todo: 屏幕过渡动画
        GetScene()->InitMovableObject();
        Revive();
    }
}

void PlayerBallScript::InputCheck()
{
    XMFLOAT3 cameraLook = camera->GetLook3f();
    PxVec3 forward = PxVec3(cameraLook.x, 0, cameraLook.z).getNormalized();
    PxVec3 up = PxVec3(0, 1, 0);
    PxVec3 right = up.cross(forward).getNormalized();

    if (InputSystem::GetKey('W'))
    {
        rigidbodyComponent->AddForce(force * forward);
        rigidbodyComponent->AddTorque(torque * right);
    }
    if (InputSystem::GetKey('S'))
    {
        rigidbodyComponent->AddForce(-force * forward);
        rigidbodyComponent->AddTorque(-torque * right);
    }
    if (InputSystem::GetKey('A'))
    {
        rigidbodyComponent->AddForce(-force * right);
        rigidbodyComponent->AddTorque(torque * forward);
    }
    if (InputSystem::GetKey('D'))
    {
        rigidbodyComponent->AddForce(force * right);
        rigidbodyComponent->AddTorque(-torque * forward);
    }

    //debug
    if (InputSystem::GetKey('F'))
    {
        rigidbodyComponent->AddForce(10 * force * up);
    }

    if (InputSystem::GetKey('R'))
    {
        transformComponent->SetPosition(resetPosition);
        //设置线性速度为0
        rigidbodyComponent->SetLinearVelocity(PxVec3(0, 0, 0));
        //设置角速度为0
        rigidbodyComponent->SetAngularVelocity(PxVec3(0, 0, 0));
    }

}

inline float sig(const float f)
{
    return f > 0 ? 1.0f : -1.0f;
}

void PlayerBallScript::OnMouseMove(long x, long y)
{
    // 处于显示鼠标的状态，不响应鼠标移动
    if (UISystem::showCursor) {
        return;
    }

    RECT rect;
    GetClientRect(InputSystem::GetHwnd(), &rect);
    int windowHeight = rect.bottom - rect.top;
    int windowWidth = rect.right - rect.left;

    int mLastMousePosX = windowWidth / 2;
    int mLastMousePosY = windowHeight / 2;

    int dy = mLastMousePosY - y;
    int dx = mLastMousePosX - x;

    if (!(camera->GetLook3f().y >= 0.0 && dy > 0))//Perspective is not at the lowest,mouse does not move down
    {
        if (!(camera->GetLook3f().y <= -0.99 && dy < 0))//Perspective is not at the highest,mouse does not move up
            cameraPitch = cameraPitch + 0.01f * lensSensitivity * static_cast<float>(dy);;
    }

    cameraRotationY += 0.01f * lensSensitivity * static_cast<float>(-dx);

    //std::cout << mLastMousePosX << "+" << mLastMousePosY << std::endl;

    POINT clientPoint;
    clientPoint.x = 0; // 客户区坐标的横向位置
    clientPoint.y = 0; // 客户区坐标的纵向位置


    if (UISystem::cursorInCenter) {
        // 将客户区坐标转换为屏幕坐标
        ClientToScreen(InputSystem::GetHwnd(), &clientPoint);

        // 输出转换后的屏幕坐标
        //printf("屏幕坐标：(%d, %d)\n", clientPoint.x, clientPoint.y);
        //设置鼠标光标位置为上面记录的位置
        SetCursorPos(clientPoint.x + windowWidth / 2, clientPoint.y + windowHeight / 2);
    }
}
std::vector<std::vector<PxTransform>> particlePos =
    {
    {PxTransform(3, 3, 0.5),PxTransform(3, 3, 4.5)},
    {PxTransform(-10,2,44.4),PxTransform(-10,2,47)},
{PxTransform(20.2,0,109),PxTransform(22.8,0,109)},
{PxTransform(58.7,-1,130),PxTransform(58.7,-1,132.8)},
{PxTransform(132.8,-3,137),PxTransform(132.8,-3,142)}
    };
void PlayerBallScript::Victory()
{
    // 计算分数
    // 排行榜？
    score = leaveLife * 100 + leaveTime;
    bVictory = true;
    
    if(particleScript==nullptr)
    {
        particleScript = GetScene()->GetGameObject<ParticleEntity>()->GetComponent<ParticleScript>();
    }
    particleScript->SetEmitterPos1(particlePos[4][0]);
    particleScript->SetEmitterPos2(particlePos[4][1]);
}
void PlayerBallScript::ChangeResetPosition(PxTransform transform)
{
    resetPosition = { transform.p.x,transform.p.y + 5.0f,transform.p.z };
    if(particleScript==nullptr)
    {
        particleScript = GetScene()->GetGameObject<ParticleEntity>()->GetComponent<ParticleScript>();
    }
    int num = 0;
    if(transform.p.z<47&&transform.p.z>44)num =1;
    if(transform.p.x<24&&transform.p.x>20)num =2;
    if(transform.p.z<133&&transform.p.z>130)num =3;
    if(transform.p.z<142&&transform.p.z>137)num =4;
    particleScript->SetEmitterPos1(particlePos[num][0]);
    particleScript->SetEmitterPos2(particlePos[num][1]);

}

void PlayerBallScript::ShowDetailUI()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.9f, 0.2f, 1.0f));
    ImGui::SeparatorText("PlayerBallScript Component");
    ImGui::PopStyleColor(); // 恢复颜色

    // Reset Position
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("Reset Position:"); ImGui::NextColumn();
    ImGui::InputFloat("x##PlayerBallScript StartPosX", &resetPosition.x, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("y##PlayerBallScript StartPosY", &resetPosition.y, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("z##PlayerBallScript StartPosZ", &resetPosition.z, 0.0f);
    ImGui::Columns(1);

    // Move Force
    ImGui::Separator();
    ImGui::Columns(2);
    ImGui::Text("Move Force:"); ImGui::NextColumn();
    ImGui::InputFloat("##PlayerBallScript Move Force", &force, 0.5f);
    ImGui::Columns(1);


    // Camera Distance
    ImGui::Separator();
    ImGui::Columns(2);
    ImGui::Text("Camera Distance:"); ImGui::NextColumn();
    ImGui::InputFloat("##PlayerBallScript Camera Distance", &cameraLength, 0.2f);
    ImGui::Columns(1);

    // Camera Follow Speed
    ImGui::Separator();
    ImGui::Columns(2);
    ImGui::Text("Camera Follow Speed:"); ImGui::NextColumn();
    ImGui::InputFloat("##PlayerBallScript Camera Follow Speed", &followSpeed, 0.05f);
    ImGui::Columns(1);

    // Time
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("ReStart Time:"); ImGui::NextColumn();
    ImGui::InputFloat("##PlayerBallScript Init Time", &initTime); ImGui::NextColumn();
    ImGui::Text("Leave Time:"); ImGui::NextColumn();
    ImGui::InputFloat("##PlayerBallScript Leave Time", &leaveTime);
    ImGui::Columns(1);

    // Life
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("ReStart Life:"); ImGui::NextColumn();
    ImGui::InputInt("##PlayerBallScript Init Life", &initLife, 1); ImGui::NextColumn();
    ImGui::Text("Leave Life:"); ImGui::NextColumn();
    ImGui::InputInt("##PlayerBallScript Leave Life", &leaveLife, 1);
    ImGui::Columns(1);

    ImGui::Separator();

    ImGui::Columns(2);
    ImGui::Checkbox("Fail", &bFail); ImGui::NextColumn();
    ImGui::Checkbox("Victory", &bVictory); ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::Separator();

    if (ImGui::Button("Restart")) {
        Restart();
    }

}

void PlayerBallScript::ShowUI()
{
    ShowGameInfo();
    ShowFailUI();
    ShowVictoryUI();
}

void PlayerBallScript::ShowGameInfo()
{
    static ImVec2 imageSize = ImVec2(50, 50);
    static ImVec2 heartLeftTopPoint = ImVec2(20, 20);
    static ImVec2 heartRightBottomPoint = ImVec2(70, 70);

    ImageInfo& clockImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_CLOCK);
    ImageInfo& heartImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_HEART);
    ImageInfo& ballImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_BALL);

    ImGui::SetNextWindowPos(heartLeftTopPoint);
    ImGui::Begin("GameInfo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);

    ImGui::Image((ImTextureID)heartImage.ImageId.ptr, imageSize,
        ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 0.6f)); ImGui::SameLine();

    for (int i = 1; i <= leaveLife; ++i) {
        ImGui::Image((ImTextureID)ballImage.ImageId.ptr, imageSize); ImGui::SameLine();
    }
    ImGui::NewLine();

    ImGui::Image((ImTextureID)clockImage.ImageId.ptr, imageSize,
        ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 0.6f)); ImGui::SameLine();


    const ImVec4 red = ImVec4(0.9f, 0.1f, 0.1f, 1.0f);
    const ImVec4 green = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);

    float progress = leaveTime / initTime;

    // 根据剩余时间的多少，改变进度条的颜色
    if (progress < 0.3f) {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, red);
    }
    else if (progress < 0.6f) {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogram));    // 默认颜色
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, green);
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

    // 进度条开始绘制的位置
    ImVec2 progressStartPos = ImGui::GetCursorScreenPos();

    std::string timeStr = std::to_string(static_cast<int>(leaveTime));
    static ImVec2 progressBarSize = ImVec2(160, 30);
    ImGui::ProgressBar(progress, progressBarSize, timeStr.c_str());
    // 恢复之前的颜色状态
    ImGui::PopStyleColor();

    if (progress > 1.0f) {
        static ImU32 col_ul = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        static ImU32 col_ur = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        static ImU32 col_br = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
        static ImU32 col_bl = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        float width = progressBarSize.x * (leaveTime - initTime) / initTime;    // 额外进度条的宽度
        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(progressStartPos.x, progressStartPos.y),
            ImVec2(progressStartPos.x + width, progressStartPos.y + progressBarSize.y),
            col_ul, col_ur, col_br, col_bl);
    }

    ImGui::End();

}

void PlayerBallScript::ShowFailUI()
{
    if (!bFail) return;

    UISystem::ShowUICursor();

    ImageInfo& failImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_FAIL);
    ImageInfo& tryAgainImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_TRY_AGAIN);
    ImageInfo& exitImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_EXIT);

    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;


    
    ImVec2 nextPos = ImVec2((screenWidth - wndSize.x) * 0.5f, screenHeight * 0.3f);
    ImGui::SetNextWindowPos(nextPos);
    ImGui::SetNextWindowSize(wndSize);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::Begin("Fail", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleColor();

    ImGui::SameLine((ImGui::GetContentRegionAvail().x - titleSize.x) / 2.0f);
    ImGui::Image((ImTextureID)failImage.ImageId.ptr, titleSize);
  /*  if (leaveTime <= 0) {
        ImGui::Text("Time out!");
    }
    else if (leaveLife <= 0) {
        ImGui::Text("You died!");
    }
    else {
        ImGui::Text("Anyway, you lose.");
    }*/

    ImGui::GetStyle().FrameRounding = 16.0f;    // 圆角
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // 按钮透明背景
   
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 0.3f));  // 按钮悬停颜色
    ImGui::NewLine();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x - btnSize.x) / 2.0f);
    if (ImGui::ImageButton((ImTextureID)tryAgainImage.ImageId.ptr, btnSize)) {
        Restart();
    }
    ImGui::PopStyleColor();



    ImGui::PopStyleColor();
    ImGui::GetStyle().FrameRounding = 0.0f;


    ImGui::End();

    

}

void PlayerBallScript::ShowVictoryUI()
{
    if (!bVictory) return;

    UISystem::ShowUICursor();

    ImageInfo& victoryImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_VICTORY);
    ImageInfo& playAgainImage = UISystem::GetImageByName(UISystem::ImageName::IMG_GAME_PLAY_AGAIN);

    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;

    ImVec2 nextPos = ImVec2((screenWidth - wndSize.x) * 0.5f, screenHeight * 0.3f);
    ImGui::SetNextWindowPos(nextPos);
    ImGui::SetNextWindowSize(ImVec2(wndSize.x, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::Begin("Victory", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleColor();

    ImGui::SameLine((ImGui::GetContentRegionAvail().x - titleSize.x) / 2.0f);   // 居中
    ImGui::Image((ImTextureID)victoryImage.ImageId.ptr, titleSize);
    ImGui::NewLine();

    bool computing = leaveTime > 0 || leaveLife > 0;

    // 分数
    static float elapsedTime = 0;
    if (computing) {
        if (leaveTime > 0) {
            static float timePerFrame = 0.5f;   // 一帧减0.5秒时间
            leaveTime -= leaveTime >= 1 ? 1 : leaveTime;
            timePoints += 1;
        }
        else if(leaveLife > 0) {
            static float secondPerLife = 1.0f;  // 一秒钟减一生命
            elapsedTime += lastDeltaTime / secondPerLife;
            if (elapsedTime >= 1) {
                elapsedTime = 0;
                leaveLife -= 1;
                lifePoints += 200;
            }
        }
        score = levelBonus + timePoints + lifePoints;
    }

    ImGui::Columns(2);
    ImGui::Text("Level Bonus"); ImGui::NextColumn();
    ImGui::Text("%.1f", levelBonus);
    ImGui::Columns(1);

    ImGui::Columns(2);
    ImGui::Text("Time Points"); ImGui::NextColumn();
    ImGui::Text("%.1f", timePoints);
    ImGui::Columns(1);

    ImGui::Columns(2);
    ImGui::Text("Extra Life"); ImGui::NextColumn();
    ImGui::Text("%.1f", lifePoints);
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(2);
    ImGui::Text("Score"); ImGui::NextColumn();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.1f, 1.0f));
    ImGui::Text("%.1f", score);
    ImGui::PopStyleColor();
    ImGui::Columns(1);

    // 按钮
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 32.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // 按钮透明背景
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.5f, 0.2f, 0.3f));  // 按钮悬停颜色
    ImGui::NewLine();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x - btnSize.x) / 2.0f);
    if (!computing) {
        if (ImGui::ImageButton((ImTextureID)playAgainImage.ImageId.ptr, btnSize)) {
            Restart();
        }
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

   

    ImGui::GetStyle().FrameRounding = 0.0f;


    ImGui::End();
}

void PlayerBallScript::ResetNewMaterialBall()
{
    PxVec3 difference = transformComponent->GetPosition() - targetPosition;
    if (difference.magnitude() > 0.05)
    {
        PxVec3 dir = PxVec3(targetPosition.x - transformComponent->GetPosition().x,
            0,
            targetPosition.z - transformComponent->GetPosition().z);
        dir = dir.getNormalized();
        PxVec3 dirForce = dir * force*20.0;
        PxVec3 dirTorque = dir * torque*30.0;
        rigidbodyComponent->AddForce(dirForce);
        rigidbodyComponent->AddTorque(dirTorque);
        difference = transformComponent->GetPosition() - targetPosition;
    }
    else
    {
        //设置线性速度为0
        rigidbodyComponent->SetLinearVelocity(PxVec3(0, 0, 0));
        //设置角速度为0
        rigidbodyComponent->SetAngularVelocity(PxVec3(0, 0, 0));
        acceptInput = true;
        SetBallType(playerBallType);
    }
}

void PlayerBallScript::SetTargetPosition(PxVec3 targetPosition)
{
    this->targetPosition = targetPosition;
    acceptInput = false;

    // 设置线性速度为0
    rigidbodyComponent->SetLinearVelocity(PxVec3(0, 0, 0));
    //设置角速度为0
    rigidbodyComponent->SetAngularVelocity(PxVec3(0, 0, 0));
}

void PlayerBallScript::RollToTerminal()
{
    PxVec3 difference = transformComponent->GetPosition() - targetPosition;
    if (difference.magnitude() > 0.01)
    {
        PxVec3 dir = PxVec3(targetPosition.x - transformComponent->GetPosition().x,
            0,
            targetPosition.z - transformComponent->GetPosition().z);
        dir = dir.getNormalized();
        PxVec3 dirForce = dir * force;
        PxVec3 dirTorque = dir * torque;
        rigidbodyComponent->AddForce(dirForce);
        rigidbodyComponent->AddTorque(dirTorque);
        difference = transformComponent->GetPosition() - targetPosition;
    }
    else
    {
        //设置线性速度为0
        rigidbodyComponent->SetLinearVelocity(PxVec3(0, 0, 0));
        //设置角速度为0
        rigidbodyComponent->SetAngularVelocity(PxVec3(0, 0, 0));
    }
}
