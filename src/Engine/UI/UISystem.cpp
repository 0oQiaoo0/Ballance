#define STB_IMAGE_IMPLEMENTATION
#include "UISystem.h"
#include "imgui/imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <d3d12.h>
#include <tchar.h>
#include <GL/gl.h>
#include "../GameEngine.h"
#include "../core/GlobalContext.h"
#include "UIState.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "imgui/imstb_truetype.h"

bool UISystem::cursorInCenter = false;
bool UISystem::showCursor = true;
std::vector<ImageInfo> UISystem::imageInfos;

void UISystem::OnResize(long x, long y) {
    RECT rect;
    GetClientRect(hWnd, &rect);
    hWndWidth = rect.right - rect.left;
    hWndHeight = rect.bottom - rect.top;
    ui->SetHWndSize(hWndWidth, hWndHeight);
}
void UISystem::OnEscDown() {
    ui->OnEscDown();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool UISystem::Init()
{
    InputSystem::BindMouseEvent(WinMouseEventType::WinResize, &UISystem::OnResize, this);
    char c = char(27);  //Esc键
    InputSystem::BindKeyDownEvent(c, &UISystem::OnEscDown, this);
    //注册窗口事件消息
    globalContext.mGameEngine->mInputSystem->RegisterMessageHandle(ImGui_ImplWin32_WndProcHandler);
    return true;
}

Scene* UISystem::GetScene()
{
    return mScene;
}

bool UISystem::BindRenderTarget(HWND mhMainWnd, Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice,
    int SwapChainBufferCount, ImGuiIO _io)
{
    //渲染器绑定窗口回调
    hWnd = mhMainWnd;
    io = _io;
    OnResize(0, 0);
    LoadFont();
    return true;
}

void UISystem::LoadFont() {
    ImFontConfig Font_cfg;
    Font_cfg.FontDataOwnedByAtlas = false;
    Font = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 16.0f, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());
    Font_Big = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 48.0f, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());
   // ImGui::PushFont(Font_Big);
}

void UISystem::HideUICursor()
{
    UISystem::showCursor = false;
    while (ShowCursor(FALSE) >= 0);
    UISystem::cursorInCenter = true;
}

void UISystem::ShowUICursor()
{
    while (ShowCursor(TRUE) < 0);
    UISystem::showCursor = true;
    UISystem::cursorInCenter = false;
}

void UISystem::BindScene(Scene* scene)
{
    mScene = scene;
}

ImageInfo& UISystem::GetImageByName(ImageName name)
{
    return imageInfos[int(name)];
}

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
UISystem::UISystem(UIState* uiState)
{
    // set initial uistate
    ui = uiState;
    ui->setContext(this);

}
void UISystem::ChangeState(UIState* newState)
{
    delete ui;
    ui = newState;
    ui->setContext(this);
    OnResize(0, 0);
}
void UISystem::Draw()
{
    ui->CreateUI();
}

void UISystem::Tick(float DeltaTime)
{
    ui->Tick(DeltaTime);
}
