#pragma once

class IInput;

struct WindowSize
{
    int Width = 0;
    int Height = 0;
};

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual bool Create(const char* title, int width, int height) = 0;
    virtual void Destroy() = 0;

    virtual WindowSize GetPixelSize() const = 0;

    virtual void DisplaySetFullscreen(bool enabled) = 0;

    virtual void PumpOSEvents(IInput* sink, bool& outExitRequested) = 0;
};
