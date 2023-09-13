#include "ParticleScript.h"


#include <memory>

#include "../../Engine/Input/InputSystem.h"
#include "../../Engine/Particle/ParticleEmitter.h"
#include "../../Engine/Particle/ParticleEmitterRate.h"
#include "../../Engine/Particle/ParticleEmitterPressure.h"



void ParticleScript::SetEmitterPos1(PxTransform transform)
{
    if(pressureEmitter==nullptr)return;
    BaseHeight = transform.p.y;
    pressureEmitter->setLocalPose(transform);
}

void ParticleScript::SetEmitterPos2(PxTransform transform)
{
    if(pressureEmitter2==nullptr)return;
    BaseHeight = transform.p.y;
    pressureEmitter2->setLocalPose(transform);
}
void ParticleScript::AddEmitter(Emitter emitter)
{
    GetScene()->physScene->particleScene->mEmitters.push_back(emitter);
}

void ParticleScript::Start()
{
    Component::Start();
    xm_position = std::make_shared<std::vector<XMFLOAT4X4>>();
    xm_texTransform = std::make_shared<std::vector<XMFLOAT4X4>>();;
    mWaterfallParticleSystem = GetScene()->physScene->particleScene->mWaterfallParticleSystem;
    mDebrisParticleSystem = GetScene()->physScene->particleScene->mDebrisParticleSystem;
    mFireParticleSystem = GetScene()->physScene->particleScene->mFireParticleSystem;
    mSmokeParticleSystem = GetScene()->physScene->particleScene->mSmokeParticleSystem ;
    CreateTestParticle();
    
}

void ParticleScript::CreateTestParticle()
{
    // medium
   float  mFluidParticleCount = 8000;
   float  mFluidEmitterWidth = 2.0f;
   float  mFluidEmitterVelocity = 10.0f;
   float  mSmokeParticleCount = 1200;
   float  mSmokeLifetime = 3.5f;
   float  mSmokeEmitterRate = 70.0f;
   float  mDebrisParticleCount = 800;
    
     pressureEmitter = new ParticleEmitterRate(
    ParticleEmitterRate::Shape::eRECTANGLE, 0.5, 0.5, 0.81f);	// shape and size of emitter and spacing between particles being emitted
    pressureEmitter->setRate(23);
    pressureEmitter->setVelocity(1.2f);
    pressureEmitter->setRandomAngle(2.0f);
    pressureEmitter->setRandomPos(PxVec3(0.1f,0.1f,0.1f));
    pressureEmitter->setLocalPose(PxTransform(PxVec3(3, 3, 0.5), PxQuat(-PxHalfPi, PxVec3(1, 0, 0))));

    
    pressureEmitter2 = new ParticleEmitterRate(
    ParticleEmitterRate::Shape::eRECTANGLE, 0.5, 0.5, 0.81f);	// shape and size of emitter and spacing between particles being emitted
    pressureEmitter2->setRate(23);
    pressureEmitter2->setVelocity(1.2f);
    pressureEmitter2->setRandomAngle(2.0f);
    pressureEmitter2->setRandomPos(PxVec3(0.1f,0.1f,0.1f));
    pressureEmitter2->setLocalPose(PxTransform(PxVec3(3, 3, 4.5), PxQuat(-PxHalfPi, PxVec3(1, 0, 0))));
    
    Emitter emitter;
    emitter.emitter = pressureEmitter;
    emitter.isEnabled = true;
    emitter.particleSystem = mSmokeParticleSystem;
    mSmokeParticleSystem->setLifetime(2.0f);
    XMFLOAT4X4 float4x4;
    XMStoreFloat4x4(&float4x4,XMMatrixIdentity());
    xm_position->resize(mFluidParticleCount,float4x4);
    xm_texTransform->resize(mFluidParticleCount,float4x4);
    AddEmitter(emitter);
    emitter.emitter = pressureEmitter2;
    AddEmitter(emitter);
    particle_instance =  GetScene()->renderScene->AddParticles(DefaultShape::Box,DefaultMaterial::Wood,XMMatrixIdentity(),xm_position,xm_texTransform,mFluidParticleCount);

}
XMMATRIX calculateScale(float height,float baseHeight,float maxHeigth,float factor=1)
{
    if(height<baseHeight)return XMMatrixScaling(0.333,0.333,0.333);
    if(height>maxHeigth)return XMMatrixScaling(0,0,0);
    float scale = max( powf(cosf((XM_PI/(maxHeigth-baseHeight))*(height-baseHeight)),factor),0)/3;
    return XMMatrixScaling(scale,scale,scale);
}
XMMATRIX CalculateRotation(float height,float velocity,UINT id)
{
    uint8_t seed = 0x114514;
    std::hash<UINT> uintHash;
    float roll = uintHash(id)+1;
    float pitch = uintHash(id+1)+1;
    float yaw = uintHash(id+2)+1;
    return XMMatrixRotationAxis({pitch,yaw,roll},velocity*height);
}
void ParticleScript::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
    if(InputSystem::GetKey('B'))
    {
        pressureEmitter->setRate(0);
    }
    //这里获取到的是多个发射器的所有顶点信息，要分别哪些粒子是属于哪个发射器的
    position = mSmokeParticleSystem->getPositions();
    int valid = mSmokeParticleSystem->getValidParticleRange();
    particle_instance->count = valid;
    for(int i = 0 ;i<valid;i++)
    {
        XMMATRIX scale = calculateScale(position[i].y,BaseHeight,BaseHeight+3);
        XMMATRIX rotation = CalculateRotation(position[i].y,1,i);
        //todo: check valid particle
        XMStoreFloat4x4( &(*xm_position.get())[i],scale*rotation*XMMatrixTranslation(position[i].x,position[i].y,position[i].z));
        XMStoreFloat4x4( &(*xm_texTransform.get())[i],XMMatrixIdentity());
    }
    particle_instance->Update();
}
