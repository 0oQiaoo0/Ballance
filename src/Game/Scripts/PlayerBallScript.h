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

    //�ƶ���
    PxReal force = 0.0f;
    //�ƶ�����
    PxReal torque = 0.0f;

    //�뾶
    PxReal radius = 0.4f;

    //��ʼλ��
    PxVec3 startPosition = { 3.752f, 2.464f, 2.613f };//

    //����λ��
    PxVec3 resetPosition = { 3.752f, 2.464f + 5.0f,2.613f };

    //��ʼ�����ٶ�
    PxVec3 startLinearVelocity = { 0.0f,0.0f,0.0f };

    //��ʼ���ٶ�
    PxVec3 startAngularVelocity = { 0.0f,0.0f,0.0f };

    

    /*****************Wood Ball******************/
    //�ƶ���
    PxReal woodForce = 15.0f;
    //�ƶ�����
    PxReal woodTorque = 6.0f;

    PxReal woodStaticFriction = 0.5f;
    PxReal woodDynamicFriction = 0.5f;
    PxReal woodRestitution = 0.2f;

    PxReal woodMass = 1.9f;
    PxReal woodLinearDamping = 0.34f;
    PxReal woodAngularDamping = 0.9f;
    /*****************Paper Ball******************/
    //�ƶ���
    PxReal paperForce = 2.0f;
    //�ƶ�����
    PxReal paperTorque = 3.0f;

    PxReal paperStaticFriction = 0.2f;
    PxReal paperDynamicFriction = 0.2f;
    PxReal paperRestitution = 0.4f;

    PxReal paperMass = 0.2f;
    PxReal paperLinearDamping = 0.2f;
    PxReal paperAngularDamping = 0.9f;
    /*****************Stone Ball******************/
    //�ƶ���
    PxReal stoneForce = 80.0f;
    //�ƶ�����
    PxReal stoneTorque = 0.0f;

    PxReal stoneStaticFriction = 0.5f;
    PxReal stoneDynamicFriction = 0.5f;
    PxReal stoneRestitution = 0.1f;

    PxReal stoneMass = 20.0f;
    PxReal stoneLinearDamping = 0.6f;
    PxReal stoneAngularDamping = 0.9f;


    // ��һ֡���λ��
    PxVec3 lastBallPos;

    // С���Ŀ��λ��
    PxVec3 targetPosition;


    

    float followSpeed = 0.1f;
    float cameraRotationY = 1.0f;
    float cameraPitch = 1.0f;

    void UpdateCamera();
    void CheckFall();//���С���Ƿ������׹��
    void InputCheck();
    void OnMouseMove(long x, long y);

    // ????��Ϸ�߼�????
    int initLife = 3;               // ��ʼ����ֵ
    int leaveLife = 2;              // ʣ������ֵ
    float initTime = 300.f;         // ��ʼʱ��
    float leaveTime = 300.f;        // ʣ��ʱ��
    float score = 0;                // ����
    float levelBonus = 100;
    float timePoints = 0;           
    float lifePoints = 0;
    bool bFail = false;
    bool bVictory = false;
    bool acceptInput = true;

    // �����棺�����Χ�Ƴ�����ת
    PxVec3 lookAtPoint = PxVec3(-20.f, 10.f, 20.f);
    XMVECTOR worldUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    float distance = 10.f;
    float theta = 0.0f;
    float alpha = 1.0f;


    // ui����
    ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);	// ������ɫ
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
    void Restart();         // ���¿�ʼ��Ϸ
    void Revive();          // �������
    void GetMoreTime(float time);     // ��ü�ʱ����
    void GetMoreLife();     // �������ֵ����
    void Fail();            // ��Ϸʧ�ܣ�ʱ��/�����ľ�
    void Victory();         // ��Ϸʤ��
    void ChangeResetPosition(PxTransform transform);
    void ShowDetailUI() override;   // �༭��ϸ�����
    void ShowUI();                  // ���˱༭����UI
    void ShowGameInfo();            // ��Ϸ��Ϣ��ʱ�䡢����ֵ
    void ShowFailUI();              // ��Ϸʧ�ܵ�UI
    void ShowVictoryUI();           // ��Ϸʤ����UI
    void ResetNewMaterialBall();    // С����ʸı�����һ��������
    void SetTargetPosition(PxVec3);
    void RollToTerminal();
};