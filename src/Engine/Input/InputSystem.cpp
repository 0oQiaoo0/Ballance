#include "InputSystem.h"

#include <unordered_map>
#include <windowsx.h>

//win窗口初始化
HWND InputSystem::hwnd = nullptr;
//是否强制锁定到中间
bool InputSystem::IsForceToCenter = true;
std::vector<std::vector<std::function<void(long, long)>>> WinMouseEvent(MouseTypeCount + 1);
std::unordered_map<char, std::vector<std::function<void()>>> KeyDownEvent;
std::unordered_map<char, std::vector<std::function<void()>>> KeyUpEvent;
void SendMsg(const std::vector<std::function<void(long, long)>>& Observers, WPARAM& wParam, LPARAM& lParam)
{
    for (auto& o : Observers)
    {
        o(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    }
}
void SendMsg(const std::vector<std::function<void()>>& Observers)
{
    for (auto& o : Observers)
    {
        o();
    }
}
LRESULT  WinMsghandle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_MOUSEMOVE:
    {
        SendMsg(WinMouseEvent[MouseMove], wParam, lParam); break;
    }
    case WM_RBUTTONUP:
    {
        SendMsg(WinMouseEvent[RMouseUp], wParam, lParam); break;
    }
    case WM_KEYDOWN:
    {
        if (KeyDownEvent.count(static_cast<char>(wParam)) <= 0)break;
        SendMsg(KeyDownEvent[static_cast<char>(wParam)]); break;
    }
    case WM_KEYUP:
    {
        if (KeyUpEvent.count(static_cast<char>(wParam)) <= 0)break;
        SendMsg(KeyUpEvent[static_cast<char>(wParam)]); break;
    }
    case WM_SIZE:
    {
        SendMsg(WinMouseEvent[WinResize], wParam, lParam); break;
    }
    }
    return false;
}
InputSystem::InputSystem()
{
    RegisterMessageHandle(WinMsghandle);
}

bool InputSystem::GetKey(char c)
{
    return GetAsyncKeyState(c) & 0x8000;
}

HWND& InputSystem::GetHwnd()
{
    // TODO: 在此处插入 return 语句
    return hwnd;
}
void InputSystem::BindKeyDownEvent(char _c, std::function<void()>&& func)
{
    char c = static_cast<char>(toupper(_c));
    if (KeyDownEvent.find(c) == KeyDownEvent.end())
    {
        KeyDownEvent[c] = { func };
    }
    else
    {
        KeyDownEvent[c].push_back(func);
    }
}
void InputSystem::BindKeyDownEvent(char c, void(*action)())
{
    std::function<void()> func(action);
    BindKeyDownEvent(c, std::move(func));
}

void InputSystem::ShowWinCursor(bool show)
{
    ShowCursor(show);
}

void InputSystem::RegisterMessageHandle(HandleFunction handle)
{
    Handles.push_back(handle);
}

bool InputSystem::Initialize()
{
    return true;
}

void InputSystem::SetHwnd(HWND _hwnd)
{
    hwnd = _hwnd;
}

void InputSystem::Tick(float DeltaTime)
{
    if(GetKey('W')) 
    if (IsForceToCenter)
    {
        //SetCursorPos(0,0);
    }
}

LRESULT InputSystem::ReceiveMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int r = Handles.size();
    for (int i = 0; i < r; ++i)
    {
        if (Handles[i](hwnd, msg, wParam, lParam))
        {
            return true;
        }
    }
    return false;
}
