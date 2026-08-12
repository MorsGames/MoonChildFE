#pragma once

#include "IInput.h"

#include <SDL.h>

#include <cstdint>
#include <deque>

class SDL2Input final : public IInput
{
public:
    SDL2Input();
    ~SDL2Input() override;

    bool Init() override;
    void Destroy() override;

    void OnKeyEvent(int nativeKeyCode, bool isDown, bool isRepeat) override;
    void OnMouseMovement(float x, float y, float xrel, float yrel) override;
    void OnMouseButton(int button, bool isDown, float x, float y) override;
    void OnGamepadConnected(int instanceId) override;
    void OnGamepadDisconnected(int instanceId) override;
    void OnGamepadButton(int instanceId, int button, bool isDown) override;
    void OnGamepadAxis(int instanceId, int axis, int value) override;
    void OnFocusLost() override;

    bool PollNext(InputEvent& out) override;

private:
    static int TranslateKey(int sdlKey);
    static int TranslateGamepadButton(int button);
    static void TranslateGamepadAxis(int axis, int& outNegativeCode, int& outPositiveCode);

    void SetSource(uint32_t sourceId, int code, bool isDown);
    void ClearAllSources();

    SDL_GameController* Gamepad = nullptr;
    SDL_JoystickID GamepadId = 0;
    float MouseDeltaRemainderX = 0.0f;
    float MouseDeltaRemainderY = 0.0f;

    std::deque<InputEvent> Queue;
};
