#pragma once

#include "IRenderer.h"

#include <SDL3/SDL.h>

// Simplier backend that uses SDL3's default renderer
// Worse for when scaling the image up to weird resolutions, but easier to set up
class SDL3Renderer final : public IRenderer
{
public:
    SDL3Renderer() = default;
    ~SDL3Renderer() override;

    bool Init(IWindow* hostWindow) override;
    void Destroy() override;

    void BeginFrame() override;
    void DrawFrame(const unsigned char* rgbaPixels, int width, int height) override;
    void EndFrame() override;

    void DisplaySetVSync(bool enabled) override;

private:
    bool EnsureTexture(int width, int height);

    IWindow* Window = nullptr;
    SDL_Window* SdlWindow = nullptr;
    SDL_Renderer* Renderer = nullptr;
    SDL_Texture* Texture = nullptr;
    int TextureWidth = 0;
    int TextureHeight = 0;
};
