#pragma once
#include <PxRigidDynamic.h>
#include <cooking/PxCooking.h>
#include <foundation/Px.h>
#include <map>
#include <vector>

#include "ParticleEmitter.h"
using namespace physx;
struct Emitter
{
    Emitter() : emitter(NULL), isEnabled(false) {}
    void update(float dt);
    ParticleEmitter* emitter;
    bool isEnabled;
    ParticleSystem* particleSystem;
};
static const PxFilterData collisionGroupDrain		(0, 0, 0, 1);
static const PxFilterData collisionGroupForceSmoke	(0, 0, 0, 2);
static const PxFilterData collisionGroupForceWater	(0, 0, 0, 3);
static const PxFilterData collisionGroupWaterfall	(0, 0, 1, 0);
static const PxFilterData collisionGroupSmoke		(0, 0, 1, 1);
static const PxFilterData collisionGroupDebris		(0, 0, 1, 2);
class ParticleScene
{
public:
    ParticleScene(PxFoundation* m_foundation, PxPhysics* m_physics, PxCooking* m_cooking, PxScene* m_scene,
        PxMaterial* m_material)
        : mFoundation(m_foundation),
          mPhysics(m_physics),
          mCooking(m_cooking),
          mScene(m_scene),
          mMaterial(m_material)
    {
    }
    void createParticleContent();
    PxFoundation*							mFoundation;
    PxPhysics*								mPhysics;
    PxCooking*								mCooking;
    PxScene*								mScene;
    PxMaterial*								mMaterial;
    ParticleSystem*		                    mWaterfallParticleSystem;
    ParticleSystem*		                    mFireParticleSystem;
    ParticleSystem*		                    mSmokeParticleSystem;
    ParticleSystem*		                    mDebrisParticleSystem;

    std::vector<Emitter>			mEmitters;
    
    
    void							onSubstepSetup(float dtime, PxBaseTask* cont);		
private:

    
    friend			class			Raygun;

    PxU32							mFluidParticleCount;
    PxF32							mFluidEmitterWidth;
    PxF32							mFluidEmitterVelocity;
    PxU32							mSmokeParticleCount;
    PxF32							mSmokeLifetime;
    PxF32							mSmokeEmitterRate;
    PxU32							mDebrisParticleCount;
		
    PxU32							mParticleLoadIndex =1;
    PxU32							mParticleLoadIndexMax;
    PxF32							mLoadTextAlpha;


    


    void createWaterfallEmitter(const PxTransform& transform);

    void createHotpotEmitter(const PxVec3& position);
    PxParticleFluid* createFluid(PxU32 maxParticles, PxReal restitution, PxReal viscosity, PxReal stiffness,
                                 PxReal dynamicFriction, PxReal particleDistance);
    void createParticleSystems();
    PxParticleSystem* createFireParticleSystem(PxU32 maxParticles);

    // void releaseParticleSystems();
#pragma region samepleparticle

    PxParticleSystem* createParticleSystem(PxU32 maxParticles);
#if PX_SUPPORT_GPU_PHYSX
    bool							mRunOnGpu;
#endif
    
#pragma endregion
    
};
