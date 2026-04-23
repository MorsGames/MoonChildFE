#pragma once

#include <cstdint>

static constexpr uint32_t INPUT_SOURCE_CODE_MASK = 0x00FFFFFFu;
static constexpr int INPUT_CODE_NONE = 0;  // sorry nothing

static constexpr int VK_BACK   = 0x08;
static constexpr int VK_TAB    = 0x09;
static constexpr int VK_RETURN = 0x0D;
static constexpr int VK_SHIFT  = 0x10;
static constexpr int VK_CTRL   = 0x11;
static constexpr int VK_ALT    = 0x12;
static constexpr int VK_ESCAPE = 0x1B;
static constexpr int VK_LEFT   = 37;
static constexpr int VK_UP     = 38;
static constexpr int VK_RIGHT  = 39;
static constexpr int VK_DOWN   = 40;

static constexpr int CB_LEFT   = 240;
static constexpr int CB_RIGHT  = 241;
static constexpr int CB_UP     = 242;
static constexpr int CB_DOWN   = 243;
static constexpr int CB_JUMP   = 244;
static constexpr int CB_ACTION = 245;
static constexpr int CB_START  = 246;
static constexpr int CB_BACK   = 247;

enum InputSourceKind : uint32_t
{
    INPUT_SOURCE_NONE              = 0x00000000u,
    INPUT_SOURCE_KEY               = 0x01000000u,
    INPUT_SOURCE_GAMEPAD_BUTTON    = 0x02000000u,
    INPUT_SOURCE_GAMEPAD_AXIS_NEG  = 0x03000000u,
    INPUT_SOURCE_GAMEPAD_AXIS_POS  = 0x04000000u
};

struct InputEvent
{
    uint32_t SourceId = 0;
    int Code = 0;
    bool IsDown = false;

    InputEvent() = default;
    InputEvent(int code, bool isDown) : Code(code), IsDown(isDown) {}
    InputEvent(uint32_t sourceId, int code, bool isDown) : SourceId(sourceId), Code(code), IsDown(isDown) {}
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
