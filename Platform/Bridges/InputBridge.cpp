#include "InputBridge.h"

#include <cstring>
#include <deque>
#include <unordered_map>

namespace InputBridge
{
    static constexpr int REPEAT_DELAY_FRAMES = 30;
    static constexpr int REPEAT_INTERVAL_FRAMES = 2;

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

    static bool IsValidGameKey(int gameKeyCode)
    {
        return gameKeyCode > 0 && gameKeyCode < 256;
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

    static void SubmitEvent(const InputEvent& inputEvent);
    static void QueueRepeats();

    void Attach(IInput* input)
    {
        InputBackend = input;
        ClearState();
    }

    void Detach()
    {
        InputBackend = nullptr;
        ClearState();
    }

    void Tick()
    {
        if (InputBackend != nullptr)
        {
            InputEvent inputEvent;
            while (InputBackend->PollNext(inputEvent))
            {
                SubmitEvent(inputEvent);
            }
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

            for (auto& pair : ActiveSources)
            {
                pair.second.RepeatSuppressed = true;
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

            if ((source.FramesHeld - REPEAT_DELAY_FRAMES) % REPEAT_INTERVAL_FRAMES == 0)
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
}
