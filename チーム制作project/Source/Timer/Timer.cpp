#include "Timer.h"
#include "../GameSetting/GameSetting.h"
#include "../Scene/SceneManager.h"
#include "../Scene/GameOverScene/GameOverScene.h"
#include "../Player/Player.h"
#include "../Score/Score.h"
#include "../SaveData/SaveData.h"
#include "../Player/YellowSelect/YellowSelect.h"

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
}

void StepTimer()
{
    if (g_IsYellowSelecting) return;
}

void UpdateTimer()
{
    if (g_IsYellowSelecting) return;

    //現在の経過時間
    int now = GetNowCount();
    int elapsed = (now - g_StartTime) / 1000;

    //スタート時間から経過時間を引く
    g_RemainTime = g_LimitTime - elapsed;

    if (g_RemainTime <= 0)
    {
        g_RemainTime = 0;

        // プレイヤー取得
        PlayerData* player = GetPlayer();

        // ライフとスコア初期化
        player->life = 3;
        SetScore(0);

        // セーブデータ更新
        SaveGameData(player->life, GetScore());

        // GameOverへ
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
        GetColor(255, 255, 255),
        g_FontHandle
    );
}

void ResetTimer()
{
    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;
}