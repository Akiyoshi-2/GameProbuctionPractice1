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

// 制限時間用
int g_StartTime = 0;
int g_CurrentStage = 0;
int g_LimitTime = 180;
int g_RemainTime = 180;

// クリアタイム用
int g_PlayStartTime = 0;
int g_ClearTime = 0;

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

    // 制限時間
    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;

    // クリアタイム用
    g_PlayStartTime = GetNowCount();
}

void StepTimer()
{
    if (g_IsYellowSelecting) return;
}

void UpdateTimer()
{
    if (g_IsYellowSelecting) return;

    // 制限時間計算
    int now = GetNowCount();
    int elapsed = (now - g_StartTime) / 1000;

    g_RemainTime = g_LimitTime - elapsed;

    if (g_RemainTime <= 0)
    {
        g_RemainTime = 0;

        // プレイヤー取得
        PlayerData* player = GetPlayer();

        // ライフとスコア初期化
        player->life = 5;
        SetScore(0);

        // セーブデータ更新
        int yellow = GetYellowStock();
        SaveGameData(player->life, GetScore(), yellow);

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
        GetColor(0, 0, 0),
        g_FontHandle
    );
}

// 制限時間リセット（死亡時など）
void ResetTimer()
{
    g_StartTime = GetNowCount();
    g_RemainTime = g_LimitTime;

}

//ここからクリアタイム関連
// クリア時に呼ぶ
void SaveClearTime()
{
    int now = GetNowCount();
    g_ClearTime = (now - g_PlayStartTime) / 1000;
}

// クリアタイム取得
int GetClearTime()
{
    return g_ClearTime;
}

int GetRemainTime()
{
    return g_RemainTime;
}

int GetLimitTime()
{
    return g_LimitTime;
}