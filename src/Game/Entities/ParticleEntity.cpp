#include "ParticleEntity.h"

#include "../Scripts/ParticleScript.h"

void ParticleEntity::Start()
{
    GameObject::Start();
    AddComponent<ParticleScript>();
}
