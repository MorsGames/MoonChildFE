#pragma once

struct InputEvent
{
    int GameKeyCode = 0;
    bool IsDown = false;

    InputEvent() = default;
    InputEvent(int gameKeyCode, bool isDown) : GameKeyCode(gameKeyCode), IsDown(isDown) {}
};

class IInput
{
public:
    virtual ~IInput() = default;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual void OnKeyEvent(int nativeKeyCode, bool isDown, bool isRepeat) = 0;

    virtual void OnGamepadConnected(int instanceId) = 0;
    virtual void OnGamepadDisconnected(int instanceId) = 0;
    virtual void OnGamepadButton(int instanceId, int button, bool isDown) = 0;
    virtual void OnGamepadAxis(int instanceId, int axis, int value) = 0;
    
    virtual void OnFocusLost() = 0;

    virtual bool PollNext(InputEvent& out) = 0;
};
