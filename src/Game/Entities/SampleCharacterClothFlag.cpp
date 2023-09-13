#include "PxPhysXConfig.h"
#if PX_USE_CLOTH_API

#include "SampleCharacterClothFlag.h"
//#include "SampleCharacterCloth.h"
//#include "TestClothHelpers.h"
#include "cloth/PxClothParticleData.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/TransformComponent.h"

using namespace physx;

SampleCharacterClothFlag::SampleCharacterClothFlag() :
	mWindDir(1.0f, 0.0f, 0.0f),
	mWindRange(1.0f),
	mWindStrength(1.0f),
	mTime(0.0f),
	mCapsuleActor(NULL)
{
	
}
std::vector<PxVec4> vertices;
std::vector<PxU32> primitives;

void SampleCharacterClothFlag::Start()
{
	GameObject::Start();

	// 分辨率，越大越精细
	PxReal resX = 50;
	PxReal resY = 25;
	// 旗子大小
	PxReal sizeX = 2.0f, sizeY = 1.0f, height = 5.0f;
	PxTransform pose = PxTransform(PxVec3(1, 5, 3), PxQuat(PxIdentity));


	PxScene* scene = GetScene()->physScene->gScene;
	auto physics = GetScene()->physScene->gPhysics;

	PxVec3 pos = pose.p;

	// settings to adjust flag and pole position on the ground
	PxTransform clothPose(pos, pose.q);

	PxReal halfSizeX = sizeX * 0.5f;
	PxReal halfSizeY = sizeY * 0.5f;
	PxReal offset = sizeX * 0.1f;
	PxReal halfHeight = height * 0.5f;
	PxReal clothHeight = height - halfSizeY;
	clothPose.p.y += clothHeight;

	// create the cloth flag mesh
	//std::vector<PxVec4> vertices;
	//std::vector<PxU32> primitives;
	PxClothMeshDesc meshDesc = createMeshGrid(
		PxVec3(sizeX, 0, 0), PxVec3(0, -sizeY, 0), resX, resY, vertices, primitives, mUVs);

	// attach two corners on the left
	for (PxU32 i = 0; i < meshDesc.points.count; i++)
	{
		PxReal u = mUVs[i].x, v = mUVs[i].y;
		bool constrain = ((u < 0.01) && (v < 0.01)) || ((u < 0.01) && (v > 0.99));
		vertices[i].w = constrain ? 0.0f : 1.0f;
	}

	// create cloth fabric
	PxClothFabric* clothFabric = PxClothFabricCreate(*physics, meshDesc, PxVec3(0, -1, 0));
	PX_ASSERT(meshDesc.points.stride == sizeof(PxVec4));

	// create the cloth actor
	const PxClothParticle* particles = (const PxClothParticle*)meshDesc.points.data;
	PxCloth* cloth = physics->createCloth(clothPose, *clothFabric, particles, PxClothFlags());
	PX_ASSERT(cloth);

	// add this cloth into the scene
	scene->addActor(*cloth);

	// create collision capsule for the pole
	std::vector<PxClothCollisionSphere> spheres(2);
	spheres[0].pos = PxVec3(-halfSizeX - offset, -clothHeight, 0);
	spheres[1].pos = PxVec3(-halfSizeX - offset, halfSizeY, 0);
	spheres[0].radius = 0.1f;
	spheres[1].radius = 0.1f;

	std::vector<PxU32> indexPairs;
	indexPairs.push_back(0);
	indexPairs.push_back(1);
	cloth->setCollisionSpheres(spheres.begin()._Ptr, 2);
	cloth->addCollisionCapsule(indexPairs[0], indexPairs[1]);


	// set solver settings
	cloth->setSolverFrequency(120);
	cloth->setDampingCoefficient(PxVec3(0.0f));

	cloth->setStretchConfig(PxClothFabricPhaseType::eBENDING, PxClothStretchConfig(0.1f));
	cloth->setTetherConfig(PxClothTetherConfig(1.0f, 1.0f));

	mCloth = cloth;

	// we create an invisible kinematic capsule so that the CCT avoids colliding character against the pole of this flag
	// note that cloth collision capsule above has no effect on rigid body scenes.
	PxTransform capsulePose(pos, PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	capsulePose.p.x -= halfSizeX + offset;
	capsulePose.p.y += halfHeight;
	PxCapsuleGeometry geom(0.2f, halfHeight);


	mCapsuleActor = physics->createRigidDynamic(capsulePose);
	mCapsuleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.1f);
	PxRigidActorExt::createExclusiveShape(*mCapsuleActor, geom, *material);
	scene->addActor(*mCapsuleActor);
}


////////////////////////////////////////////////////////////////////////////////
void SampleCharacterClothFlag::release()
{

	mCloth->release();
	mCapsuleActor->release();

	delete this;
}

////////////////////////////////////////////////////////////////////////////////
void SampleCharacterClothFlag::setWind(const PxVec3 &dir, PxReal strength, const PxVec3& range)
{
    mWindStrength = strength;
    mWindDir = dir;
    mWindRange = range;
}


float RandFloat(float a, float b)
{  
	return ((a)+(double)(rand()) / RAND_MAX * ((b)-(a)));
}

////////////////////////////////////////////////////////////////////////////////
void SampleCharacterClothFlag::Update(float dtime)
{
	GameObject::Update(dtime);

    PX_ASSERT(mCloth);
	

	// implementation of a simple wind force that varies its direction, strength over time
	mTime += dtime * RandFloat(0.0f, 1.0f);

	float st = 1.0f + (float)sin(mTime);
	float windStrength = RandFloat(1.0f, st) * mWindStrength;
	float windRangeStrength = RandFloat(0.0f, 2.0f);

	PxVec3 offset(PxReal(RandFloat(-1, 1)), PxReal(RandFloat(-1, 1)), PxReal(RandFloat(-1, 1)));
	float ct = 1.0f + (float)cos(mTime + 0.1);
	offset *= ct;
	PxVec3 windAcceleration = windStrength * mWindDir + windRangeStrength * mWindRange.multiply(offset);

	mCloth->setExternalAcceleration(windAcceleration);
	//mCloth->setExternalAcceleration(windStrength * mWindDir * 10);

	//std::cout << vertices[9].x << ", " << vertices[9].y << ", " << vertices[9].z << std::endl;
}

PxClothMeshDesc SampleCharacterClothFlag::createMeshGrid(PxVec3 dirU, PxVec3 dirV, PxU32 numU, PxU32 numV,
	std::vector<PxVec4>& vertices, std::vector<PxU32>& indices, std::vector<PxVec2>& texcoords)
{
	int numVertices = numU * numV;				// 顶点数
	int numQuads = (numU - 1) * (numV - 1);		// 方格数

	vertices.resize(numVertices);
	indices.resize(numQuads * 4);				// 1个方格4个顶点
	texcoords.resize(numVertices);

	// fill in point data
	PxReal scaleU = 1 / PxReal(numU - 1);
	PxReal scaleV = 1 / PxReal(numV - 1);

	auto posIt = vertices.begin();
	auto texIt = texcoords.begin();
	for (PxU32 i = 0; i < numV; i++)
	{
		PxReal texV = i * scaleV;
		PxVec3 posV = (texV - 0.5f) * dirV;
		for (PxU32 j = 0; j < numU; j++)
		{
			PxReal texU = j * scaleU;
			PxVec3 posU = (texU - 0.5f) * dirU;
			*(posIt++) = PxVec4(posU + posV, 1.0f);
			*texIt++ = PxVec2(texU, 1.0f - texV);
		}
	}

	// fill in quad index data
	PxU32* idxIt = indices.begin()._Ptr;
	for (PxU32 i = 0; i < numV - 1; i++)
	{
		for (PxU32 j = 0; j < numU - 1; j++)
		{
			PxU32 i0 = i * numU + j;
			*idxIt++ = i0;
			*idxIt++ = i0 + 1;
			*idxIt++ = i0 + numU + 1;
			*idxIt++ = i0 + numU;
		}
	}

	PxClothMeshDesc meshDesc;

	// convert vertex array to PxBoundedData (desc.points)
	meshDesc.points.data = vertices.begin()._Ptr;
	meshDesc.points.count = static_cast<PxU32>(numVertices);
	meshDesc.points.stride = sizeof(PxVec4);

	meshDesc.invMasses.data = &vertices.begin()->w;
	meshDesc.invMasses.count = static_cast<PxU32>(numVertices);
	meshDesc.invMasses.stride = sizeof(PxVec4);

	// convert index array to PxBoundedData (desc.quads)
	meshDesc.quads.data = indices.begin()._Ptr;
	meshDesc.quads.count = static_cast<PxU32>(numQuads);
	meshDesc.quads.stride = sizeof(PxU32) * 4; // <- stride per quad

	return meshDesc;
}


#endif // PX_USE_CLOTH_API

 
