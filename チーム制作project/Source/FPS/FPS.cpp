#include "DxLib.h"
#include "FPS.h"

static int g_LastTime = 0;

void InitFPS()
{
    g_LastTime = GetNowCount();
}

void WaitFPS()
{
    int now = GetNowCount();
    int frameTime = 1000 / 60; // 60FPS = –ñ16.66ms

    int elapsed = now - g_LastTime;

    if (elapsed < frameTime)
    {
        WaitTimer(frameTime - elapsed);
    }

    g_LastTime = GetNowCount();
}