#include "Camera.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"
#include "../Map/MapParameter.h"

static CameraData g_CameraData = { 0.0f, 0.0f };
static int  g_CameraStage = 0;
static bool g_IsFirstUpdate = true;

void ResetCamera()
{
	g_CameraData.posX = 0.0f;
	g_CameraData.posY = 0.0f;
	g_IsFirstUpdate = true;
}

void UpdateCamera()
{
	PlayerData* player = GetPlayer();
	if (!player) return;

	// ----------------------------
	// X方向（中央追従）
	// ----------------------------
	float targetX = player->pos.x - SCREEN_WIDTH * 0.5f;
	if (targetX < 0.0f) targetX = 0.0f;
	g_CameraData.posX = targetX;

	// ----------------------------
	// Y方向（デッドゾーン方式）
	// ----------------------------
	const float DEAD_ZONE_Y = 120.0f;
	const float START_OFFSET_Y = 200.0f;

	float desiredY = player->pos.y - SCREEN_HEIGHT * 0.5f;
	float currentY = g_CameraData.posY;

	if (g_IsFirstUpdate)
	{
		currentY = desiredY - START_OFFSET_Y;
		g_IsFirstUpdate = false;
	}
	else
	{
		if (desiredY < currentY - DEAD_ZONE_Y)
		{
			currentY = desiredY + DEAD_ZONE_Y;
		}
		else if (desiredY > currentY + DEAD_ZONE_Y)
		{
			currentY = desiredY - DEAD_ZONE_Y;
		}
	}

	// ----------------------------
	// マップ上下限制限
	// ----------------------------
	int mapChipYNum = 0;

	switch (g_CameraStage)
	{
	case 0: mapChipYNum = 20;  break; // Tutorial
	case 1: mapChipYNum = 30;  break; // Stage1
	case 2: mapChipYNum = 133; break; // Stage2
	case 3: mapChipYNum = 34;  break; // Stage3
	}

	float mapBottomY = mapChipYNum * MAP_CHIP_HEIGHT;
	float cameraMinY = 0.0f;
	float cameraMaxY = mapBottomY - SCREEN_HEIGHT;
	if (cameraMaxY < cameraMinY) cameraMaxY = cameraMinY;

	if (currentY < cameraMinY)      currentY = cameraMinY;
	else if (currentY > cameraMaxY) currentY = cameraMaxY;

	g_CameraData.posY = currentY;
}


// ============================
// デバッグ描画
// ============================
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

void SetCameraStage(int stage)
{
	g_CameraStage = stage;
}

// ============================
// カメラ取得
// ============================
CameraData GetCamera()
{
	return g_CameraData;
}