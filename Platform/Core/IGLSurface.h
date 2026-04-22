#pragma once

class IGLSurface
{
public:
    virtual ~IGLSurface() = default;
    virtual void MakeCurrent() = 0;
    virtual void SwapBuffers() = 0;
    virtual void SetSwapInterval(int interval) = 0;
};
