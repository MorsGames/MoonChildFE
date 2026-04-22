#include "mydefs.hpp"

extern bool frmwrk_usefastfile;

int g_SettingsFlg;
int g_ReqKeyFlg;
int g_KeyTimeOut;

int g_MouseFlg;
int g_MouseActualFlg;
int g_MouseXDown;
int g_MouseYDown;
int g_MouseXCurrent;
int g_MouseYCurrent;

int g_CurDeltaX;

int frmwrk_CenterX;
int frmwrk_CenterY;

unsigned short* SettingsPic;

void framework_usefastfile(bool offOn)
{
    frmwrk_usefastfile = offOn;
}
