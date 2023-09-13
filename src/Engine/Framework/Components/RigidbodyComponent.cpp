#include "RigidbodyComponent.h"

#include "../Scene.h"
#include "TransformComponent.h"

#include "../../core/ModelLoader.h"
#include "../../../Game/Entities/PlayerBall.h"
void RigidbodyComponent::Start()
{
    transformComponent = GetComponent<TransformComponent>();
    isStatic = true;
    gPhys = GetScene()->physScene->gPhysics;
    gMat = GetScene()->physScene->gMaterial;
}

PxRigidActor* RigidbodyComponent::GetRigidActor()
{
    return rigidActor;
}

void RigidbodyComponent::PreUpdate(float DeltaTime)
{
    Component::PreUpdate(DeltaTime);
#if _DEBUG
    CheckActorFlagByDistance(DeltaTime);
#endif
    
    if (!isStatic && rigidActor)
    {
        //从PxRigidDynamic同步Transform信息
        transformComponent->SetWorldTransform(rigidActor->getGlobalPose());
    }
}
void RigidbodyComponent::CheckActorFlagByDistance(float DeltaTime)
{
    if (!rigidActor || isStatic) return;

    checkElapsedTime += DeltaTime;
    PxVec3 curpos = GetComponent<TransformComponent>()->GetWorldTransform().p;
    if(gameObject->parent!=nullptr)
    {
        curpos = gameObject->parent->GetComponent<TransformComponent>()->GetWorldTransform().p;
       
    }
    if (checkElapsedTime >= checkActorFlagInterval) {
        checkElapsedTime = 0;

        PxVec3 playerPos = GetScene()->GetGameObject<PlayerBall>("PlayerBall")->GetComponent<TransformComponent>()->GetWorldTransform().p;
     
        if (pow(playerPos.x - curpos.x, 2) + pow(playerPos.z - curpos.z, 2) > disableSimulationDistance_2) {
            if(!(rigidActor->getActorFlags()&PxActorFlag::eDISABLE_SIMULATION)) rigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
        }
        else {
            if((rigidActor->getActorFlags()&PxActorFlag::eDISABLE_SIMULATION)) rigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
        }
    }
}
void RigidbodyComponent::PostUpdate(float DeltaTime)
{
    Component::PostUpdate(DeltaTime);
    
   // CheckActorFlagByDistance(DeltaTime);

    if (!isStatic && rigidActor)
    {
        //将其他Component中更改的Transform应用于PxRigidActor
        rigidActor->setGlobalPose(transformComponent->GetWorldTransform());
        //同步scale
        if (transformComponent->scaleChange)
        {
            switch (geometryType)
            {
            case PxGeometryType::eSPHERE:
            {
                auto tmp = transformComponent->GetScale();
                if (transformComponent->scaleChangeX)
                {
                    transformComponent->SetScale(PxVec3(tmp.x, tmp.x, tmp.x));
                }
                else if (transformComponent->scaleChangeY)
                {
                    transformComponent->SetScale(PxVec3(tmp.y, tmp.y, tmp.y));
                }
                else
                {
                    transformComponent->SetScale(PxVec3(tmp.z, tmp.z, tmp.z));
                }
                tmp = transformComponent->GetScale();
                rigidActor->detachShape(*shape);
                shape->release();
                shape = gPhys->createShape(PxSphereGeometry(tmp.x), *material);
                rigidActor->attachShape(*shape);
                break;
            }
            case PxGeometryType::eBOX:
            {
                auto tmp = transformComponent->GetScale();
                rigidActor->detachShape(*shape);
                shape->release();
                shape = gPhys->createShape(PxBoxGeometry(tmp), *material);
                rigidActor->attachShape(*shape);
                break;
            }
            default:break;
            }
        }
    }
}

void RigidbodyComponent::SetDynamicDefault(PxGeometry &_geometry,const PxTransform &_worldTransform)
{
    SetDynamic(_geometry, _worldTransform,
        0.2f, 0.6f, 0.2f,
        0.9f, 0.3f, 0.9f,
        PxVec3(0, 12, 0), PxVec3(0, 0, 0));
}
void RigidbodyComponent::SetDynamic(PxGeometry &_geometry,const PxTransform &_worldTransform,
    const PxReal &_staticFriction,const PxReal &_dynamicFriction,const PxReal &_restitution,
    const PxReal &_mass,const PxReal &_linearDamping,const PxReal &_angularDamping,const PxVec3 &_linearVelocity,PxVec3 _angularVelocity)
{
    if (rigidActor) RemoveFromScene();
    if (material)
    {
        material->release();
        material = nullptr;
    }
    if (shape)
    {
        shape->release();
        shape = nullptr;
    }
    if (rigidActor)
    {
        rigidActor->release();
        rigidActor = nullptr;
    }
    //标记
    isStatic = false;
    geometryType = _geometry.getType();
    switch (geometryType)
    {
    case PxGeometryType::eSPHERE:
    {
        auto radius = static_cast<PxSphereGeometry*>(&_geometry)->radius;
        transformComponent->SetScale(PxVec3(radius, radius, radius));
        break;
    }
    case PxGeometryType::eBOX:
    {
        auto halfExtents = static_cast<PxBoxGeometry*>(&_geometry)->halfExtents;
        transformComponent->SetScale(halfExtents);
        break;
    }
    default:break;
    }
    //设置静摩擦力
    staticFriction = _staticFriction;
    //设置动摩擦力
    dynamicFriction = _dynamicFriction;
    //设置恢复系数
    restitution = _restitution;
    //创建PxMaterial
    material = gPhys->createMaterial(staticFriction, dynamicFriction, restitution);
    //用PxMaterial与PxGrometry创建PxShape
    shape = gPhys->createShape(_geometry, *material);
    //使用Transform创建rigidDynamic
    transformComponent->SetWorldTransform(_worldTransform);
    rigidActor = gPhys->createRigidDynamic(_worldTransform);
    //绑定GameObject
    rigidActor->userData = gameObject;
    //attach shape
    rigidActor->attachShape(*shape);
    //设置质量
    SetMass(_mass);
    //设置线性阻尼
    SetLinearDamping(_linearDamping);
    //设置角阻尼
    SetAngularDamping(_angularDamping);
    //设置线性速度
    SetLinearVelocity(_linearVelocity);
    //设置角速度
    SetAngularVelocity(_angularVelocity);

    AddToScene();
}
void RigidbodyComponent::SetSolverIterationCounts(PxU32 posIters, PxU32 velIters)
{
    assert(!isStatic && rigidActor);
    
    static_cast<PxRigidDynamic*>(rigidActor)->setSolverIterationCounts(posIters, velIters);
}
void RigidbodyComponent::SetRigidbodyFlag(PxRigidBodyFlag::Enum flag, bool value)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->setRigidBodyFlag(flag, value);
}

void RigidbodyComponent::AddToScene()
{
    assert(rigidActor);

    GetScene()->physScene->gScene->addActor(*rigidActor);
}
void RigidbodyComponent::RemoveFromScene()
{
    assert(rigidActor);

    GetScene()->physScene->gScene->removeActor(*rigidActor);
}

void RigidbodyComponent::AddForce(PxVec3 force)
{
    assert(!isStatic && rigidActor);
    
    static_cast<PxRigidDynamic*>(rigidActor)->addForce(force);
}

void RigidbodyComponent::AddTorque(PxVec3 torque)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->addTorque(torque);
}

void RigidbodyComponent::SetStaticFriction(PxReal friction)
{
    assert(rigidActor);

    staticFriction = friction;

    // 获取PxRigidDynamic上的形状数量
    PxU32 numShapes = rigidActor->getNbShapes();

    // 创建一个数组来存储获取到的形状指针
    PxShape** shapes = new PxShape * [numShapes];

    // 获取PxRigidDynamic上的所有形状
    rigidActor->getShapes(shapes, numShapes);

    for (PxU32 j = 0; j < numShapes; j++)
    {
        PxShape* shape = shapes[j];

        // 获取形状关联的材质
        PxMaterial* material = shape->getMaterialFromInternalFaceIndex(0); // 假设形状只关联一个材质

        // 更改材质的静摩擦力数值
        material->setStaticFriction(friction);
    }
}
PxReal RigidbodyComponent::GetStaticFriction()
{
    assert(rigidActor);

    return staticFriction;
}

void RigidbodyComponent::SetDynamicFriction(PxReal friction)
{
    assert(rigidActor);

    dynamicFriction = friction;

    // 获取PxRigidDynamic上的形状数量
    PxU32 numShapes = rigidActor->getNbShapes();

    // 创建一个数组来存储获取到的形状指针
    PxShape** shapes = new PxShape * [numShapes];

    // 获取PxRigidDynamic上的所有形状
    rigidActor->getShapes(shapes, numShapes);

    for (PxU32 j = 0; j < numShapes; j++)
    {
        PxShape* shape = shapes[j];

        // 获取形状关联的材质
        PxMaterial* material = shape->getMaterialFromInternalFaceIndex(0); // 假设形状只关联一个材质

        // 更改材质的动摩擦力数值
        material->setDynamicFriction(friction);
    }
}
PxReal RigidbodyComponent::GetDynamicFriction()
{
    assert(rigidActor);

    return dynamicFriction;
}

void RigidbodyComponent::SetRestitution(PxReal rest)
{
    assert(rigidActor);

    restitution = rest;

    // 获取PxRigidDynamic上的形状数量
    PxU32 numShapes = rigidActor->getNbShapes();

    // 创建一个数组来存储获取到的形状指针
    PxShape** shapes = new PxShape * [numShapes];

    // 获取PxRigidDynamic上的所有形状
    rigidActor->getShapes(shapes, numShapes);

    for (PxU32 j = 0; j < numShapes; j++)
    {
        PxShape* shape = shapes[j];

        // 获取形状关联的材质
        PxMaterial* material = shape->getMaterialFromInternalFaceIndex(0); // 假设形状只关联一个材质

        // 更改材质的恢复系数
        material->setRestitution(rest);
    }
}
PxReal RigidbodyComponent::GetRestitution()
{
    assert(rigidActor);

    return restitution;
}

void RigidbodyComponent::SetMass(PxReal mass)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->setMass(mass);
}
PxReal RigidbodyComponent::GetMass()
{
    assert(!isStatic && rigidActor);

    return static_cast<PxRigidDynamic*>(rigidActor)->getMass();
}

void RigidbodyComponent::SetLinearDamping(PxReal linearDamping)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->setLinearDamping(linearDamping);
}
PxReal RigidbodyComponent::GetLinearDamping()
{
    assert(!isStatic && rigidActor);

    return static_cast<PxRigidDynamic*>(rigidActor)->getLinearDamping();
}

void RigidbodyComponent::SetAngularDamping(PxReal angularDamping)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->setAngularDamping(angularDamping);
}
PxReal RigidbodyComponent::GetAngularDamping()
{
    assert(!isStatic && rigidActor);

    return static_cast<PxRigidDynamic*>(rigidActor)->getAngularDamping();
}

void RigidbodyComponent::SetLinearVelocity(PxVec3 velocity)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->setLinearVelocity(velocity);
}
PxVec3 RigidbodyComponent::GetLinearVelocity()
{
    assert(!isStatic && rigidActor);

    return static_cast<PxRigidDynamic*>(rigidActor)->getLinearVelocity();
}

void RigidbodyComponent::SetAngularVelocity(PxVec3 velocity)
{
    assert(!isStatic && rigidActor);

    static_cast<PxRigidDynamic*>(rigidActor)->setAngularVelocity(velocity);
}
PxVec3 RigidbodyComponent::GetAngularVelocity()
{
    assert(!isStatic && rigidActor);    

    return static_cast<PxRigidDynamic*>(rigidActor)->getAngularVelocity();
}

PxRigidActor* RigidbodyComponent::SetTriangleActor(const PxTransform& _worldTransform, PxU32 nbVerts, const void* verts, PxU32 triCount, const void* indices32, bool IsStatic)
{
    if (rigidActor) RemoveFromScene();
    if (material)
    {
        material->release();
        material = nullptr;
    }
    if (shape)
    {
        shape->release();
        shape = nullptr;
    }
    if (rigidActor)
    {
        rigidActor->release();
        rigidActor = nullptr;
    }
    isStatic = IsStatic;
    transformComponent->SetWorldTransform(_worldTransform);
    rigidActor = GetScene()->physScene->CreateTriangleActor(_worldTransform, nbVerts, verts, triCount, indices32, IsStatic);
    rigidActor->userData = gameObject;
    AddToScene();
    return rigidActor;
}

void RigidbodyComponent::SetTrigger(PxGeometry& _geometry, const PxTransform& _worldTransform)
{
    if (rigidActor) RemoveFromScene();
    if (material)
    {
        material->release();
        material = nullptr;
    }
    if (shape)
    {
        shape->release();
        shape = nullptr;
    }
    if (rigidActor)
    {
        rigidActor->release();
        rigidActor = nullptr;
    }
    isStatic = false;
    //get geometry type
    geometryType = _geometry.getType();
    //set transformCompoent's scale
    switch (geometryType)
    {
    case PxGeometryType::eSPHERE:
    {
        auto radius = static_cast<PxSphereGeometry*>(&_geometry)->radius;
        //transformComponent->SetScale(PxVec3(radius, radius, radius));
        break;
    }
    case PxGeometryType::eBOX:
    {
        auto halfExtents = static_cast<PxBoxGeometry*>(&_geometry)->halfExtents;
        //transformComponent->SetScale(halfExtents);
        break;
    }
    default:break;
    }
    //set transformCompoent's translation & rotation
    transformComponent->SetWorldTransform(_worldTransform);
    //rigid create
    rigidActor = gPhys->createRigidDynamic(_worldTransform);
    //gameobject attach
    rigidActor->userData = gameObject;
    //disable gravity
    rigidActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
    //geometry attach
    shape = PxRigidActorExt::createExclusiveShape(*rigidActor, _geometry, *gMat);
    //形状不参与物理仿真
    shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    //trigger
    shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

    AddToScene();
}

void RigidbodyComponent::ShowDetailUI()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.9f, 0.2f, 1.0f));
    ImGui::SeparatorText("Rigidbody Component");    
    ImGui::PopStyleColor(); // 恢复颜色

    if (!rigidActor) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::Text("No rigidActor!");
        ImGui::PopStyleColor(); // 恢复颜色
    }
    else {
        // LinearDamping
        ImGui::Separator();
        ImGui::Columns(2);
        auto linearDamping = GetLinearDamping();
        ImGui::Text("LinearDamping:"); ImGui::NextColumn();
        if (ImGui::InputFloat("##RigidbodyComponent LinearDamping", &linearDamping, 0.1f)) {
            SetLinearDamping(linearDamping);
        }
        ImGui::Columns(1);

        // AngularDamping
        ImGui::Separator();
        ImGui::Columns(2);
        auto angularDamping = GetAngularDamping();
        ImGui::Text("AngularDamping:"); ImGui::NextColumn();
        if (ImGui::InputFloat("##RigidbodyComponent AngularDamping", &angularDamping, 0.1f)) {
            SetAngularDamping(angularDamping);
        }
        ImGui::Columns(1);

        // LinearVelocity & AngularVelocity
        auto linearVelocity = GetLinearVelocity();
        auto angularVelocity = GetAngularVelocity();
        bool linearVelocityChange = false, angularVelocityChange = false;
        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("LinearVelocity:"); ImGui::NextColumn();
        linearVelocityChange |= ImGui::InputFloat("##RigidbodyComponent LinearVelocityX", &linearVelocity.x, 0.0f); ImGui::NextColumn();
        linearVelocityChange |= ImGui::InputFloat("##RigidbodyComponent LinearVelocityY", &linearVelocity.y, 0.0f); ImGui::NextColumn();
        linearVelocityChange |= ImGui::InputFloat("##RigidbodyComponent LinearVelocityZ", &linearVelocity.z, 0.0f);
        ImGui::Columns(1);

        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("AngularVelocity:"); ImGui::NextColumn();
        angularVelocityChange |= ImGui::InputFloat("##RigidbodyComponent AngularVelocityX", &angularVelocity.x, 0.0f); ImGui::NextColumn();
        angularVelocityChange |= ImGui::InputFloat("##RigidbodyComponent AngularVelocityY", &angularVelocity.y, 0.0f); ImGui::NextColumn();
        angularVelocityChange |= ImGui::InputFloat("##RigidbodyComponent AngularVelocityZ", &angularVelocity.z, 0.0f);
        ImGui::Columns(1);

        if (linearVelocityChange) {
            SetLinearVelocity(linearVelocity);
        }
        if (angularVelocityChange) {
            SetAngularVelocity(angularVelocity);
        }


        // Static Friction & Dynamic Friction
        auto staticFriction = GetStaticFriction();
        auto dynamicFriction = GetDynamicFriction();
        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("Static Friction"); ImGui::NextColumn();
        if (ImGui::InputFloat("##staticFriction", &staticFriction, 0.0f)) {
            SetStaticFriction(staticFriction);
        }
        ImGui::NextColumn();
        ImGui::Text("Dynamic Friction"); ImGui::NextColumn();
        if (ImGui::InputFloat("##dynamicFriction", &dynamicFriction, 0.0f)) {
            SetDynamicFriction(dynamicFriction);
        }
        ImGui::NextColumn();
        ImGui::Columns(1);

        auto restitution = GetRestitution();
        auto mass = GetMass();
        ImGui::Separator();
        ImGui::Columns(4);
        ImGui::Text("Restitution"); ImGui::NextColumn();
        if (ImGui::InputFloat("##restitution", &restitution, 0.0f)) {
            SetRestitution(restitution);
        }
        ImGui::NextColumn();
        ImGui::Text("Mass"); ImGui::NextColumn();
        if (ImGui::InputFloat("##mass", &mass, 0.0f)) {
            SetMass(mass);
        }
        ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
    }

    
    
    // 重置 rigidDynamic

    if (ImGui::TreeNode("Reset rigidDynamic(Click the <reset> button to take effect)")) {
        // 形状
        const char* shapeTypes[] = {
            "PxBoxGeometry",
            "PxSphereGeometry"
        };
        // 参数
        static int selectShape = 0;
        static float staticFriction = 0.2f, dynamicFriction = 0.2f, restitution = 0.3f;
        static float boxX = 0.5f, boxY = 0.5f, boxZ = 0.5f;
        static float sphereRadius = 0.5f;

        ImGui::Text("Select Shape:");
        ImGui::SameLine();
        if (ImGui::Combo("##Shape Type", &selectShape, shapeTypes, IM_ARRAYSIZE(shapeTypes)))
        {
        }
        // 根据形状的不同，设置不同的参数
        switch (selectShape) {
        case 0: {
            // PxBoxGeometry
            ImGui::Separator();
            ImGui::Columns(3);
            ImGui::InputFloat("Length##Box X", &boxX, 0.0f); ImGui::NextColumn();
            ImGui::InputFloat("Width##Box Y", &boxY, 0.0f); ImGui::NextColumn();
            ImGui::InputFloat("Height##Box Z", &boxZ, 0.0f);
            ImGui::Columns(1);
            break;
        }
        case 1: {
            // PxSphereGeometry
            ImGui::Separator();
            ImGui::Columns(1);
            ImGui::InputFloat("Radius##Sphere Radius", &sphereRadius, 0.0f);
            ImGui::Columns(1);
            break;
        }
        default:break;
        }



        ImGui::Columns(3);
        ImGui::NextColumn();
        if (ImGui::Button("Reset rigidDynamic")) {
            // 默认初始参数
            PxTransform defaultTransform = PxTransform(PxVec3(1, 3, 3));
            PxReal defaultStaticFriction = 0.5f,
                defaultDynamicFriction = 0.5f,
                defaultRestitution = 0.5f,
                defaultMass = 1.0f,
                defaultLinearDamping = 0.5f,
                defaultAngularDamping = 0.5f;
            PxVec3 defaultLinearVelocity = { 0,0,0 },
                defaultAngularVelocity = { 0,0,0 };
            // 如果已有刚体，使用原来的参数
            if (rigidActor) {
                RemoveFromScene();

                defaultTransform = rigidActor->getGlobalPose();
                defaultStaticFriction = GetStaticFriction(),
                    defaultDynamicFriction = GetDynamicFriction(),
                    defaultRestitution = GetRestitution(),
                    defaultMass = GetMass(),
                    defaultLinearDamping = GetLinearDamping(),
                    defaultAngularDamping = GetAngularDamping();
                defaultLinearVelocity = GetLinearVelocity(),
                    defaultAngularVelocity = GetAngularVelocity();
            }
            //Create RigidDynamic
            switch (selectShape)
            {
            case 0: {
                // PxBoxGeometry
                SetDynamic(PxBoxGeometry(boxX, boxY, boxZ), defaultTransform, defaultStaticFriction, defaultDynamicFriction, defaultRestitution, defaultMass, defaultLinearDamping, defaultAngularDamping, defaultLinearVelocity, defaultAngularVelocity);
                break;
            }
            case 1: {
                // PxSphereGeometry
                SetDynamic(PxSphereGeometry(sphereRadius), defaultTransform, defaultStaticFriction, defaultDynamicFriction, defaultRestitution, defaultMass, defaultLinearDamping, defaultAngularDamping, defaultLinearVelocity, defaultAngularVelocity);
                break;
            }
            default:break;
            }
            AddToScene();
        }
        ImGui::NextColumn();
        ImGui::Columns(1);

        ImGui::TreePop();
    }
    
}