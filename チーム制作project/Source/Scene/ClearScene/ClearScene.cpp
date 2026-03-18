#include "DxLib.h"
#include "ClearScene.h"
#include "../SceneManager.h"
#include "../../Input/Input.h"
#include "../PlayScene/PlayScene.h"
#include "../../Sound/SoundManager.h"
#include "../../UI/UIText.h"
#include "../../Enemy/Goal/Goal.h"
#include "../TitleScene/TitleScene.h"
#include "../../Score/Score.h"
#include "../../SaveData/SaveData.h"
#include "../../Player/Player.h"
#include "../../Timer/Timer.h"

// タイトルに戻る画像
int g_titleCHandle = -1;

// リザルト画像
int g_ResultHandle = -1;

// クリアタイム文字画像
int g_ClearTimeHandle = -1;

// フォント
int g_ClearTimeFontHandle = -1;

// BGM
int g_GameClearSEHandle = -1;

// ★保存したクリアタイム
int g_DisplayClearTime = 0;

void InitClearScene()
{
    g_titleCHandle = -1;
    g_ResultHandle = -1;
    g_ClearTimeHandle = -1;
    g_ClearTimeFontHandle = -1;
    g_GameClearSEHandle = -1;

    g_DisplayClearTime = 0;
}

void LoadClearScene()
{
    g_titleCHandle = LoadGraph("Data/Clear/画像/titleC.png");
    g_ResultHandle = LoadGraph("Data/Clear/画像/result.png");
    g_ClearTimeHandle = LoadGraph("Data/Clear/画像/ClearTime.png");

    g_GameClearSEHandle = LoadSoundMem("Data/Sound/BGM/GameClear.ogg");
}

void StartClearScene()
{
    PlaySoundMem(g_GameClearSEHandle, DX_PLAYTYPE_BACK);

    // ★ここでクリアタイムを取得（固定する）
    g_DisplayClearTime = GetClearTime();

    if (!g_IsTutorialMode)
    {
        int life;
        int score;
        int yellow;

        LoadGameData(life, score, yellow);
        SaveGameData(life, 0, yellow);
    }

    g_ClearTimeFontHandle = CreateFontToHandle("Agency FB", 80, 3);
}

void StepClearScene()
{
    if (IsTriggerKey(KEY_F))
    {
        g_ReturnFromGame = true;

        StopSoundMem(g_GameClearSEHandle);

        ChangeScene(SCENE_TITLE);
    }
}

void UpdateClearScene()
{
}

void DrawClearScene()
{
    DrawGraph(0, 0, g_ResultHandle, TRUE);
    DrawGraph(450, 250, g_ClearTimeHandle, TRUE);
    DrawGraph(535, 600, g_titleCHandle, TRUE);

    // 分と秒に変換
    int min = g_DisplayClearTime / 60;
    int sec = g_DisplayClearTime % 60;

    // 00:00形式で表示
    DrawFormatStringToHandle(
        1000,
        355,
        GetColor(255, 255, 255),
        g_ClearTimeFontHandle,
        "%02d:%02d",
        min,
        sec
    );
}

void FinClearScene()
{
    DeleteGraph(g_titleCHandle);
    DeleteGraph(g_ResultHandle);
    DeleteGraph(g_ClearTimeHandle);

    DeleteFontToHandle(g_ClearTimeFontHandle);

    DeleteSoundMem(g_GameClearSEHandle);
}