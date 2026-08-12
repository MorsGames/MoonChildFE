#include "InputBridge.h"

#include <globals.hpp>

#include <cstring>
#include <deque>
#include <unordered_map>

extern int g_MouseFlg;
extern int g_MouseActualFlg;
extern int g_MouseXDown;
extern int g_MouseYDown;
extern int g_MouseDeltaX;
extern int g_MouseDeltaY;

namespace InputBridge
{
    static constexpr int REPEAT_DELAY_FRAMES = 30;
    static constexpr int REPEAT_INTERVAL_FRAMES = 2;
    static constexpr int REPEAT_INTERVAL_FRAMES_EASY = 10;

    struct ActiveSource
    {
        int Code = 0;
        int FramesHeld = 0;
        bool RepeatSuppressed = false;

        ActiveSource() = default;
        ActiveSource(int code, int framesHeld) : Code(code), FramesHeld(framesHeld) {}
    };

    static IInput* InputBackend = nullptr;

    static int KeyRefCount[256] = {};
    static std::unordered_map<uint32_t, ActiveSource> ActiveSources;
    static std::deque<InputEvent> Queue;
    static int MouseIdleFrames = 999999;

    static bool IsValidGameKey(int gameKeyCode)
    {
        return gameKeyCode > 0 && gameKeyCode < 256;
    }

    static void ResetMouseState()
    {
        g_MouseFlg = 0;
        g_MouseActualFlg = 0;
        g_MouseXDown = 0;
        g_MouseYDown = 0;
        g_MouseDeltaX = 0;
        g_MouseDeltaY = 0;
        mouselbut = 0;
        mouserbut = 0;
        mouselchng = 0;
        mouserchng = 0;
        MouseIdleFrames = 999999;
    }

    static void ClearState()
    {
        ActiveSources.clear();
        std::memset(KeyRefCount, 0, sizeof(KeyRefCount));
        Queue.clear();
    }

    static void ReleaseAllSources()
    {
        for (int key = 1; key < 256; key++)
        {
            if (KeyRefCount[key] > 0)
            {
                Queue.push_back({key, false});
            }
        }

        ActiveSources.clear();
        std::memset(KeyRefCount, 0, sizeof(KeyRefCount));
    }

    static void ApplyMouseMovement(int x, int y, int deltaX, int deltaY)
    {
        g_MouseXCurrent = x;
        g_MouseYCurrent = y;
        g_MouseFlg = 0;
        g_MouseDeltaX += deltaX;
        g_MouseDeltaY += deltaY;

        if (deltaX != 0 || deltaY != 0)
        {
            MouseIdleFrames = 0;
        }
    }

    static void ApplyMouseButton(int button, bool isDown, int x, int y)
    {
        g_MouseXCurrent = x;
        g_MouseYCurrent = y;
        MouseIdleFrames = 0;

        switch (button)
        {
            case INPUT_MOUSE_BUTTON_LEFT:
            {
                mouselbut = isDown ? 1 : 0;
                mouselchng = 1;
                g_MouseActualFlg = isDown ? 1 : 0;
                if (isDown)
                {
                    g_MouseXDown = x;
                    g_MouseYDown = y;
                }
                else
                {
                    g_MouseFlg = 1;
                }
                break;
            }

            case INPUT_MOUSE_BUTTON_RIGHT:
            {
                mouserbut = isDown ? 1 : 0;
                mouserchng = 1;
                break;
            }

            default:
            {
                break;
            }
        }
    }

    static void SubmitEvent(const InputEvent& inputEvent);
    static void QueueRepeats();

    void Attach(IInput* input)
    {
        InputBackend = input;
        ClearState();
        ResetMouseState();
    }

    void Detach()
    {
        InputBackend = nullptr;
        ClearState();
        ResetMouseState();
    }

    void Tick()
    {
        InputEvent inputEvent;
        while (InputBackend->PollNext(inputEvent))
        {
            switch (inputEvent.Kind)
            {
                case INPUT_EVENT_MOUSE_MOVE:
                    ApplyMouseMovement(inputEvent.X, inputEvent.Y, inputEvent.DeltaX, inputEvent.DeltaY);
                    break;

                case INPUT_EVENT_MOUSE_BUTTON:
                    ApplyMouseButton(inputEvent.Button, inputEvent.IsDown, inputEvent.X, inputEvent.Y);
                    break;

                case INPUT_EVENT_FOCUS_LOST:
                    ResetMouseState();
                    break;

                case INPUT_EVENT_SOURCE:
                default:
                    SubmitEvent(inputEvent);
                    break;
            }
        }

        if (MouseIdleFrames < 999999)
        {
            MouseIdleFrames++;
        }

        QueueRepeats();
    }

    static void SubmitEvent(const InputEvent& inputEvent)
    {
        if (inputEvent.SourceId == 0)
        {
            if (!inputEvent.IsDown)
            {
                ReleaseAllSources();
            }
            return;
        }

        auto it = ActiveSources.find(inputEvent.SourceId);
        const bool wasDown = it != ActiveSources.end();

        if (inputEvent.IsDown && !wasDown)
        {
            const int gameKeyCode = inputEvent.Code;
            if (!IsValidGameKey(gameKeyCode))
            {
                return;
            }

            if (!easiershootflg || speedrun_state.running)
            {
                for (auto& pair : ActiveSources)
                {
                    pair.second.RepeatSuppressed = true;
                }
            }

            ActiveSources.emplace(inputEvent.SourceId, ActiveSource{gameKeyCode, 0});
            if (KeyRefCount[gameKeyCode]++ == 0)
            {
                Queue.push_back({gameKeyCode, true});
            }
        }
        else if (!inputEvent.IsDown && wasDown)
        {
            const int previousCode = it->second.Code;
            ActiveSources.erase(it);
            if (--KeyRefCount[previousCode] == 0)
            {
                Queue.push_back({previousCode, false});
            }
        }
    }

    static void QueueRepeats()
    {
        const int repeatIntervalFrames = (easiershootflg && !speedrun_state.running) ? REPEAT_INTERVAL_FRAMES_EASY
                                                        : REPEAT_INTERVAL_FRAMES;

        for (auto& entry : ActiveSources)
        {
            ActiveSource& source = entry.second;
            if (source.RepeatSuppressed)
            {
                continue;
            }

            source.FramesHeld++;

            if (source.FramesHeld < REPEAT_DELAY_FRAMES)
            {
                continue;
            }

            if ((source.FramesHeld - REPEAT_DELAY_FRAMES) % repeatIntervalFrames == 0)
            {
                Queue.push_back({source.Code, true});
            }
        }
    }

    bool PollNext(InputEvent& out)
    {
        if (Queue.empty())
        {
            return false;
        }

        out = Queue.front();
        Queue.pop_front();
        return true;
    }

    int GetMouseIdleFrames()
    {
        return MouseIdleFrames;
    }
}
