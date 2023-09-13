#pragma once
#include "ParticleScript.h"
#include "../../Engine/Framework/Component.h"
#include "../Utils.h"
using namespace std;

class TransformComponent;
class MeshRendererComponent;
class RigidbodyComponent;
class Camera;
class PlayerBallScript : public Component
{
public:
    Utils::BallType playerBallType;
    static float cameraLength;
    static float lensSensitivity;
private:
    TransformComponent* transformComponent;
    MeshRendererComponent* meshRendererComponent;
    RigidbodyComponent* rigidbodyComponent;

    Camera* camera;

    //移动力
    PxReal force = 0.0f;
    //移动力矩
    PxReal torque = 0.0f;

    //半径
    PxReal radius = 0.4f;

    //起始位置
    PxVec3 startPosition = { 3.752f, 2.464f, 2.613f };//

    //重生位置
    PxVec3 resetPosition = { 3.752f, 2.464f + 5.0f,2.613f };

    //初始线性速度
    PxVec3 startLinearVelocity = { 0.0f,0.0f,0.0f };

    //初始角速度
    PxVec3 startAngularVelocity = { 0.0f,0.0f,0.0f };

    

    /*****************Wood Ball******************/
    //移动力
    PxReal woodForce = 15.0f;
    //移动力矩
    PxReal woodTorque = 6.0f;

    PxReal woodStaticFriction = 0.5f;
    PxReal woodDynamicFriction = 0.5f;
    PxReal woodRestitution = 0.2f;

    PxReal woodMass = 1.9f;
    PxReal woodLinearDamping = 0.34f;
    PxReal woodAngularDamping = 0.9f;
    /*****************Paper Ball******************/
    //移动力
    PxReal paperForce = 2.0f;
    //移动力矩
    PxReal paperTorque = 3.0f;

    PxReal paperStaticFriction = 0.2f;
    PxReal paperDynamicFriction = 0.2f;
    PxReal paperRestitution = 0.4f;

    PxReal paperMass = 0.2f;
    PxReal paperLinearDamping = 0.2f;
    PxReal paperAngularDamping = 0.9f;
    /*****************Stone Ball******************/
    //移动力
    PxReal stoneForce = 80.0f;
    //移动力矩
    PxReal stoneTorque = 0.0f;

    PxReal stoneStaticFriction = 0.5f;
    PxReal stoneDynamicFriction = 0.5f;
    PxReal stoneRestitution = 0.1f;

    PxReal stoneMass = 20.0f;
    PxReal stoneLinearDamping = 0.6f;
    PxReal stoneAngularDamping = 0.9f;


    // 上一帧球的位置
    PxVec3 lastBallPos;

    // 小球的目标位置
    PxVec3 targetPosition;


    

    float followSpeed = 0.1f;
    float cameraRotationY = 1.0f;
    float cameraPitch = 1.0f;

    void UpdateCamera();
    void CheckFall();//检查小球是否冲出轨道坠落
    void InputCheck();
    void OnMouseMove(long x, long y);

    // ????游戏逻辑????
    int initLife = 3;               // 初始生命值
    int leaveLife = 2;              // 剩余生命值
    float initTime = 300.f;         // 初始时间
    float leaveTime = 300.f;        // 剩余时间
    float score = 0;                // 分数
    float levelBonus = 100;
    float timePoints = 0;           
    float lifePoints = 0;
    bool bFail = false;
    bool bVictory = false;
    bool acceptInput = true;

    // 主界面：摄像机围绕场景旋转
    PxVec3 lookAtPoint = PxVec3(-20.f, 10.f, 20.f);
    XMVECTOR worldUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    float distance = 10.f;
    float theta = 0.0f;
    float alpha = 1.0f;


    // ui参数
    ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);	// 背景颜色
    ImVec2 wndSize = ImVec2(420, 0);
    ImVec2 titleSize = ImVec2(200, 120);
    ImVec2 btnSize = ImVec2(300, 130);

protected:
    ParticleScript* particleScript=nullptr;
    float lastDeltaTime = 0.016f;
    void Update(float DeltaTime) override;
public:
    void SetBallType(Utils::BallType playerBallType);

    void Start() override;  
    void Restart();         // 重新开始游戏
    void Revive();          // 复活到检查点
    void GetMoreTime(float time);     // 获得加时道具
    void GetMoreLife();     // 获得生命值道具
    void Fail();            // 游戏失败：时间/生命耗尽
    void Victory();         // 游戏胜利
    void ChangeResetPosition(PxTransform transform);
    void ShowDetailUI() override;   // 编辑器细节面板
    void ShowUI();                  // 除了编辑器的UI
    void ShowGameInfo();            // 游戏信息：时间、生命值
    void ShowFailUI();              // 游戏失败的UI
    void ShowVictoryUI();           // 游戏胜利的UI
    void ResetNewMaterialBall();    // 小球材质改变后进行一定的重置
    void SetTargetPosition(PxVec3);
    void RollToTerminal();
};