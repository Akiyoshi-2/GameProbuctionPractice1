#include "Camera.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"

// カメラデータ（1つだけ持つ）
static CameraData g_CameraData = { 0.0f, 0.0f };

void UpdateCamera()
{
    PlayerData* player = GetPlayer();

    // プレイヤーを画面中央に来るようにする
    g_CameraData.posX = player->posX - SCREEN_WIDTH * 0.5f;
    g_CameraData.posY = player->posY - SCREEN_HEIGHT * 0.5f;

    // カメラがマイナスにならないよう制限
    if (g_CameraData.posX < 0.0f)
        g_CameraData.posX = 0.0f;

    if (g_CameraData.posY < 0.0f)
        g_CameraData.posY = 0.0f;
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