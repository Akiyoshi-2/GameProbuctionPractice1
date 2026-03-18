#include "DxLib.h"
#include "GameClearScene.h"
#include "../SceneManager.h"
#include "../../Timer/Timer.h"
#include "../../Player/Player.h"
#include "../../Sound/SoundManager.h"

// 画像
int g_BackHandle = -1;

int g_ClearFontHandle = -1;

GameClearData g_GameClear;

struct ClearAnimParam
{
    int interval;
    int frameNum;
    int width;
    int height;
};

// アニメ設定（3つのみ）
const ClearAnimParam CLEAR_ANIM_PARAM[CLEAR_ANIM_MAX] =
{
    {20, 2, 50, 50}, // RED
    {20, 2, 50, 50}, // BLUE
    {20, 2, 50, 50}, // YELLOW
};

void InitGameClear()
{
    g_BackHandle = -1;

    g_ClearFontHandle = CreateFontToHandle("Agency FB", 40, 3);

    g_GameClear.clearAnim = CLEAR_ANIM_NONE;

    for (int i = 0; i < CLEAR_ANIM_MAX; i++)
    {
        InitAnimation(&g_GameClear.animation[i]);
    }

    // 表示位置
    g_GameClear.pos[RED_PLAYER_WIN] = { 300.0f, 600.0f, 0.0f };
    g_GameClear.pos[BLUE_PLAYER_WIN] = { 400.0f, 400.0f, 0.0f };
    g_GameClear.pos[YELLOW_PLAYER_WIN] = { 200.0f, 400.0f, 0.0f };
}

void LoadGameClear()
{
    g_BackHandle = LoadGraph("Data/GameClear/GameClearBG.png");

    g_GameClear.animation[RED_PLAYER_WIN].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_win.png");
    g_GameClear.animation[BLUE_PLAYER_WIN].handle = LoadGraph("Data/animation/BluePlayer/BluePlayer_win.png");
    g_GameClear.animation[YELLOW_PLAYER_WIN].handle = LoadGraph("Data/animation/YellowPlayer/YellowPlayer_win.png");
}

void StartClearAnimation(GameClearAnimationType anim)
{
    AnimationData* animData = &g_GameClear.animation[anim];
    ClearAnimParam param = CLEAR_ANIM_PARAM[anim];

    VECTOR pos = g_GameClear.pos[anim];

    StartAnimation(
        animData,
        pos.x,
        pos.y,
        param.interval,
        param.frameNum,
        param.width,
        param.height,
        true
    );

    PlayBGM(BGM_ALL_CLEAR);
}

void StartGameClear()
{
    for (int i = 0; i < CLEAR_ANIM_MAX; i++)
    {
        StartClearAnimation((GameClearAnimationType)i);
    }
}

void StepGameClear()
{
}

void UpdateGameClear()
{
    for (int i = 0; i < CLEAR_ANIM_MAX; i++)
    {
        UpdateAnimation(&g_GameClear.animation[i]);
    }
}

void DrawGameClear()
{
    DrawGraph(0, 0, g_BackHandle, TRUE);

    for (int i = 0; i < CLEAR_ANIM_MAX; i++)
    {
        AnimationData* anim = &g_GameClear.animation[i];
        VECTOR pos = g_GameClear.pos[i];

        int w = anim->framWidth;
        int h = anim->framHeight;

        int srcX = anim->nowFrame * w;
        int srcY = 0;

        float scale = 3.0f;

        int drawW = (int)(w * scale);
        int drawH = (int)(h * scale);

        DrawRectExtendGraph(
            (int)(pos.x - drawW / 2),
            (int)(pos.y - drawH / 2),
            (int)(pos.x + drawW / 2),
            (int)(pos.y + drawH / 2),
            srcX, srcY, w, h,
            anim->handle,
            TRUE
        );
    }

    int font = g_ClearFontHandle;

    // STAGE1
    int t1 = GetStageClearTime(1);
    DrawFormatStringToHandle(800, 300, GetColor(255, 255, 255), font,
        "STAGE1 : %02d:%02d", t1 / 60, t1 % 60);

    // STAGE2
    int t2 = GetStageClearTime(2);
    DrawFormatStringToHandle(800, 400, GetColor(255, 255, 255), font,
        "STAGE2 : %02d:%02d", t2 / 60, t2 % 60);

    // STAGE3
    int t3 = GetStageClearTime(3);
    DrawFormatStringToHandle(800, 500, GetColor(255, 255, 255), font,
        "STAGE3 : %02d:%02d", t3 / 60, t3 % 60);

    // TOTAL
    int total = t1 + t2 + t3;
    DrawFormatStringToHandle(800, 600, GetColor(255, 255, 0), font,
        "TOTAL : %02d:%02d", total / 60, total % 60);

    // 死亡回数
    int death = GetPlayerDeathCount();
    DrawFormatStringToHandle(1100, 300, GetColor(255, 100, 100),font,
        "DEATH : %d", death
    );
}

void FinGameClear()
{
    DeleteGraph(g_BackHandle);

    for (int i = 0; i < CLEAR_ANIM_MAX; i++)
    {
        DeleteGraph(g_GameClear.animation[i].handle);
    }

    DeleteFontToHandle(g_ClearFontHandle);

    StopBGM(BGM_ALL_CLEAR);
}