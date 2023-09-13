#pragma once
#include <d3d12.h>
#include "../Render/dx12/Common/Camera.h"
#include "imgui/imgui.h"
#include <functional>
#include "Font.h"
#include "../../Engine/Framework/Scene.h"
#include "../../Engine/Framework/GameObject.h"

struct ImageInfo;

class UIState;

class UISystem
{
    friend class Renderer;
    friend class GameEngine;
    friend class UIPass;
public:
    enum class ImageName {
        IMG_BTN_YES = 0,
        IMG_BTN_NO,
        IMG_MAIN_BACKROUND,
        IMG_MAIN_PLAY,
        IMG_MAIN_EXIT,
        IMG_MAIN_CHOOSE_LEVEL,
        IMG_MAIN_LEFT_ARROW,
        IMG_GAME_MENU,
        IMG_GAME_SETTING,
        IMG_GAME_INTRO,
        IMG_GAME_RESTART,
        IMG_GMAE_CONTINUE,
        IMG_GAME_EXIT,
        IMG_GAME_CLOCK,
        IMG_GAME_HEART,
        IMG_GAME_BALL,
        IMG_GAME_FAIL,
        IMG_GAME_VICTORY,
        IMG_GAME_TRY_AGAIN,
        IMG_GAME_PLAY_AGAIN,
    };

    static bool cursorInCenter;
    static bool showCursor;
    static void SetCursorInCenter(bool in) {
        cursorInCenter = in;
    }
public:
    UISystem(UIState* uiState);
    void ChangeState(UIState* newState);
    //draw per frame
    void Draw();
    void Tick(float DeltaTime);
    bool BindRenderTarget(
        HWND      mhMainWnd,
        Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice,
        int SwapChainBufferCount, ImGuiIO io);
    static ImageInfo& GetImageByName(ImageName name);
    
    static void ShowUICursor();
    static void HideUICursor();


public:
    void BindScene(Scene* scene);
    Scene* GetScene();

    ImFont* Font = nullptr;
    ImFont* Font_Big = nullptr;
protected:
    //ID3D12Device* g_pd3dDevice = nullptr;
    bool Init();
    void OnResize(long x, long y);
    void OnEscDown();
    void LoadFont();

    //imagePaths必须在init中或者之前初始化完毕,一次性把用到的所用图片上传
    std::vector<std::string> imagePaths = {
        "../../src/Asset/UI/yes.png" ,
        "../../src/Asset/UI/no.png" ,
        "../../src/Asset/UI/ballance_main.png" ,
        "../../src/Asset/UI/start_game.png",
        "../../src/Asset/UI/exit_program.png",
        "../../src/Asset/UI/choose_level.png",
        "../../src/Asset/UI/left_arrow.png",
        "../../src/Asset/UI/menu.png",
        "../../src/Asset/UI/setting.png",
        "../../src/Asset/UI/intro.png",
        "../../src/Asset/UI/restart.png",
        "../../src/Asset/UI/continue.png",
        "../../src/Asset/UI/exit.png",
        "../../src/Asset/UI/clock.png",
        "../../src/Asset/UI/heart.png",
        "../../src/Asset/UI/ball.png",
        "../../src/Asset/UI/fail.png",
        "../../src/Asset/UI/victory.png",
        "../../src/Asset/UI/try_again.png",
        "../../src/Asset/UI/play_again.png",

    };
    static std::vector<ImageInfo> imageInfos;
    ImGuiIO io;
    HWND hWnd;
    int hWndWidth = 0;
    int hWndHeight = 0;

private:
    UIState* ui = nullptr;
    Scene* mScene;
};

