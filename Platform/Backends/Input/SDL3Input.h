#pragma once

#include "IInput.h"

#include <SDL3/SDL.h>

#include <cstdint>
#include <deque>
#include <unordered_map>

class SDL3Input final : public IInput
{
public:
    SDL3Input();
    ~SDL3Input() override;

    bool Init() override;
    void Destroy() override;

    void OnKeyEvent(int nativeKeyCode, bool isDown, bool isRepeat) override;
    void OnGamepadConnected(int instanceId) override;
    void OnGamepadDisconnected(int instanceId) override;
    void OnGamepadButton(int instanceId, int button, bool isDown) override;
    void OnGamepadAxis(int instanceId, int axis, int value) override;
    void OnFocusLost() override;

    bool PollNext(InputEvent& out) override;

private:
    enum SourceKind : uint32_t
    {
        SOURCE_KEY               = 0x01000000u,
        SOURCE_GAMEPAD_BUTTON    = 0x02000000u,
        SOURCE_GAMEPAD_AXIS_NEG  = 0x03000000u,
        SOURCE_GAMEPAD_AXIS_POS  = 0x04000000u
    };

    static int TranslateKey(int sdlKey);
    static int TranslateGamepadButton(int button);
    static void TranslateGamepadAxis(int axis, int& outNegativeKey, int& outPositiveKey);

    void SetSource(uint32_t sourceId, int gameKeyCode, bool isDown);
    void ClearAllSources();

    SDL_Gamepad* Gamepad = nullptr;
    SDL_JoystickID GamepadId = 0;

    int KeyRefCount[256] = {};
    std::unordered_map<uint32_t, int> ActiveSources;
    std::deque<InputEvent> Queue;
};
