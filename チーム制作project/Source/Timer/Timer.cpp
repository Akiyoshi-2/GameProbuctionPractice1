#include "DxLib.h"
#include "Timer.h"
#include "../GameSetting/GameSetting.h"
#include "../Scene/SceneManager.h"
#include "../Scene/GameOverScene/GameOverScene.h"
#include "../Player/Player.h"
#include "../Score/Score.h"
#include "../SaveData/SaveData.h"
#include "../Player/YellowSelect/YellowSelect.h"
#include "../Player/YellowStock/YellowStock.h"

// フォント
int g_FontHandle = -1;

// 制限時間
int g_StartTime = 0;
int g_CurrentStage = 0;
int g_LimitTime = 180;
int g_RemainTime = 180;

// クリアタイム
int g_PlayStartTime = 0;
int g_ClearTime = 0;

//ステージごとのクリアタイム
int g_ClearTimeList[4] = { 0 };

void SetTimerStage(int stage)
{
    g_CurrentStage = stage;

    switch (g_CurrentStage)
    {
    case 0:  g_LimitTime = 999; break;
    case 1:  g_LimitTime = 250; break;
    case 2:  g_LimitTime = 500; break;
    case 3:  g_LimitTime = 500; break;
    default: g_LimitTime = 180; break;
    }
}

void InitTimer()
{
    g_FontHandle = CreateFontToHandle("Agency FB", 64, 3);

    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;

    g_PlayStartTime = GetNowCount();
}

void StepTimer()
{
    if (g_IsYellowSelecting) return;
}

void UpdateTimer()
{
    if (g_IsYellowSelecting) return;

    int now = GetNowCount();
    int elapsed = (now - g_StartTime) / 1000;

    g_RemainTime = g_LimitTime - elapsed;

    if (g_RemainTime <= 0)
    {
        g_RemainTime = 0;

        PlayerData* player = GetPlayer();

        player->life = 5;
        SetScore(0);

        int yellow = GetYellowStock();
        SaveGameData(player->life, GetScore(), yellow);

        ChangeScene(SCENE_GAMEOVER);
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
        GetColor(0, 0, 0),
        g_FontHandle
    );
}

void ResetTimer()
{
    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;
}

void SaveClearTime(int stage)
{
    int now = GetNowCount();
    int time = (now - g_PlayStartTime) / 1000;

    g_ClearTime = time;

    if (stage >= 1 && stage <= 3)
    {
        g_ClearTimeList[stage] = time;
    }
}

int GetClearTime()
{
    return g_ClearTime;
}

int GetStageClearTime(int stage)
{
    if (stage >= 1 && stage <= 3)
    {
        return g_ClearTimeList[stage];
    }
    return 0;
}

int GetRemainTime()
{
    return g_RemainTime;
}

int GetLimitTime()
{
    return g_LimitTime;
}

int GetCurrentStage()
{
    return g_CurrentStage;
}