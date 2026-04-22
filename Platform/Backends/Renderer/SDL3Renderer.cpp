#include "SDL3Renderer.h"

#include "IWindow.h"
#include "SDL3Window.h"

#include <algorithm>
#include <cstdio>

SDL3Renderer::~SDL3Renderer()
{
    Destroy();
}

bool SDL3Renderer::Init(IWindow* hostWindow)
{
    Window = hostWindow;
    SDL3Window* sdlWindow = dynamic_cast<SDL3Window*>(Window);
    if (sdlWindow == nullptr)
    {
        printf("SDL3Renderer requires SDL3Window!\n");
        Window = nullptr;
        return false;
    }
    SdlWindow = sdlWindow->GetNativeWindow();

    Renderer = SDL_CreateRenderer(SdlWindow, nullptr);
    if (Renderer == nullptr)
    {
        printf("SDL_CreateRenderer failed! %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void SDL3Renderer::DisplaySetVSync(bool enabled)
{
    SDL_SetRenderVSync(Renderer, enabled ? 1 : 0);
}

void SDL3Renderer::Destroy()
{
    if (Texture != nullptr)
    {
        SDL_DestroyTexture(Texture);
        Texture = nullptr;
    }
    if (Renderer != nullptr)
    {
        SDL_DestroyRenderer(Renderer);
        Renderer = nullptr;
    }
    Window = nullptr;
    SdlWindow = nullptr;
    TextureWidth = 0;
    TextureHeight = 0;
}

bool SDL3Renderer::EnsureTexture(int width, int height)
{
    if (Texture != nullptr && TextureWidth == width && TextureHeight == height)
    {
        return true;
    }

    if (Texture != nullptr)
    {
        SDL_DestroyTexture(Texture);
        Texture = nullptr;
    }

    Texture = SDL_CreateTexture(
        Renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height);

    if (Texture == nullptr)
    {
        printf("SDL3Renderer: SDL_CreateTexture failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetTextureScaleMode(Texture, SDL_SCALEMODE_NEAREST);
    TextureWidth = width;
    TextureHeight = height;
    return true;
}

void SDL3Renderer::BeginFrame()
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
}

void SDL3Renderer::DrawFrame(const unsigned char* rgbaPixels, int width, int height)
{
    if (!EnsureTexture(width, height))
    {
        return;
    }

    SDL_UpdateTexture(Texture, nullptr, rgbaPixels, width * 4);

    const WindowSize pixelSize = Window->GetPixelSize();
    const float scaleX = static_cast<float>(pixelSize.Width) / width;
    const float scaleY = static_cast<float>(pixelSize.Height) / height;
    const float scale = std::min(scaleX, scaleY);

    SDL_FRect dst;
    dst.w = width * scale;
    dst.h = height * scale;
    dst.x = (pixelSize.Width - dst.w) * 0.5f;
    dst.y = (pixelSize.Height - dst.h) * 0.5f;

    SDL_RenderTexture(Renderer, Texture, nullptr, &dst);
}

void SDL3Renderer::EndFrame()
{
    SDL_RenderPresent(Renderer);
}
