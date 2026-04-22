#pragma once

#include "IRenderer.h"

#include "glad.h"

class IGLSurface;

// Fancier backend that uses OpenGL ES 2.0
// Tries to correct shimmering on awkward resolutions
class GLRenderer final : public IRenderer
{
public:
    bool Init(IWindow* hostWindow) override;
    void Destroy() override;

    void BeginFrame() override;
    void DrawFrame(const unsigned char* rgbaPixels, int width, int height) override;
    void EndFrame() override;

    void DisplaySetVSync(bool enabled) override;

private:
    void SetGlViewport(int windowWidth, int windowHeight, int bufferWidth, int bufferHeight);
    bool BuildSharpBilinearProgram();

    IWindow* Window = nullptr;
    IGLSurface* Surface = nullptr;
    GLuint Texture = 0;
    GLuint VertexBuffer = 0;

    GLuint Program = 0;
    GLint LocPosition = -1;
    GLint LocTexCoord = -1;
    GLint LocSource = -1;
    GLint LocSourceSize = -1;
    GLint LocScale = -1;

    int ViewportWidth = 0;
    int ViewportHeight = 0;
    int SourceWidth = 0;
    int SourceHeight = 0;
};
