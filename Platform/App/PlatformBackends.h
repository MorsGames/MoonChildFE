#pragma once

#include "IAudio.h"
#include "IInput.h"
#include "IRenderer.h"
#include "IWindow.h"

#include <memory>

struct PlatformBackends
{
    std::unique_ptr<IWindow> Window;
    std::unique_ptr<IRenderer> Renderer;
    std::unique_ptr<IInput> Input;
    std::unique_ptr<IAudio> Audio;
};

PlatformBackends MakeDefaultBackends();
