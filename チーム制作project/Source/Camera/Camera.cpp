#include "Camera.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"

// カメラデータ
static CameraData g_CameraData = { 0.0f, 0.0f };

// カメラがこれまで到達した「最下点」
static float g_CameraLowerLimitY = 0.0f;

void UpdateCamera()
{
    static bool isFirstUpdate = true;

    PlayerData* player = GetPlayer();

    // ===== X =====
    float targetX = player->pos.x - SCREEN_WIDTH * 0.5f;
    if (targetX < 0.0f)
    {
        targetX = 0.0f;
    }
    g_CameraData.posX = targetX;

    // ===== Y =====
    const float DEAD_ZONE_Y = 120.0f;
    const float START_OFFSET_Y = 200.0f;   // ← ここで初期高さを調整

    float desiredY = player->pos.y - SCREEN_HEIGHT * 0.5f;
    float currentY = g_CameraData.posY;

    // 初回
    if (isFirstUpdate)
    {
        g_CameraData.posY = desiredY - START_OFFSET_Y;   // ← 上にずらす
        g_CameraLowerLimitY = g_CameraData.posY;         // ← その高さを下限に
        isFirstUpdate = false;
        return;
    }

    // ===== 追従 =====
    if (desiredY < currentY - DEAD_ZONE_Y)
    {
        // 上に追従（Yが小さくなる）
        currentY = desiredY + DEAD_ZONE_Y;
    }
    else if (desiredY > currentY + DEAD_ZONE_Y)
    {
        // 下に追従（Yが大きくなる）
        currentY = desiredY - DEAD_ZONE_Y;
    }

    // ===== 下限制限（これ以上下に行かせない）=====
    if (currentY > g_CameraLowerLimitY)
    {
        currentY = g_CameraLowerLimitY;
    }

    // ===== 下限更新（さらに下に行けたら）=====
    if (currentY > g_CameraLowerLimitY)
    {
        g_CameraLowerLimitY = currentY;
    }

    g_CameraData.posY = currentY;
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