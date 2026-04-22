#pragma once

namespace InputBridge
{
    void Attach();
    void Detach();

    enum InputContext : unsigned char
    {
        INPUT_CONTEXT_MENU,
        INPUT_CONTEXT_REMAP,
        INPUT_CONTEXT_GAME
    };

    extern InputContext Context;

    extern int BindUp;
    extern int BindDown;
    extern int BindLeft;
    extern int BindRight;
    extern int BindJump;
    extern int BindAction;

    void SetContext(InputContext context);
    void SetBindings(int up, int down, int left, int right, int jump, int action);
}
