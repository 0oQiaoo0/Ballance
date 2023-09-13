#include "CameraComponent.h"

#include "../Scene.h"
#include "TransformComponent.h"

#include "../../Render/dx12/Common/Camera.h"

void CameraComponent::ShowDetailUI()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.9f, 0.2f, 1.0f));
    ImGui::SeparatorText("Camera Component");
    ImGui::PopStyleColor(); // »Ö¸´ÑÕÉ«


    // Position
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("Position:"); ImGui::NextColumn();
    ImGui::InputFloat("x##CameraComponent PosX", &camera->mPosition.x, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("y##CameraComponent PosY", &camera->mPosition.y, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("z##CameraComponent PosZ", &camera->mPosition.z, 0.0f);
    ImGui::Columns(1);

    // Look
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("Look:"); ImGui::NextColumn();
    ImGui::InputFloat("x##CameraComponent LookX", &camera->mLook.x, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("y##CameraComponent LookY", &camera->mLook.y, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("z##CameraComponent LookZ", &camera->mLook.z, 0.0f);
    ImGui::Columns(1);

    // Up
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("Up:"); ImGui::NextColumn();
    ImGui::InputFloat("x##CameraComponent UpX", &camera->mUp.x, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("y##CameraComponent UpY", &camera->mUp.y, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("z##CameraComponent UpZ", &camera->mUp.z, 0.0f);
    ImGui::Columns(1);

    // Right
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("Right:"); ImGui::NextColumn();
    ImGui::InputFloat("x##CameraComponent RightX", &camera->mRight.x, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("y##CameraComponent RightY", &camera->mRight.y, 0.0f); ImGui::NextColumn();
    ImGui::InputFloat("z##CameraComponent RightZ", &camera->mRight.z, 0.0f);
    ImGui::Columns(1);

    // Aspect & FovY
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("Aspect:"); ImGui::NextColumn();
    ImGui::InputFloat("##CameraComponent Aspect", &camera->mAspect, 0.0f); ImGui::NextColumn();
    ImGui::Text("FovY:"); ImGui::NextColumn();
    ImGui::InputFloat("##CameraComponent FovY", &camera->mFovY, 0.0f);
    ImGui::Columns(1);

    // NearZ & FarZ
    ImGui::Separator();
    ImGui::Columns(4);
    ImGui::Text("NearZ:"); ImGui::NextColumn();
    ImGui::InputFloat("##CameraComponent NearZ", &camera->mNearZ, 0.0f); ImGui::NextColumn();
    ImGui::Text("FarZ:"); ImGui::NextColumn();
    ImGui::InputFloat("##CameraComponent FarZ", &camera->mFarZ, 0.0f);
    ImGui::Columns(1);


    ImGui::Separator();

}

void CameraComponent::Start()
{
    Component::Start();
    camera = std::make_unique<Camera>();
    camera->SetPosition(0.0f, 0.0f, -15.0f);
    camera->SetLens(0.25f * MathHelper::Pi, 16 / 9, 1.0f, 1000.0f);

    GetScene()->SetRenderCamera(camera.get());
}
