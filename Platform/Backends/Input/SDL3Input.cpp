#include "SDL3Input.h"

#include "InputBridge.h"

#include <cstdio>
#include <cstring>

static constexpr int AXIS_THRESHOLD = 16000;

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

SDL3Input::SDL3Input() = default;

SDL3Input::~SDL3Input()
{
    Destroy();
}

bool SDL3Input::Init()
{
    if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD))
    {
        printf("SDL Gamepad subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

    if (SDL_AddGamepadMappingsFromFile("gamecontrollerdb.txt") < 0)
    {
        printf("SDL gamepad mapping load failed! %s\n", SDL_GetError());
    }

    int count = 0;
    if (SDL_JoystickID* ids = SDL_GetGamepads(&count))
    {
        if (count > 0)
        {
            Gamepad = SDL_OpenGamepad(ids[0]);
            if (Gamepad != nullptr)
            {
                GamepadId = SDL_GetGamepadID(Gamepad);
            }
        }
        SDL_free(ids);
    }
    return true;
}

void SDL3Input::Destroy()
{
    ClearAllSources();
    if (Gamepad != nullptr)
    {
        SDL_CloseGamepad(Gamepad);
        Gamepad = nullptr;
        GamepadId = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
}

int SDL3Input::TranslateKey(int sdlKey)
{
    if (sdlKey >= SDLK_A && sdlKey <= SDLK_Z)
    {
        return 'A' + (sdlKey - SDLK_A);
    }
    if (sdlKey >= SDLK_0 && sdlKey <= SDLK_9)
    {
        return '0' + (sdlKey - SDLK_0);
    }

    switch (sdlKey)
    {
        case SDLK_LEFT:     return VK_LEFT;
        case SDLK_RIGHT:    return VK_RIGHT;
        case SDLK_UP:       return VK_UP;
        case SDLK_DOWN:     return VK_DOWN;
        case SDLK_SPACE:    return ' ';
        case SDLK_RETURN:
        case SDLK_KP_ENTER: return VK_RETURN;
        case SDLK_ESCAPE:   return VK_ESCAPE;
        case SDLK_BACKSPACE:return VK_BACK;
        case SDLK_TAB:      return VK_TAB;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:   return VK_SHIFT;
        case SDLK_LCTRL:
        case SDLK_RCTRL:    return VK_CTRL;
        case SDLK_LALT:
        case SDLK_RALT:     return VK_ALT;
        default:            return 0;
    }
}

int SDL3Input::TranslateGamepadButton(int button)
{
    if (InputBridge::Context == InputBridge::INPUT_CONTEXT_GAME)
    {
        switch (button)
        {
            case SDL_GAMEPAD_BUTTON_DPAD_LEFT:  return InputBridge::BindLeft;
            case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return InputBridge::BindRight;
            case SDL_GAMEPAD_BUTTON_DPAD_DOWN:  return InputBridge::BindDown;
            case SDL_GAMEPAD_BUTTON_DPAD_UP:    return InputBridge::BindUp;
            case SDL_GAMEPAD_BUTTON_SOUTH:      return InputBridge::BindJump;
            case SDL_GAMEPAD_BUTTON_WEST:       return InputBridge::BindAction;
            case SDL_GAMEPAD_BUTTON_START:      return VK_ESCAPE;
            default:                            return 0;
        }
    }

    switch (button)
    {
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:  return VK_LEFT;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return VK_RIGHT;
        case SDL_GAMEPAD_BUTTON_DPAD_UP:    return VK_UP;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:  return VK_DOWN;
        case SDL_GAMEPAD_BUTTON_SOUTH:      return ' ';
        case SDL_GAMEPAD_BUTTON_EAST:       return VK_ESCAPE;
        case SDL_GAMEPAD_BUTTON_START:      return VK_RETURN;
        case SDL_GAMEPAD_BUTTON_BACK:       return VK_ESCAPE;
        default:                            return 0;
    }
}

void SDL3Input::TranslateGamepadAxis(int axis, int& outNegativeKey, int& outPositiveKey)
{
    outNegativeKey = 0;
    outPositiveKey = 0;

    if (InputBridge::Context == InputBridge::INPUT_CONTEXT_GAME)
    {
        switch (axis)
        {
            case SDL_GAMEPAD_AXIS_LEFTX:
                outNegativeKey = InputBridge::BindLeft;
                outPositiveKey = InputBridge::BindRight;
                break;
            case SDL_GAMEPAD_AXIS_LEFTY:
                outNegativeKey = InputBridge::BindUp;
                outPositiveKey = InputBridge::BindDown;
                break;
            default:
                break;
        }
        return;
    }

    switch (axis)
    {
        case SDL_GAMEPAD_AXIS_LEFTX:
            outNegativeKey = VK_LEFT;
            outPositiveKey = VK_RIGHT;
            break;
        case SDL_GAMEPAD_AXIS_LEFTY:
            outNegativeKey = VK_UP;
            outPositiveKey = VK_DOWN;
            break;
        default:
            break;
    }
}

void SDL3Input::SetSource(uint32_t sourceId, int gameKeyCode, bool isDown)
{
    if (gameKeyCode <= 0 || gameKeyCode >= 256)
    {
        return;
    }

    auto it = ActiveSources.find(sourceId);
    const bool wasDown = it != ActiveSources.end();

    if (isDown && !wasDown)
    {
        ActiveSources.emplace(sourceId, gameKeyCode);
        if (KeyRefCount[gameKeyCode]++ == 0)
        {
            Queue.push_back({gameKeyCode, true});
        }
    }
    else if (!isDown && wasDown)
    {
        const int previousCode = it->second;
        ActiveSources.erase(it);
        if (--KeyRefCount[previousCode] == 0)
        {
            Queue.push_back({previousCode, false});
        }
    }
}

void SDL3Input::ClearAllSources()
{
    for (const auto& entry : ActiveSources)
    {
        const int code = entry.second;
        if (--KeyRefCount[code] == 0)
        {
            Queue.push_back({code, false});
        }
    }
    ActiveSources.clear();
    std::memset(KeyRefCount, 0, sizeof(KeyRefCount));
}

void SDL3Input::OnKeyEvent(int nativeKeyCode, bool isDown, bool isRepeat)
{
    if (isRepeat)
    {
        return;
    }
    const int code = TranslateKey(nativeKeyCode);
    if (code == 0)
    {
        return;
    }
    const uint32_t sourceId = SOURCE_KEY | static_cast<uint32_t>(nativeKeyCode & 0x00FFFFFF);
    SetSource(sourceId, code, isDown);
}

void SDL3Input::OnGamepadConnected(int instanceId)
{
    if (Gamepad == nullptr)
    {
        Gamepad = SDL_OpenGamepad(static_cast<SDL_JoystickID>(instanceId));
        if (Gamepad != nullptr)
        {
            GamepadId = SDL_GetGamepadID(Gamepad);
        }
    }
}

void SDL3Input::OnGamepadDisconnected(int instanceId)
{
    if (Gamepad != nullptr && GamepadId == static_cast<SDL_JoystickID>(instanceId))
    {
        ClearAllSources();
        SDL_CloseGamepad(Gamepad);
        Gamepad = nullptr;
        GamepadId = 0;
    }
}

void SDL3Input::OnGamepadButton(int instanceId, int button, bool isDown)
{
    if (InputBridge::Context == InputBridge::INPUT_CONTEXT_REMAP)
    {
        return;
    }
    if (Gamepad == nullptr || GamepadId != static_cast<SDL_JoystickID>(instanceId))
    {
        return;
    }
    const int code = TranslateGamepadButton(button);
    if (code == 0)
    {
        return;
    }
    const uint32_t sourceId = SOURCE_GAMEPAD_BUTTON | static_cast<uint32_t>(button & 0x00FFFFFF);
    SetSource(sourceId, code, isDown);
}

void SDL3Input::OnGamepadAxis(int instanceId, int axis, int value)
{
    if (InputBridge::Context == InputBridge::INPUT_CONTEXT_REMAP)
    {
        return;
    }
    if (Gamepad == nullptr || GamepadId != static_cast<SDL_JoystickID>(instanceId))
    {
        return;
    }
    int negativeCode = 0;
    int positiveCode = 0;
    TranslateGamepadAxis(axis, negativeCode, positiveCode);

    const uint32_t negSourceId = SOURCE_GAMEPAD_AXIS_NEG | static_cast<uint32_t>(axis & 0x00FFFFFF);
    const uint32_t posSourceId = SOURCE_GAMEPAD_AXIS_POS | static_cast<uint32_t>(axis & 0x00FFFFFF);

    SetSource(negSourceId, negativeCode, value <= -AXIS_THRESHOLD);
    SetSource(posSourceId, positiveCode, value >=  AXIS_THRESHOLD);
}

void SDL3Input::OnFocusLost()
{
    ClearAllSources();
}

bool SDL3Input::PollNext(InputEvent& out)
{
    if (Queue.empty())
    {
        return false;
    }
    out = Queue.front();
    Queue.pop_front();
    return true;
}
