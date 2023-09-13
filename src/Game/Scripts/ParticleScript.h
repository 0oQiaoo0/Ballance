#pragma once
#include "../../Engine/Framework/Component.h"

class ParticleEmitterRate;

class ParticleScript:public Component
{
public:
protected:
    ParticleSystem* mWaterfallParticleSystem = nullptr;
    ParticleSystem* mFireParticleSystem = nullptr;
    ParticleSystem* mSmokeParticleSystem = nullptr;
    ParticleSystem* mDebrisParticleSystem = nullptr;
    std::vector<PxVec3> position;

    std::shared_ptr<std::vector<XMFLOAT4X4>>  xm_texTransform;
    std::shared_ptr< std::vector<XMFLOAT4X4>> xm_position;

    float BaseHeight =4;
    ParticleEmitterRate* pressureEmitter2;
    ParticleEmitterRate* pressureEmitter;
public:
    void SetEmitterPos1(PxTransform transform);
    void SetEmitterPos2(PxTransform transform);
    ParticleInstance* particle_instance = nullptr;
    void AddEmitter(Emitter emitter);
    void Start() override;
    void CreateTestParticle();
    void Update(float DeltaTime) override;
};


