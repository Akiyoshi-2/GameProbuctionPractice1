#include "Camera.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"

static float g_CameraX = 0.0f;
static float g_CameraY = 0.0f;

CameraData g_CameraData = { 0 };


void UpdateCamera()
{
    //プレイヤーを置きたい画面位置（少し左）
    const float PLAYER_SCREEN_RATIO = 0.2f; // 0.5 = 中央
    float targetX = SCREEN_WIDTH * PLAYER_SCREEN_RATIO;

    PlayerData* player = GetPlayer();

    // プレイヤーが targetX を超えたら追従
    if (player->posX >= targetX)
    {
        g_CameraX = player->posX - targetX;
    }
    else
    {
        // 最初は固定
        g_CameraX = 0.0f;
    }

    // 縦は固定
    g_CameraY = 0.0f;
}
CameraData GetCamera()
{
    return g_CameraData;
}
