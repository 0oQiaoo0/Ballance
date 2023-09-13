#include "ParticleScene.h"
#define SAMPLE_NEW(x)	new(#x, __FILE__, __LINE__) x
#include <iostream>
#include <PxPhysics.h>
#include <PxSceneLock.h>
#include "ParticleEmitterPressure.h"
#include "ParticleEmitterRate.h"



void ParticleScene::createWaterfallEmitter(const PxTransform& transform) 
{
	// setup emitter
	ParticleEmitterPressure* pressureEmitter = new ParticleEmitterPressure(
		ParticleEmitterPressure::Shape::eRECTANGLE, mFluidEmitterWidth, mFluidEmitterWidth, 0.81f);	// shape and size of emitter and spacing between particles being emitted

	pressureEmitter->setMaxRate(PxF32(mFluidParticleCount));
	pressureEmitter->setVelocity(mFluidEmitterVelocity);
	pressureEmitter->setRandomAngle(0.0f);
	pressureEmitter->setRandomPos(PxVec3(0.0f,0.0f,0.0f));
	pressureEmitter->setLocalPose(transform);
	Emitter emitter;
	emitter.emitter = pressureEmitter;
	emitter.isEnabled = true;
	emitter.particleSystem = mWaterfallParticleSystem;
	mEmitters.push_back(emitter);
}
void ParticleScene::createParticleContent()
{
	PxSceneWriteLock scopedLock(*mScene);

	switch (mParticleLoadIndex)
	{
	case 0:
		{
			// moderate
			mFluidParticleCount = 1000;
			mFluidEmitterWidth = 1.5f;
			mFluidEmitterVelocity = 9.0f;
			mSmokeParticleCount = 200;
			mSmokeLifetime = 2.5f;
			mSmokeEmitterRate = 20.0f;
			mDebrisParticleCount = 200;	
		}
		break;
	case 1:
		{
			// medium
			mFluidParticleCount = 8000;
			mFluidEmitterWidth = 2.0f;
			mFluidEmitterVelocity = 10.0f;
			mSmokeParticleCount = 1200;
			mSmokeLifetime = 3.5f;
			mSmokeEmitterRate = 70.0f;
			mDebrisParticleCount = 800;	
		}
		break;
	case 2:
		{
			// high
			mFluidParticleCount = 20000;
			mFluidEmitterWidth = 3.5f;
			mFluidEmitterVelocity = 50.0f;
			mSmokeParticleCount = 2000;
			mSmokeLifetime = 4.0f;
			mSmokeEmitterRate = 70.0f;
			mDebrisParticleCount = 1600;
		}
		break;
	}

	// create particle systems
	createParticleSystems();
	createHotpotEmitter(PxVec3(-0.455374f, 5.830761f, 0.995798f));

	PxTransform transform(PxVec3(-71.8f, 43.0f, -62.0f), PxQuat(-PxHalfPi, PxVec3(1, 0, 0)));	
	//createWaterfallEmitter(transform);
	return;
	// create waterfall emitters
	if (mParticleLoadIndex == 0)
	{
		PxTransform transform(PxVec3(-71.8f, 43.0f, -62.0f), PxQuat(-PxHalfPi, PxVec3(1, 0, 0)));	
		createWaterfallEmitter(transform);
	}
	else if (mParticleLoadIndex == 1)
	{
		PxTransform transform0(PxVec3(-39.9, 36.3f, -80.2f), PxQuat(-PxHalfPi, PxVec3(1, 0, 0)));	
		createWaterfallEmitter(transform0);

		PxTransform transform1(PxVec3(-71.8f, 43.0f, -62.0f), PxQuat(-PxHalfPi, PxVec3(1, 0, 0)));	
		createWaterfallEmitter(transform1);
	}
	else
	{
		PxTransform transform(PxVec3(-120.0f, 75.0f, -120.0f), PxQuat(PxHalfPi*0.5f, PxVec3(0, 1, 0)));	
		createWaterfallEmitter(transform);
	}

	// create several hotpots emitters
	
	//createHotpotEmitter(PxVec3(10.154306f, 16.634144f, -97.960060f));
	//createHotpotEmitter(PxVec3(14.048334f, 8.3961439f, -32.113029f));
}

void ParticleScene::onSubstepSetup(float dtime, PxBaseTask* cont)
{
	PxSceneWriteLock scopedLock(*mScene);

	for (PxU32 i = 0; i < mEmitters.size(); ++i)
		mEmitters[i].update(dtime);
	
	
	if (mWaterfallParticleSystem)
		mWaterfallParticleSystem->update(dtime);
	
	if (mSmokeParticleSystem)
		mSmokeParticleSystem->update(dtime);
	
	if (mDebrisParticleSystem)
		mDebrisParticleSystem->update(dtime);

	const std::vector<PxVec3> res =  mSmokeParticleSystem->getPositions();
	mSmokeParticleSystem->getValidity();
	int valid = mSmokeParticleSystem->getValidParticleRange();
	if(valid>0)
	{
		//testPos = res[valid-1];
	}


}

void Emitter::update(float dtime)
{
	if(!isEnabled || !particleSystem)
		return; 

	ParticleSystem& ps = *particleSystem;

	PxVec3 acceleration = ps.getPxParticleBase()->getExternalAcceleration() + ps.getPxParticleBase()->getScene()->getGravity();
	float maxVeloctiy = ps.getPxParticleBase()->getMaxMotionDistance() / (dtime);	

	ParticleData initData(ps.getPxParticleBase()->getMaxParticles());
	emitter->step(initData, dtime, acceleration, maxVeloctiy);

	ps.createParticles(initData);

	//std::cout<<particleSystem->getNumParticles()<<std::endl;
}

void ParticleScene::createHotpotEmitter(const PxVec3& position) 
{
	// setup smoke emitter
	{
		/*ParticleEmitterPressure* rateEmitter = new ParticleEmitterPressure(
			ParticleEmitter::Shape::eRECTANGLE, .4f, .4f, 1.1f);				// shape and size of emitter and spacing between particles being emitted

		rateEmitter->setMaxRate(mSmokeEmitterRate) ;//setRate();
		rateEmitter->setVelocity(3.0f);
		rateEmitter->setRandomAngle(0.4f);
		rateEmitter->setLocalPose(PxTransform(PxVec3(position.x, position.y, position.z), PxQuat(-PxHalfPi, PxVec3(1, 0, 0))));
		rateEmitter->setRandomPos({0.21f,0.2f,0.2f});
		Emitter emitter;
		emitter.emitter = rateEmitter;
		emitter.isEnabled = true;
		emitter.particleSystem = mSmokeParticleSystem;
		mEmitters.push_back(emitter);*/
		ParticleEmitterPressure* pressureEmitter = new ParticleEmitterPressure(
	ParticleEmitterPressure::Shape::eRECTANGLE, mFluidEmitterWidth, mFluidEmitterWidth, 0.81f);	// shape and size of emitter and spacing between particles being emitted

		pressureEmitter->setMaxRate(PxF32(mFluidParticleCount));
		pressureEmitter->setVelocity(mFluidEmitterVelocity);
		pressureEmitter->setRandomAngle(0.0f);
		pressureEmitter->setRandomPos(PxVec3(0.0f,0.0f,0.0f));
		pressureEmitter->setLocalPose(PxTransform(PxVec3(position.x, position.y, position.z), PxQuat(-PxHalfPi, PxVec3(1, 0, 0))));
		Emitter emitter;
		emitter.emitter = pressureEmitter;
		emitter.isEnabled = true;
		emitter.particleSystem = mSmokeParticleSystem;
		//mEmitters.push_back(emitter);
	}
	
	// setup debris emitter
	#if !defined(RENDERER_ENABLE_GLES2)
	{
		ParticleEmitterRate* rateEmitter = new ParticleEmitterRate(
			ParticleEmitter::Shape::eRECTANGLE, 1.0f, 1.0f, 0.1f);				// shape and size of emitter and spacing between particles being emitted

		rateEmitter->setRate(10.0f);
		rateEmitter->setVelocity(12.0f);
		rateEmitter->setRandomAngle(PxHalfPi / 8);
		rateEmitter->setLocalPose(PxTransform(PxVec3(position.x, position.y, position.z), PxQuat(-PxHalfPi, PxVec3(1, 0, 0))));
	
		Emitter emitter;
		emitter.emitter = rateEmitter;
		emitter.isEnabled = true;
		emitter.particleSystem = mDebrisParticleSystem;
		//mEmitters.push_back(emitter);		
	}
#endif
}


PxParticleFluid* ParticleScene::createFluid(PxU32 maxParticles, PxReal restitution, PxReal viscosity,
									PxReal stiffness, PxReal dynamicFriction, PxReal particleDistance) 
{	
	PxParticleFluid* fluid = mPhysics->createParticleFluid(maxParticles);
	//runtimeAssert(fluid, "PxPhysics::createParticleFluid returned NULL\n");
	fluid->setGridSize(5.0f);
	fluid->setMaxMotionDistance(0.3f);
	fluid->setRestOffset(particleDistance*0.3f);
	fluid->setContactOffset(particleDistance*0.3f*2);
	fluid->setDamping(0.0f);
	fluid->setRestitution(restitution);
	fluid->setDynamicFriction(dynamicFriction);
	fluid->setRestParticleDistance(particleDistance);
	fluid->setViscosity(viscosity);
	fluid->setStiffness(stiffness);
	fluid->setParticleReadDataFlag(PxParticleReadDataFlag::eVELOCITY_BUFFER, true);
#if PX_SUPPORT_GPU_PHYSX
	fluid->setParticleBaseFlag(PxParticleBaseFlag::eGPU, mRunOnGpu);
#endif
	mScene->addActor(*fluid);


#if PX_SUPPORT_GPU_PHYSX
	//check gpu flags after adding to scene, cpu fallback might have been used.
	mRunOnGpu = mRunOnGpu && (fluid->getParticleBaseFlags() & PxParticleBaseFlag::eGPU);
#endif

	return fluid;
}

void ParticleScene::createParticleSystems()
{
	// create waterfall fluid
	{
		PxParticleFluid* px_fluid = createFluid(mFluidParticleCount, 0.3f, 60.0f, 45.0f, 0.001f, 0.8f);
		px_fluid->setSimulationFilterData(collisionGroupWaterfall);
		// This will be deleted by RenderParticleSystemActor
		ParticleSystem* fluid = new  ParticleSystem(px_fluid);
		mWaterfallParticleSystem =fluid;
		//mWaterfallParticleSystem->setRenderMaterial(getMaterial(MATERIAL_WATERFALL));
		//addRenderParticleSystemActor(*mWaterfallParticleSystem);
	}

	// create fire particle
	{
		PxParticleBase* px_ps = createFireParticleSystem(mSmokeParticleCount);
		px_ps->setSimulationFilterData(PxFilterData());
		// This will be deleted by RenderParticleSystemActor
		ParticleSystem* ps = new  ParticleSystem(px_ps, false);
		mFireParticleSystem = ps;
		//mWaterfallParticleSystem->setRenderMaterial(getMaterial(MATERIAL_WATERFALL));
		//addRenderParticleSystemActor(*mWaterfallParticleSystem);
	}
	
	// create smoke particle system
	{
		PxParticleBase* px_ps = createParticleSystem(mSmokeParticleCount);
		px_ps->setDamping(0.5f);
		px_ps->setRestitution(0.0f);
		px_ps->setDynamicFriction(0.0f);
		px_ps->setExternalAcceleration(PxVec3(0.0f, 17.0f, 0.0f));
		px_ps->setSimulationFilterData(PxFilterData());
		// This will be deleted by RenderParticleSystemActor
		ParticleSystem* ps = new  ParticleSystem(px_ps, false);
		ps->setUseLifetime(true);
		ps->setLifetime(mSmokeLifetime);
		mSmokeParticleSystem = ps;
		//mSmokeParticleSystem->setRenderMaterial(getMaterial(MATERIAL_SMOKE));
		//addRenderParticleSystemActor(*mSmokeParticleSystem);
	}

	// create debris particle system
#if !defined(RENDERER_ENABLE_GLES2)
	{
		PxParticleBase* px_ps = createParticleSystem(mDebrisParticleCount);
		px_ps->setStaticFriction(10.0f);
		px_ps->setSimulationFilterData(collisionGroupDebris);
		// This will be deleted by RenderParticleSystemActor
		ParticleSystem* ps = new ParticleSystem(px_ps, true);
		ps->setUseLifetime(true);
		ps->setLifetime(5.0f);
		mDebrisParticleSystem = ps;
		//mDebrisParticleSystem->setRenderMaterial(getMaterial(MATERIAL_DEBRIS));
		//addRenderParticleSystemActor(*mDebrisParticleSystem);
	}
#endif
}

PxParticleSystem* ParticleScene::createFireParticleSystem(PxU32 maxParticles) 
{	
	PxParticleSystem* ps = mPhysics->createParticleSystem(maxParticles);
	//runtimeAssert(ps, "PxPhysics::createParticleSystem returned NULL\n");
	ps->setParticleMass(10);
	ps->setGridSize(6.0f);
	ps->setMaxMotionDistance(0.43f);
	ps->setRestOffset(0.0143f);
	ps->setContactOffset(0.0143f*2);
	ps->setDamping(0.0f);
	ps->setRestitution(0.2f);
	ps->setDynamicFriction(0.05f);
	ps->setParticleReadDataFlag(PxParticleReadDataFlag::eVELOCITY_BUFFER, true);
#if PX_SUPPORT_GPU_PHYSX
	ps->setParticleBaseFlag(PxParticleBaseFlag::eGPU, mRunOnGpu);
#endif
	mScene->addActor(*ps);
	//runtimeAssert(ps->getScene(), "PxScene::addActor failed\n");

	#if PX_SUPPORT_GPU_PHYSX
	//check gpu flags after adding to scene, cpu fallback might have been used.
	mRunOnGpu = mRunOnGpu && (ps->getParticleBaseFlags() & PxParticleBaseFlag::eGPU);
#endif
	return ps;
}

PxParticleSystem* ParticleScene::createParticleSystem(PxU32 maxParticles) 
{	
	PxParticleSystem* ps = mPhysics->createParticleSystem(maxParticles);
	//runtimeAssert(ps, "PxPhysics::createParticleSystem returned NULL\n");
	ps->setParticleMass(10);
	ps->setGridSize(6.0f);
	ps->setMaxMotionDistance(0.43f);
	ps->setRestOffset(0.0143f);
	ps->setContactOffset(0.0143f*2);
	ps->setDamping(0.0f);
	ps->setRestitution(0.2f);
	ps->setDynamicFriction(0.05f);
	ps->setParticleReadDataFlag(PxParticleReadDataFlag::eVELOCITY_BUFFER, true);
#if PX_SUPPORT_GPU_PHYSX
	ps->setParticleBaseFlag(PxParticleBaseFlag::eGPU, mRunOnGpu);
#endif
	mScene->addActor(*ps);
	//runtimeAssert(ps->getScene(), "PxScene::addActor failed\n");

#if PX_SUPPORT_GPU_PHYSX
	//check gpu flags after adding to scene, cpu fallback might have been used.
	mRunOnGpu = mRunOnGpu && (ps->getParticleBaseFlags() & PxParticleBaseFlag::eGPU);
#endif
	return ps;
}
