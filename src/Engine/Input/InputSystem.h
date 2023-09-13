#pragma once
//bug if not include d3d9.h throw error "winnt.h(173, 1): [C1189] #error:  "No Target Architecture""
#include <d3d9.h>
#include <functional>
#include <unordered_map>
#include <windef.h>
#include <vector>

struct KeyEvent
{

};
enum WinMouseEventType
{
    RMouseDown = 0,
    RMouseUp,
    LMouseDown,
    LMouseUp,
    MouseMove,
    WinResize,
    MouseTypeCount,

};
extern  std::vector<std::vector<std::function<void(long, long)>>> WinMouseEvent;
extern  std::unordered_map<char, std::vector<std::function<void()>>> KeyDownEvent;
extern  std::unordered_map<char, std::vector<std::function<void()>>> KeyUpEvent;
typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> HandleFunction;
LRESULT  WinMsghandle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
class InputSystem
{
    friend class GameEngine;
    friend class Renderer;
public:
    InputSystem();
    /**
     * \brief 查询某个键是否按下
     * \param c 按键值
     * \return bool 按键是否按下
     */
    static  bool GetKey(char c);
    static HWND& GetHwnd();
    /**
     * \brief 绑定到鼠标事件
     * \tparam T 自动推导
     * \param EventType 事件类型
     * \param action 绑定的动作
     * \param ClassP 类指针
     * \code
     * example:
     *  Ball::DoWhenMouseMove(long x, long y)
     *  InputSystem::BindMouseEvent(WinMouseEventType::MouseMove,&Ball::DoWhenMouseMove,this);
     *  Ball中的成员方法DoWhenMouseMove会绑定到鼠标移动事件中
     * \endcode
     */
    template<class T>
    inline static  void BindMouseEvent(WinMouseEventType EventType, void(T::* action)(long, long), T* ClassP);

    /**
     * \brief 绑定到键盘按下事件
     * \tparam T 自动推导
     * \param c 按键值似乎只支持a到z
     * \param action 绑定的动作
     * \param ClassP 类指针
     * \code
     * example:
     *  Ball::DoWhenKey_V_Down()
     *   InputSystem::BindKeyUpEvent('v',&Ball::DoWhenKey_V_Down,this);
     *  Ball中的成员方法DoWhenMouseMove会绑定到V键按下的事件
     * \endcode
     */
    template<class T>
    inline static  void BindKeyDownEvent(char c, void(T::* action)(), T* ClassP);
    static  void BindKeyDownEvent(char c, void(*action)());
    /**
      * \brief 绑定到键盘抬起的事件
      * \tparam T 自动推导
      * \param c 按键值似乎只支持a到z
      * \param action 绑定的动作
      * \param ClassP 类指针
      * \code
      * example:
      *  Ball::DoWhenKey_V_Up()
      *   InputSystem::BindKeyUpEvent('v',&Ball::DoWhenKey_V_Up,this);
      * Ball中的成员方法DoWhenMouseMove会绑定到V键抬起的事件
      * \endcode
      */
    template<class T>
    inline static  void BindKeyUpEvent(char c, void(T::* action)(), T* ClassP);
    static   void ShowWinCursor(bool show);

    //************内部方法*******************************************
    static  bool IsForceToCenter;
    void RegisterMessageHandle(HandleFunction);
    LRESULT CALLBACK ReceiveMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool Initialize();
protected:
    void SetHwnd(HWND _hwnd);
    static   HWND hwnd;
    void Tick(float DeltaTime);
    std::vector<HandleFunction> Handles;
    inline static  void BindKeyDownEvent(char c, std::function<void()>&& func);
    //************内部方法*******************************************
};



template<class T>
inline void InputSystem::BindMouseEvent(WinMouseEventType EventType, void(T::* action)(long, long), T* ClassP)
{
    std::function<void(long, long)> func = std::bind(action, ClassP, std::placeholders::_1,
        std::placeholders::_2);
    WinMouseEvent[EventType].emplace_back(func);
}

template <class T>
void InputSystem::BindKeyDownEvent(char _c, void(T::* action)(), T* ClassP)
{
    const char c = toupper(_c);
    std::function<void()> func = std::bind(action, ClassP);
    BindKeyDownEvent(c, std::move(func));
}
template <class T>
void InputSystem::BindKeyUpEvent(char _c, void(T::* action)(), T* ClassP)
{
    const char c = toupper(_c);
    std::function<void()> func = std::bind(action, ClassP);
    if (KeyUpEvent.find(c) == KeyUpEvent.end())
    {
        KeyUpEvent[c] = { func };
    }
    else
    {
        KeyUpEvent[c].push_back(func);
    }

}
