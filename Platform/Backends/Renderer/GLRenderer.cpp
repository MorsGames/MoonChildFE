#include "GLRenderer.h"

#include "IGLSurface.h"
#include "IWindow.h"
#include "PlatformConfig.h"

#include <SDL3/SDL_video.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <vector>

static const char* VERTEX_SRC =
    "precision highp float;\n"
    "attribute vec2 aPosition;\n"
    "attribute vec2 aTexCoord;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
    "    vTexCoord = aTexCoord;\n"
    "}\n";

// We could just scale it up with nearest-neighbour scaling, but that'd give us a lot of shimmering on awkward resolutions
// This shader fixes that
static const char* FRAGMENT_SRC =
    "precision mediump float;\n"
    "uniform sampler2D uSource;\n"
    "uniform vec2 uSourceSize;\n"
    "uniform vec2 uScale;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    vec2 texel = vTexCoord * uSourceSize;\n"
    "    vec2 texel_floored = floor(texel);\n"
    "    vec2 s = fract(texel);\n"
    "    vec2 region_range = 0.5 - 0.5 / uScale;\n"
    "    vec2 center_dist = s - 0.5;\n"
    "    vec2 f = (center_dist - clamp(center_dist, -region_range, region_range)) * uScale + 0.5;\n"
    "    vec2 mod_texel = texel_floored + f;\n"
    "    gl_FragColor = texture2D(uSource, mod_texel / uSourceSize);\n"
    "}\n";

static const GLfloat QUAD_VERTICES[] =
{
    -1.0f,  1.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 1.0f,
};

static void* LoadGlProc(const char* name)
{
    return reinterpret_cast<void*>(SDL_GL_GetProcAddress(name));
}

static GLuint CompileShader(GLenum stage, const char* src)
{
    GLuint shader = glCreateShader(stage);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE)
    {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen > 0 ? logLen : 1, 0);
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        printf("Shader compilation failed! %s\n", log.data());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool GLRenderer::BuildSharpBilinearProgram()
{
    GLuint vs = CompileShader(GL_VERTEX_SHADER, VERTEX_SRC);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FRAGMENT_SRC);
    if (vs == 0 || fs == 0)
    {
        if (vs != 0) glDeleteShader(vs);
        if (fs != 0) glDeleteShader(fs);
        return false;
    }

    Program = glCreateProgram();
    glAttachShader(Program, vs);
    glAttachShader(Program, fs);
    glLinkProgram(Program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint linked = GL_FALSE;
    glGetProgramiv(Program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        GLint logLen = 0;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen > 0 ? logLen : 1, 0);
        glGetProgramInfoLog(Program, logLen, nullptr, log.data());
        printf("Program link failed! %s\n", log.data());
        glDeleteProgram(Program);
        Program = 0;
        return false;
    }

    LocSource = glGetUniformLocation(Program, "uSource");
    LocSourceSize = glGetUniformLocation(Program, "uSourceSize");
    LocScale = glGetUniformLocation(Program, "uScale");
    LocPosition = glGetAttribLocation(Program, "aPosition");
    LocTexCoord = glGetAttribLocation(Program, "aTexCoord");

    return LocSource >= 0
        && LocSourceSize >= 0
        && LocScale >= 0
        && LocPosition >= 0
        && LocTexCoord >= 0;
}

bool GLRenderer::Init(IWindow* hostWindow)
{
    Window = hostWindow;

    Surface = dynamic_cast<IGLSurface*>(hostWindow);
    if (Surface == nullptr)
    {
        printf("Window does not implement IGLSurface!\n");
        Window = nullptr;
        return false;
    }
    Surface->MakeCurrent();

    if (gladLoadGLES2Loader(LoadGlProc) == 0)
    {
        printf("gladLoadGLES2Loader failed!\n");
        Window = nullptr;
        Surface = nullptr;
        return false;
    }

    const int w = GAME_FRAMEBUFFER_WIDTH;
    const int h = GAME_FRAMEBUFFER_HEIGHT;
    SourceWidth = w;
    SourceHeight = h;
    const std::vector<unsigned char> zeros(w * h * 4, 0);

    // Hack to force disable sRGB, which the drivers seem to enable on Windows regardless of what SDL says!
    glDisable(0x8DB9); // GL_FRAMEBUFFER_SRGB

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, zeros.data());

    if (!BuildSharpBilinearProgram())
    {
        printf("GL shader program initialization failed!\n");
        Destroy();
        return false;
    }

    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void GLRenderer::Destroy()
{
    if (Surface != nullptr)
    {
        Surface->MakeCurrent();
    }
    if (Program != 0)
    {
        glDeleteProgram(Program);
        Program = 0;
    }
    if (Texture != 0)
    {
        glDeleteTextures(1, &Texture);
        Texture = 0;
    }
    if (VertexBuffer != 0)
    {
        glDeleteBuffers(1, &VertexBuffer);
        VertexBuffer = 0;
    }
    Window = nullptr;
    Surface = nullptr;
}

void GLRenderer::BeginFrame()
{
    Surface->MakeCurrent();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::SetGlViewport(int windowWidth, int windowHeight, int bufferWidth, int bufferHeight)
{
    const float scaleX = static_cast<float>(windowWidth) / bufferWidth;
    const float scaleY = static_cast<float>(windowHeight) / bufferHeight;
    const float scale = std::min(scaleX, scaleY);

    const int viewportWidth = static_cast<int>(bufferWidth * scale);
    const int viewportHeight = static_cast<int>(bufferHeight * scale);
    const int viewportX = (windowWidth - viewportWidth) / 2;
    const int viewportY = (windowHeight - viewportHeight) / 2;

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
    ViewportWidth = viewportWidth;
    ViewportHeight = viewportHeight;
}

void GLRenderer::DrawFrame(const unsigned char* rgbaPixels, int width, int height)
{
    Surface->MakeCurrent();

    const WindowSize pixelSize = Window->GetPixelSize();
    SetGlViewport(pixelSize.Width, pixelSize.Height, width, height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgbaPixels);

    const float scaleX = std::max(1, ViewportWidth / width);
    const float scaleY = std::max(1, ViewportHeight / height);

    glUseProgram(Program);
    glUniform1i(LocSource, 0);
    glUniform2f(LocSourceSize, width, height);
    glUniform2f(LocScale, scaleX, scaleY);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glEnableVertexAttribArray(LocPosition);
    glEnableVertexAttribArray(LocTexCoord);
    glVertexAttribPointer(LocPosition, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(0));
    glVertexAttribPointer(LocTexCoord, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(2 * sizeof(GLfloat)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(LocTexCoord);
    glDisableVertexAttribArray(LocPosition);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void GLRenderer::EndFrame()
{
    Surface->SwapBuffers();
}

void GLRenderer::DisplaySetVSync(bool enabled)
{
    Surface->SetSwapInterval(enabled ? 1 : 0);
}
