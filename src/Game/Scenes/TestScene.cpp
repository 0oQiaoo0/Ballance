#include "TestScene.h"

#include "../../Engine/core/ThreadPool.h"
#include "../Entities/CameraEntity.h"
#include "../Entities/PlayerBall.h"
#include "../Entities/Model.h"
#include "../Entities/CheckPoint.h"
#include "../Entities/MyTrigger.h"
#include "../Entities/Obstacle.h"
#include "../Entities/Bridge.h"
#include "../Entities/GameProp.h"
#include "../Entities/Fence.h"

#include "../../Engine/Net/NetManager.h"
#include "../Entities/Door.h"
#include "../Entities/GameProp.h"
#include "../Entities/Transformer.h"
#include "../Entities/RobotBall.h"
#include "../Entities/Hammer.h"
#include "../Entities/RobotBallTrigger.h"
#include "../Entities/SampleCharacterClothFlag.h"

#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../Entities/ParticleEntity.h"

#include "../Scripts/ModelScript.h"
#include "../Scripts/ObstacleScript.h"
#include "../Scripts/TransformerScript.h"
#include "../Scripts/BridgeScript.h"
#include "../Scripts/CheckPointScript.h"
#include "../Scripts/DoorScript.h"
#include "../Scripts/HammerScript.h"
#include "../Scripts/GamePropScript.h"
#include "../Scripts/RBallTriggerScript.h"
#include "../Scripts/FenceScript.h"

void TestScene::Start()
{
    Scene::Start();
    
    AddGameObject<CameraEntity>("Camera");

    AddGameObject<PlayerBall>("PlayerBall");

    AddGameObject<CheckPoint>("CheckPoint");

    AddGameObject<MyTrigger>("MyTrigger");

    auto level_1 = AddGameObject<Model>("Level_1");
    level_1->GetComponent<TransformComponent>()->SetRotation(PxVec3(-90, 0, 0));
    level_1->GetComponent<MeshRendererComponent>()->RenderModel("../../src/Asset/Models/level01.fbx");
    level_1->GetComponent<ModelScript>()->LoadModel("../../src/Asset/Models/level01.fbx", true);

    //At the long steel wire
    DeleteGameObject("Hammer_1");
    auto hammer_1 = AddGameObject<Hammer>("Hammer_1");
    hammer_1->GetComponent<HammerScript>()->CreateHammerDefault(PxTransform(PxVec3(-29.0f, 3.2f, 42.757f)));
    DeleteGameObject("Hammer_2");
    auto hammer_2 = AddGameObject<Hammer>("Hammer_2");
    hammer_2->GetComponent<HammerScript>()->CreateHammer(PxTransform(PxVec3(-25.0f, 3.5f, 42.757f)), 1.5f, 2.0);
    DeleteGameObject("Hammer_3");
    auto hammer_3 = AddGameObject<Hammer>("Hammer_3");
    hammer_3->GetComponent<HammerScript>()->CreateHammerDefault(PxTransform(PxVec3(-21.0f, 3.2f, 42.757f)));

    auto stoneTransformer_1 = AddGameObject<Transformer>("StoneTransformer_1");
    stoneTransformer_1->GetComponent<TransformerScript>()->SetTransformer(Utils::BallType::Stone, PxTransform(PxVec3(-43.0f, 2.1f, 16.2f)));
    auto stoneTransformer_2 = AddGameObject<Transformer>("StoneTransformer_2");
    stoneTransformer_2->GetComponent<TransformerScript>()->SetTransformer(Utils::BallType::Stone, PxTransform(PxVec3(21.5f, -1.1f, 75.0f)));

    auto woodTransformer_1 = AddGameObject<Transformer>("WoodTransformer_1");
    woodTransformer_1->GetComponent<TransformerScript>()->SetTransformer(Utils::BallType::Wood, PxTransform(PxVec3(-35.85f, 2.8f, 25.3f)));
    auto woodTransformer_2 = AddGameObject<Transformer>("WoodTransformer_2");
    woodTransformer_2->GetComponent<TransformerScript>()->SetTransformer(Utils::BallType::Paper, PxTransform(PxVec3(50.95f, -2.2f, 131.68f)));

    AddGameObject<ParticleEntity>();
    //NetManager::GetInstance()->HostGame();
    //ball->AddToNet(Owner);
    /*
    ThreadPool::AddTask([this]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(14));
        AddGameObject<Obstacle>("Ob");
    });
  */

    InitMovableObject();
    InitGameProp();
    //AddGameObject<SampleCharacterClothFlag>("flag");
}

void TestScene::Update(float DeltaTime)
{
    Scene::Update(DeltaTime);

    PlayBgm(DeltaTime);
}

void TestScene::Restart()
{
    InitMovableObject();
    GetGameObject<CheckPoint>("CheckPoint")->GetComponent<CheckPointScript>()->Restart();
}

void TestScene::InitMovableObject()
{
    //Three boxes beside the robot ball
    DeleteGameObject("Box_1");
    auto box_1 = AddGameObject<Obstacle>("Box_1");
    box_1->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(-34.2f, 3.088f, 3.088f)));
    DeleteGameObject("Box_2");
    auto box_2 = AddGameObject<Obstacle>("Box_2");
    box_2->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(-34.2f, 3.088f, 2.0f)));
    DeleteGameObject("Box_3");
    auto box_3 = AddGameObject<Obstacle>("Box_3");
    box_3->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(-36.2f, 3.088f, -0.30f)));
    //Two boxes hide the timeprop
    DeleteGameObject("Box_4");
    auto box_4 = AddGameObject<Obstacle>("Box_4");
    box_4->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(25.35f, -2.069f, 81.65f)));
    DeleteGameObject("Box_5");
    auto box_5 = AddGameObject<Obstacle>("Box_5");
    box_5->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(25.35f, -1.069f, 81.65f)));
    //Two boxes beside the gap
    DeleteGameObject("Box_6");
    auto box_6 = AddGameObject<Obstacle>("Box_6");
    box_6->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(23.45f, -2.069f, 80.74f)));
    DeleteGameObject("Box_7");
    auto box_7 = AddGameObject<Obstacle>("Box_7");
    box_7->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(19.59f, -2.069f, 80.74f)));
    //Two boxes to fill the gap
    DeleteGameObject("Box_8");
    auto box_8 = AddGameObject<Obstacle>("Box_8");
    box_8->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(21.5f, -2.069f, 80.7f)));
    DeleteGameObject("Box_9");
    auto box_9 = AddGameObject<Obstacle>("Box_9");
    box_9->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(21.5f, -1.069f, 80.7f)));
    //The box at the top of the hill
    DeleteGameObject("Box_10");
    auto box_10 = AddGameObject<Obstacle>("Box_10");
    box_10->GetComponent<ObstacleScript>()->SetBoxDefault(PxTransform(PxVec3(21.388f, 2.24f, 97.14f)));


    //On a platform with a hole
    DeleteGameObject("PaperBall_1");
    auto paperBall_1 = AddGameObject<Obstacle>("PaperBall_1");
    paperBall_1->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(-35.2f, 2.757f, 41.8f)));
    DeleteGameObject("PaperBall_2");
    auto paperBall_2 = AddGameObject<Obstacle>("PaperBall_2");
    paperBall_2->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(-36.5f, 2.757f, 41.8f)));
    DeleteGameObject("PaperBall_3");
    auto paperBall_3 = AddGameObject<Obstacle>("PaperBall_3");
    paperBall_3->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(-34.5f, 2.757f, 42.5f)));
    DeleteGameObject("PaperBall_4");
    auto paperBall_4 = AddGameObject<Obstacle>("PaperBall_4");
    paperBall_4->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(-35.8f, 2.757f, 43.5f)));
    //With a row of stone balls
    DeleteGameObject("PaperBall_5");
    auto paperBall_5 = AddGameObject<Obstacle>("PaperBall_5");
    paperBall_5->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(28.2f, 0.266f, 123.17f)));
    //before end
    DeleteGameObject("PaperBall_6");
    auto paperBall_6 = AddGameObject<Obstacle>("PaperBall_6");
    paperBall_6->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(87.124f, 13.753f, 147.312f)));
    DeleteGameObject("PaperBall_7");
    auto paperBall_7 = AddGameObject<Obstacle>("PaperBall_7");
    paperBall_7->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(93.88f, 9.562f, 140.02f)));
    DeleteGameObject("PaperBall_8");
    auto paperBall_8 = AddGameObject<Obstacle>("PaperBall_8");
    paperBall_8->GetComponent<ObstacleScript>()->SetPaperBallDefault(PxTransform(PxVec3(113.87f, -1.869f, 147.28f)));



    //the ball to fill the hole
    DeleteGameObject("WoodBall_1");
    auto woodBall_1 = AddGameObject<Obstacle>("WoodBall_1");
    woodBall_1->GetComponent<ObstacleScript>()->SetWoodBall(PxTransform(PxVec3(-18.34f, 3.285f, 42.757f)), 0.3f, 0.3f);
    //before end
    DeleteGameObject("WoodBall_2");
    auto woodBall_2 = AddGameObject<Obstacle>("WoodBall_2");
    woodBall_2->GetComponent<ObstacleScript>()->SetWoodBall(PxTransform(PxVec3(86.46f, 13.753f, 147.95f)), 0.3f, 0.3f);
    DeleteGameObject("WoodBall_3");
    auto woodBall_3 = AddGameObject<Obstacle>("WoodBall_3");
    woodBall_3->GetComponent<ObstacleScript>()->SetWoodBall(PxTransform(PxVec3(100.96f, 7.276f, 140.15f)), 0.3f, 0.3f);

    //A row of stone balls
    DeleteGameObject("StoneBall_1");
    auto stoneBall_1 = AddGameObject<Obstacle>("StoneBall_1");
    stoneBall_1->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(28.2f, 0.266f, 126.37f)));
    DeleteGameObject("StoneBall_2");
    auto stoneBall_2 = AddGameObject<Obstacle>("StoneBall_2");
    stoneBall_2->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(28.2f, 0.266f, 125.57f)));
    DeleteGameObject("StoneBall_3");
    auto stoneBall_3 = AddGameObject<Obstacle>("StoneBall_3");
    stoneBall_3->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(28.2f, 0.266f, 124.77f)));
    DeleteGameObject("StoneBall_4");
    auto stoneBall_4 = AddGameObject<Obstacle>("StoneBall_4");
    stoneBall_4->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(28.2f, 0.266f, 123.97f)));
    DeleteGameObject("StoneBall_5");
    auto stoneBall_5 = AddGameObject<Obstacle>("StoneBall_5");
    stoneBall_5->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(28.2f, 0.266f, 122.37f)));
    //before end
    DeleteGameObject("StoneBall_6");
    auto stoneBall_6 = AddGameObject<Obstacle>("StoneBall_6");
    stoneBall_6->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(93.63f, 11.848f, 147.62f)));
    DeleteGameObject("StoneBall_7");
    auto stoneBall_7 = AddGameObject<Obstacle>("StoneBall_7");
    stoneBall_7->GetComponent<ObstacleScript>()->SetStoneBallDefault(PxTransform(PxVec3(100.92f, 4.99f, 147.8f)));

    //at start
    DeleteGameObject("Bridge_1");
    auto bridge_1 = AddGameObject<Bridge>("Bridge_1");
    bridge_1->GetComponent<BridgeScript>()->CreateBridge(PxTransform(PxVec3(-10.6f, 2.5f, 8.3f), PxQuat(PxPi / 2,PxVec3(0, 1, 0))), 13);
    //beside first stone transformer
    DeleteGameObject("Bridge_2");
    auto bridge_2 = AddGameObject<Bridge>("Bridge_2");
    bridge_2->GetComponent<BridgeScript>()->CreateBridge(PxTransform(PxVec3(-37.5f, 2.9f, 34.2f)), 12);
    
    //at start
    DeleteGameObject("Door_1");
    auto door_1 = AddGameObject<Door>("Door_1");
    door_1->GetComponent<DoorScript>()->CreateDoorDefault(PxTransform(PxVec3(-18.75f, 3.0f, 8.2f)));

    //The first platform after leaving the starting platform
    DeleteGameObject("RobotBall_1");
    auto robotBall_1 = AddGameObject<RobotBall>("RobotBall_1");   

    //fence_1
    DeleteGameObject("Fence_1");
    auto fence_1 = AddGameObject<Fence>("Fence_1");
    fence_1->GetComponent<FenceScript>()->CreateFenceDefault(PxTransform(PxVec3(43.4f, -1.8f, 124.516f)));
    //fence_2
    DeleteGameObject("Fence_2");
    auto fence_2 = AddGameObject<Fence>("Fence_2");
    fence_2->GetComponent<FenceScript>()->CreateFenceDefault(PxTransform(PxVec3(45.3f, -1.8f, 124.516f)));
    DeleteGameObject("RobotBallTrigger_1");
    auto robotBallTrigger_1 = AddGameObject<RobotBallTrigger>("RobotBallTrigger_1");
    robotBallTrigger_1->GetComponent<RBallTriggerScript>()->CreateRBallTrigger(PxTransform(PxVec3(-30.5f, 5.3f, 2.5f)));
}

void TestScene::InitGameProp()
{
    //beside the robot ball
    auto lifeProp_1 = AddGameObject<GameProp>("LifeProp_1");
    lifeProp_1->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Life, PxVec3(0.4,0.4,0.4),PxTransform(PxVec3(-39.15f, 3.0f, 0.0f)));
    //On a platform with many boxes
    auto lifeProp_2 = AddGameObject<GameProp>("LifeProp_2");
    lifeProp_2->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Life,PxVec3(0.4,0.4,0.4), PxTransform(PxVec3(21.5f, -1.11f, 81.7f)));
    //Between the two fences
    auto lifeProp_3 = AddGameObject<GameProp>("LifeProp_3");
    lifeProp_3->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Life, PxVec3(0.4,0.4,0.4),PxTransform(PxVec3(44.37f, -1.219f, 124.39f)));
    //Big Uphill before end
    auto lifeProp_4 = AddGameObject<GameProp>("LifeProp_4");
    lifeProp_4->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Life, PxVec3(0.4,0.4,0.4),PxTransform(PxVec3(77.127f, 11.422f, 147.57f)));

    PxQuat TimePropQuat =  PxQuat(PxPi/2,{-1,0,0});
    //beside four paper balls
    auto timeProp_1 = AddGameObject<GameProp>("TimeProp_1");
    timeProp_1->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time, PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(-40.3f, 2.757f, 45.86f),TimePropQuat), 30.0f);
    //At the long steel wire
    auto timeProp_2 = AddGameObject<GameProp>("TimeProp_2");
    timeProp_2->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time,PxVec3(0.04,0.04,0.04), PxTransform(PxVec3(-21.0f, 3.285f, 42.757f),TimePropQuat), 30.0f);
    auto timeProp_3 = AddGameObject<GameProp>("TimeProp_3");
    timeProp_3->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time,PxVec3(0.04,0.04,0.04), PxTransform(PxVec3(-25.0f, 3.285f, 42.757f),TimePropQuat), 30.0f);
    auto timeProp_4 = AddGameObject<GameProp>("TimeProp_4");
    timeProp_4->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time,PxVec3(0.04,0.04,0.04), PxTransform(PxVec3(-29.0f, 3.285f, 42.757f),TimePropQuat), 30.0f);
    //before the pot
    auto timeProp_5 = AddGameObject<GameProp>("TimeProp_5");
    timeProp_5->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time,PxVec3(0.04,0.04,0.04), PxTransform(PxVec3(7.7f, -0.1f, 61.0f),TimePropQuat), 30.0f);
    //On a platform with many boxes
    auto timeProp_6 = AddGameObject<GameProp>("TimeProp_6");
    timeProp_6->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time, PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(17.7f, -1.23f, 80.65f),TimePropQuat), 30.0f);
    //Hide in boxes
    auto timeProp_7 = AddGameObject<GameProp>("TimeProp_7");
    timeProp_7->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time,PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(25.35f, -2.069f, 81.65f),TimePropQuat), 30.0f);
    //beside a row of stone balls
    auto timeProp_8 = AddGameObject<GameProp>("TimeProp_8");
    timeProp_8->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time, PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(29.03f, 0.266f, 126.43f),TimePropQuat), 30.0f);
    //Above the fence platform
    auto timeProp_9 = AddGameObject<GameProp>("TimeProp_9");
    timeProp_9->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time, PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(45.69f, 0.06f, 120.45f),TimePropQuat), 30.0f);
    //Big Uphill before end
    auto timeProp_10 = AddGameObject<GameProp>("TimeProp_10");
    timeProp_10->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time, PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(74.74f, 2.098f, 142.9f),TimePropQuat), 30.0f);
    auto timeProp_11 = AddGameObject<GameProp>("TimeProp_11");
    timeProp_11->GetComponent<GamePropScript>()->SetGameProp(Utils::GamePropType::Time, PxVec3(0.04,0.04,0.04),PxTransform(PxVec3(82.57f, 6.294f, 136.31f),TimePropQuat), 30.0f);
}

void TestScene::PlayBgm(float DeltaTime)
{
    if (!bPlaying) return;

    // 由于音频系统未能实现循环播放功能，只能在这里定时播放bgm
    mBgmElapsedTime += DeltaTime;
    if (mBgmElapsedTime > mBgmDuration[mBgmIndex]) {
        mBgmElapsedTime = 0;
        mBgmIndex = (mBgmIndex + 1) % mBgmNum;
        AudioSystem::Play(AudioName(AudioName::game_bgm1 + mBgmIndex), false);
    }
}

void TestScene::StopBgm()
{
    AudioSystem::Stop(AudioName(AudioName::game_bgm1));
    AudioSystem::Stop(AudioName(AudioName::game_bgm1 + 1));
    AudioSystem::Stop(AudioName(AudioName::game_bgm1 + 2));
}
