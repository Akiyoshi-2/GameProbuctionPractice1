#include "Timer.h"
#include "../GameSetting/GameSetting.h"

int g_FontHandle = -1;
int g_StartTime = 0;
int g_CurrentStage = 0;
int g_LimitTime = 180;
int g_RemainTime = 180;

void SetTimerStage(int stage)
{
    g_CurrentStage = stage;

    switch (g_CurrentStage)
    {
    case 0:  g_LimitTime = 999; break; // チュートリアル
    case 1:  g_LimitTime = 180; break;
    case 2:  g_LimitTime = 120; break;
    case 3:  g_LimitTime = 90;  break;
    default: g_LimitTime = 180; break;
    }
}

void InitTimer()
{
    g_FontHandle = CreateFontToHandle("Agency FB", 64, 3);

    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;
}

void UpdateTimer()
{
    //現在の経過時間
    int now = GetNowCount();
    int elapsed = (now - g_StartTime) / 1000;

    //スタート時間から経過時間を引く
    g_RemainTime = g_LimitTime - elapsed;

    if (g_RemainTime < 0)
    {
        g_RemainTime = 0;   // 0で止めるだけ
    }
}

void DrawTimer()
{
    char timeText[16];
    sprintf_s(timeText, "%d", g_RemainTime);

    int strWidth = GetDrawStringWidthToHandle(
        timeText,
        strlen(timeText),
        g_FontHandle
    );

    int posX = SCREEN_WIDTH - strWidth - 20;
    int posY = 20;

    DrawStringToHandle(
        posX,
        posY,
        timeText,
        GetColor(255, 255, 255),
        g_FontHandle
    );
}

void ResetTimer()
{
    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;
}