#include "SDL3Input.h"

#include <cstdio>

static constexpr int AXIS_THRESHOLD = 16000;

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
    switch (button)
    {
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:  return CB_LEFT;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return CB_RIGHT;
        case SDL_GAMEPAD_BUTTON_DPAD_UP:    return CB_UP;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:  return CB_DOWN;
        case SDL_GAMEPAD_BUTTON_SOUTH:      return CB_JUMP;
        case SDL_GAMEPAD_BUTTON_WEST:       return CB_ACTION;
        case SDL_GAMEPAD_BUTTON_EAST:       return CB_BACK;
        case SDL_GAMEPAD_BUTTON_START:      return CB_START;
        case SDL_GAMEPAD_BUTTON_BACK:       return CB_BACK;
        default:                            return INPUT_CODE_NONE;
    }
}

void SDL3Input::TranslateGamepadAxis(int axis, int& outNegativeCode, int& outPositiveCode)
{
    outNegativeCode = INPUT_CODE_NONE;
    outPositiveCode = INPUT_CODE_NONE;

    switch (axis)
    {
        case SDL_GAMEPAD_AXIS_LEFTX:
            outNegativeCode = CB_LEFT;
            outPositiveCode = CB_RIGHT;
            break;
        case SDL_GAMEPAD_AXIS_LEFTY:
            outNegativeCode = CB_UP;
            outPositiveCode = CB_DOWN;
            break;
        default:
            break;
    }
}

void SDL3Input::SetSource(uint32_t sourceId, int code, bool isDown)
{
    if (isDown && code == INPUT_CODE_NONE)
    {
        return;
    }

    Queue.push_back({sourceId, code, isDown});
}

void SDL3Input::ClearAllSources()
{
    Queue.push_back({0, 0, false});
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
    const uint32_t sourceId = INPUT_SOURCE_KEY | static_cast<uint32_t>(nativeKeyCode & INPUT_SOURCE_CODE_MASK);
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
    if (Gamepad == nullptr || GamepadId != static_cast<SDL_JoystickID>(instanceId))
    {
        return;
    }
    const int code = TranslateGamepadButton(button);
    if (code == 0)
    {
        return;
    }
    const uint32_t sourceId = INPUT_SOURCE_GAMEPAD_BUTTON | static_cast<uint32_t>(button & INPUT_SOURCE_CODE_MASK);
    SetSource(sourceId, code, isDown);
}

void SDL3Input::OnGamepadAxis(int instanceId, int axis, int value)
{
    if (Gamepad == nullptr || GamepadId != static_cast<SDL_JoystickID>(instanceId))
    {
        return;
    }
    int negativeCode = INPUT_CODE_NONE;
    int positiveCode = INPUT_CODE_NONE;
    TranslateGamepadAxis(axis, negativeCode, positiveCode);
    if (negativeCode == INPUT_CODE_NONE && positiveCode == INPUT_CODE_NONE)
    {
        return;
    }

    const uint32_t negSourceId = INPUT_SOURCE_GAMEPAD_AXIS_NEG | static_cast<uint32_t>(axis & INPUT_SOURCE_CODE_MASK);
    const uint32_t posSourceId = INPUT_SOURCE_GAMEPAD_AXIS_POS | static_cast<uint32_t>(axis & INPUT_SOURCE_CODE_MASK);

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
