#include "Camera.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"

// カメラデータ（1つだけ持つ）
static CameraData g_CameraData = { 0.0f, 0.0f };

void UpdateCamera()
{
    PlayerData* player = GetPlayer();

    g_CameraData.posX = player->posX - SCREEN_WIDTH * 0.5f;
    g_CameraData.posY = player->posY - SCREEN_HEIGHT * 0.5f;
}
void DrawCamera()
{
    DrawFormatString(
        0, 0,
        GetColor(255, 255, 255),
        "Camera = [%.2f, %.2f]",
        g_CameraData.posX,
        g_CameraData.posY
    );
}

CameraData GetCamera()
{
    return g_CameraData;
}