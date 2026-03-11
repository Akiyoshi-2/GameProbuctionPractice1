#include "DxLib.h"
#include "Score.h"
#include "../GameSetting/GameSetting.h"
#include "../Player/Player.h"

#define SCORE_POS_X (SCREEN_WIDTH - 500)
#define SCORE_POS_Y (30)

static int g_Score = 0;
static int g_FontHandle = -1;
static int g_NextLifeScore = 5000;

void InitScore()
{
    g_Score = 0;
    g_NextLifeScore = 5000;

    // Agency FB ƒtƒHƒ“ƒg
    g_FontHandle = CreateFontToHandle("Agency FB", 40, 3);
}

void UpdateScore()
{

}

void DrawScore()
{
    DrawFormatStringToHandle(
        SCORE_POS_X,
        SCORE_POS_Y,
        GetColor(255, 255, 255),
        g_FontHandle,
        "SCORE : %d",
        g_Score
    );
}

void AddScore(int score)
{
    g_Score += score;

    // 0–¢–ž‚É‚µ‚È‚¢
    if (g_Score < 0)
    {
        g_Score = 0;
    }

    if (g_Score >= g_NextLifeScore)
    {
        PlayerData* player = GetPlayer();

        if (player->life < PLAYER_MAX_LIFE)
        {
            player->life++;
        }

        g_NextLifeScore += 5000;
    }
}

int GetScore()
{
    return g_Score;
}