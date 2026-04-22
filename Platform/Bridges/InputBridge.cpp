#include "InputBridge.h"

namespace InputBridge
{
    InputContext Context = INPUT_CONTEXT_MENU;

    int BindUp    = 38;
    int BindDown  = 40;
    int BindLeft  = 37;
    int BindRight = 39;
    int BindJump  = 38;
    int BindAction = ' ';

    void Attach()
    {
    }

    void Detach()
    {
        Context = INPUT_CONTEXT_MENU;
    }

    void SetContext(InputContext context)
    {
        Context = context;
    }

    void SetBindings(int up, int down, int left, int right, int jump, int action)
    {
        BindUp = up;
        BindDown = down;
        BindLeft = left;
        BindRight = right;
        BindJump = jump;
        BindAction = action;
    }
}
