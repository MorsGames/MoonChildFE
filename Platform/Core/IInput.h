#pragma once

#include "keys.hpp"

#include <cstdint>

static constexpr uint32_t INPUT_SOURCE_CODE_MASK = 0x00FFFFFFu;
static constexpr int INPUT_CODE_NONE = 0;  // sorry nothing

enum InputMouseButton : int
{
    INPUT_MOUSE_BUTTON_LEFT = 1,
    INPUT_MOUSE_BUTTON_RIGHT = 2
};

enum InputSourceKind : uint32_t
{
    INPUT_SOURCE_NONE              = 0x00000000u,
    INPUT_SOURCE_KEY               = 0x01000000u,
    INPUT_SOURCE_GAMEPAD_BUTTON    = 0x02000000u,
    INPUT_SOURCE_GAMEPAD_AXIS_NEG  = 0x03000000u,
    INPUT_SOURCE_GAMEPAD_AXIS_POS  = 0x04000000u
};

enum InputEventKind : uint8_t
{
    INPUT_EVENT_SOURCE = 0,
    INPUT_EVENT_MOUSE_MOVE,
    INPUT_EVENT_MOUSE_BUTTON,
    INPUT_EVENT_FOCUS_LOST
};

struct InputEvent
{
    InputEventKind Kind = INPUT_EVENT_SOURCE;
    uint32_t SourceId = 0;
    int Code = 0;
    bool IsDown = false;
    int X = 0;
    int Y = 0;
    int DeltaX = 0;
    int DeltaY = 0;
    int Button = 0;

    InputEvent() = default;
    InputEvent(int code, bool isDown) : Code(code), IsDown(isDown) {}
    InputEvent(uint32_t sourceId, int code, bool isDown) : SourceId(sourceId), Code(code), IsDown(isDown) {}

    static InputEvent MouseMove(int x, int y, int deltaX, int deltaY)
    {
        InputEvent event;
        event.Kind = INPUT_EVENT_MOUSE_MOVE;
        event.X = x;
        event.Y = y;
        event.DeltaX = deltaX;
        event.DeltaY = deltaY;
        return event;
    }

    static InputEvent MouseButton(int button, bool isDown, int x, int y)
    {
        InputEvent event;
        event.Kind = INPUT_EVENT_MOUSE_BUTTON;
        event.Button = button;
        event.IsDown = isDown;
        event.X = x;
        event.Y = y;
        return event;
    }

    static InputEvent FocusLost()
    {
        InputEvent event;
        event.Kind = INPUT_EVENT_FOCUS_LOST;
        return event;
    }
};

class IInput
{
public:
    virtual ~IInput() = default;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual void OnKeyEvent(int nativeKeyCode, bool isDown, bool isRepeat) = 0;
    virtual void OnMouseMovement(float x, float y, float xrel, float yrel) = 0;
    virtual void OnMouseButton(int button, bool isDown, float x, float y) = 0;

    virtual void OnGamepadConnected(int instanceId) = 0;
    virtual void OnGamepadDisconnected(int instanceId) = 0;
    virtual void OnGamepadButton(int instanceId, int button, bool isDown) = 0;
    virtual void OnGamepadAxis(int instanceId, int axis, int value) = 0;
    
    virtual void OnFocusLost() = 0;

    virtual bool PollNext(InputEvent& out) = 0;
};
