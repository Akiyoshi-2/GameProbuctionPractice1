#include "Timer.h"
#include "../GameSetting/GameSetting.h"

int g_FontHandle = -1;
int g_TimeFrame = 0;

void InitTimer()
{
	// Agency FB フォント作成
	g_FontHandle = CreateFontToHandle("Agency FB", 64, 3);

	g_TimeFrame = 0;
}

void UpdateTimer()
{
	g_TimeFrame++;
}

void DrawTimer()
{
	int totalSeconds = g_TimeFrame / 60;

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    char timeText[32];
    sprintf_s(timeText, "%02d:%02d", minutes, seconds);

    // 文字幅取得
    int strWidth = GetDrawStringWidthToHandle(
        timeText,
        strlen(timeText),
        g_FontHandle
    );

    // 右上（余白20px）
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

void Timer_Reset()
{
    g_TimeFrame = 0;
}