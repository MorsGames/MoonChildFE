#include "SDL2Input.h"

#include <cstdio>

static constexpr int AXIS_THRESHOLD = 16000;

SDL2Input::SDL2Input() = default;

SDL2Input::~SDL2Input()
{
    Destroy();
}

bool SDL2Input::Init()
{
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("SDL GameController subsystem initialization failed! %s\n", SDL_GetError());
        return false;
    }

#ifdef MOONCHILD_GAMECONTROLLERDB_PATH
    if (SDL_GameControllerAddMappingsFromFile(MOONCHILD_GAMECONTROLLERDB_PATH) < 0)
    {
        printf("SDL game controller mapping load failed! %s\n", SDL_GetError());
    }
#endif

    const int count = SDL_NumJoysticks();
    for (int i = 0; i < count; i++)
    {
        if (SDL_IsGameController(i))
        {
            Gamepad = SDL_GameControllerOpen(i);
            if (Gamepad != nullptr)
            {
                SDL_Joystick* joystick = SDL_GameControllerGetJoystick(Gamepad);
                GamepadId = SDL_JoystickInstanceID(joystick);
            }
            break;
        }
    }
    return true;
}

void SDL2Input::Destroy()
{
    ClearAllSources();
    Queue.push_back(InputEvent::FocusLost());
    MouseDeltaRemainderX = 0.0f;
    MouseDeltaRemainderY = 0.0f;
    if (Gamepad != nullptr)
    {
        SDL_GameControllerClose(Gamepad);
        Gamepad = nullptr;
        GamepadId = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

int SDL2Input::TranslateKey(int sdlKey)
{
    if (sdlKey >= SDLK_a && sdlKey <= SDLK_z)
    {
        return 'A' + (sdlKey - SDLK_a);
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

int SDL2Input::TranslateGamepadButton(int button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:  return CB_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return CB_RIGHT;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:    return CB_UP;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:  return CB_DOWN;
        case SDL_CONTROLLER_BUTTON_A:          return CB_JUMP;
        case SDL_CONTROLLER_BUTTON_X:          return CB_ACTION;
        case SDL_CONTROLLER_BUTTON_B:          return CB_BACK;
        case SDL_CONTROLLER_BUTTON_START:      return CB_START;
        case SDL_CONTROLLER_BUTTON_BACK:       return CB_BACK;
        default:                            return INPUT_CODE_NONE;
    }
}

void SDL2Input::TranslateGamepadAxis(int axis, int& outNegativeCode, int& outPositiveCode)
{
    outNegativeCode = INPUT_CODE_NONE;
    outPositiveCode = INPUT_CODE_NONE;

    switch (axis)
    {
        case SDL_CONTROLLER_AXIS_LEFTX:
        {
            outNegativeCode = CB_LEFT;
            outPositiveCode = CB_RIGHT;
            break;
        }
        case SDL_CONTROLLER_AXIS_LEFTY:
        {
            outNegativeCode = CB_UP;
            outPositiveCode = CB_DOWN;
            break;
        }
        default:
        {
            break;
        }
    }
}

void SDL2Input::SetSource(uint32_t sourceId, int code, bool isDown)
{
    if (isDown && code == INPUT_CODE_NONE)
    {
        return;
    }

    Queue.push_back({sourceId, code, isDown});
}

void SDL2Input::ClearAllSources()
{
    Queue.push_back({0, 0, false});
}

void SDL2Input::OnKeyEvent(int nativeKeyCode, bool isDown, bool isRepeat)
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

void SDL2Input::OnMouseMovement(float x, float y, float xrel, float yrel)
{
    const float deltaX = xrel + MouseDeltaRemainderX;
    const float deltaY = yrel + MouseDeltaRemainderY;
    const int wholeDeltaX = static_cast<int>(deltaX);
    const int wholeDeltaY = static_cast<int>(deltaY);

    MouseDeltaRemainderX = deltaX - static_cast<float>(wholeDeltaX);
    MouseDeltaRemainderY = deltaY - static_cast<float>(wholeDeltaY);

    Queue.push_back(InputEvent::MouseMove(static_cast<int>(x), static_cast<int>(y), wholeDeltaX, wholeDeltaY));
}

void SDL2Input::OnMouseButton(int button, bool isDown, float x, float y)
{
    int mappedButton = 0;
    switch (button)
    {
        case SDL_BUTTON_LEFT:
        {
            mappedButton = INPUT_MOUSE_BUTTON_LEFT;
            break;
        }

        case SDL_BUTTON_RIGHT:
        {
            mappedButton = INPUT_MOUSE_BUTTON_RIGHT;
            break;
        }

        default:
        {
            break;
        }
    }

    if (mappedButton == 0)
    {
        return;
    }

    Queue.push_back(InputEvent::MouseButton(mappedButton, isDown, static_cast<int>(x), static_cast<int>(y)));
}

void SDL2Input::OnGamepadConnected(int instanceId)
{
    if (Gamepad == nullptr)
    {
        if (!SDL_IsGameController(instanceId))
        {
            return;
        }

        Gamepad = SDL_GameControllerOpen(instanceId);
        if (Gamepad != nullptr)
        {
            SDL_Joystick* joystick = SDL_GameControllerGetJoystick(Gamepad);
            GamepadId = SDL_JoystickInstanceID(joystick);
        }
    }
}

void SDL2Input::OnGamepadDisconnected(int instanceId)
{
    if (Gamepad != nullptr && GamepadId == static_cast<SDL_JoystickID>(instanceId))
    {
        ClearAllSources();
        SDL_GameControllerClose(Gamepad);
        Gamepad = nullptr;
        GamepadId = 0;
    }
}

void SDL2Input::OnGamepadButton(int instanceId, int button, bool isDown)
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

void SDL2Input::OnGamepadAxis(int instanceId, int axis, int value)
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

void SDL2Input::OnFocusLost()
{
    ClearAllSources();
    Queue.push_back(InputEvent::FocusLost());
    MouseDeltaRemainderX = 0.0f;
    MouseDeltaRemainderY = 0.0f;
}

bool SDL2Input::PollNext(InputEvent& out)
{
    if (Queue.empty())
    {
        return false;
    }
    out = Queue.front();
    Queue.pop_front();
    return true;
}
