#pragma once
#include "cloth/PxCloth.h"
#include "cloth/PxClothCollisionData.h"
#include "cloth/PxClothFabric.h"
#include "cloth/PxClothTypes.h"

#include <PxShape.h>
#include <PsArray.h>
#include "extensions/PxClothMeshDesc.h"
#include "extensions/PxClothFabricCooker.h"
#include "cooking/PxCooking.h"
#include "PxRigidBody.h"
#include <vector>
#include "../../src/Engine/Framework/GameObject.h"
class SampleCharacterClothFlag : public GameObject
{
public:
    SampleCharacterClothFlag();
    void Start() override;
	void setWind(const PxVec3 &dir, PxReal strength, const PxVec3& range);
    void Update(float dtime) override;
	void release();
	PxCloth* getCloth() { return mCloth; }
    PxClothMeshDesc createMeshGrid(PxVec3 dirU, PxVec3 dirV, PxU32 numU, PxU32 numV, std::vector<PxVec4>& vertices, std::vector<PxU32>& indices, std::vector<PxVec2>& texcoords);
protected:
    PxCloth*                mCloth;
    std::vector<PxVec2>     mUVs;
    PxVec3                  mWindDir;
    PxVec3                  mWindRange;
	PxReal                  mWindStrength;
    PxReal                  mTime;
	PxRigidDynamic*			mCapsuleActor;
private:
	SampleCharacterClothFlag& operator=(const SampleCharacterClothFlag&);
};

