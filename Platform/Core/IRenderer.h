#pragma once

class IWindow;

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual bool Init(IWindow* hostWindow) = 0;
    virtual void Destroy() = 0;

    virtual void BeginFrame() = 0;
    virtual void DrawFrame(const unsigned char* rgbaPixels, int width, int height) = 0;
    virtual void EndFrame() = 0;

    virtual void DisplaySetVSync(bool enabled) = 0;
};
