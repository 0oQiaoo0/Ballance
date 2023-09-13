#include "MeshRendererComponent.h"

#include "../../core/macro.h"
#include "../Scene.h"
#include "TransformComponent.h"
#include "../../Input/InputSystem.h"

void MeshRendererComponent::Start()
{
    transformComponent = GetComponent<TransformComponent>();
}

void MeshRendererComponent::RenderDefaultShape(DefaultShape Shape, XMFLOAT3 TexScale)
{
    mShape = Shape;
    mShapeStr = DefaultShapeStr[mShape];
    renderItem = GetScene()->AddDefaultRenderItem(Shape, TexScale, transformComponent->GetWorldMatrix());
    //return renderItem;
}

void MeshRendererComponent::RenderModel(std::string modelPath)
{
    mModelPath = modelPath;
    //XMMATRIX t = transformComponent->GetMatrix() * XMMatrixRotationX(-XM_PI / 2);//TODO
    renderModelItem = GetScene()->renderScene->AddRenderItem(modelPath, transformComponent->GetMatrix());
}

void MeshRendererComponent::ShowDetailUI()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.9f, 0.2f, 1.0f));
    ImGui::SeparatorText("MeshRenderer Component");
    ImGui::PopStyleColor();

    const char* shapeTypes[] = {"sphere", "Box", "Cylinder"};
    static int selectedShape = 1;
    
    ImGui::Text("DefaultShape: ");
    ImGui::SameLine();
    ImGui::Text(mShapeStr.c_str());
    if(ImGui::Combo("shape##shape", &selectedShape, shapeTypes, IM_ARRAYSIZE(shapeTypes)))
    {
        switch (selectedShape)
        {
        case 0:
            if(renderItem.IsValid())
            {
                renderItem.Shape = DefaultShape::Sphere;
                renderItem.Update();
            }
            break;
        case 1:
            if(renderItem.IsValid())
            {
                renderItem.Shape = DefaultShape::Box;
                renderItem.Update();
            }
            break;
        default:break;
        }
    }


    ImGui::Text("Model Path: ");
    ImGui::SameLine();
    ImGui::Text(mModelPath.c_str());

}

void MeshRendererComponent::PretendRemove()
{
    if(renderItem.IsValid())
    {
        XMStoreFloat4x4(&renderItem.WorldTransform, XMMatrixScaling(0, 0, 0));
        renderItem.Update();
    }
    if(renderModelItem!=nullptr)
    {
        renderModelItem->transform =XMMatrixScaling(0, 0, 0);
        renderModelItem->Update();
    }   

}

void MeshRendererComponent::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
    if (renderItem.IsValid())
    {
        XMMATRIX& xmMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f) * transformComponent->GetWorldMatrix();///tmp fix
        XMFLOAT4X4 float4x4;
        XMStoreFloat4x4(&float4x4, xmMatrix);
        renderItem.WorldTransform = float4x4;
        // LOG_INFO(std::to_string(renderItem->World.m[1][1]));
        renderItem.Update();
    }
    if(renderModelItem!=nullptr)
    {
       renderModelItem->transform =transformComponent->GetWorldMatrix();
        renderModelItem->Update();
    }
}

