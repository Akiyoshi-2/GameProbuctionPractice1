#include "Timer.h"
#include "../GameSetting/GameSetting.h"

#define START_TIME    180

int g_FontHandle = -1;
int g_StartTime = 0;
int g_RemainTime = START_TIME;

void InitTimer()
{
	//("フォント名",サイズ,太さ)
    g_FontHandle = CreateFontToHandle("Agency FB", 64, 3);

    g_StartTime = GetNowCount();

    g_RemainTime = START_TIME;
}

void UpdateTimer()
{
    //現在の経過時間
    int now = GetNowCount();
    int elapsed = (now - g_StartTime) / 1000;

    //スタート時間から経過時間を引く
    g_RemainTime = START_TIME - elapsed;

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
    g_RemainTime = START_TIME;
}