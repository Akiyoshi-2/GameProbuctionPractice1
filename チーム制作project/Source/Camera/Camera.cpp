#include "Camera.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"
#include "../Map/MapParameter.h"

// ============================
// カメラデータ
// ============================
static CameraData g_CameraData = { 0.0f, 0.0f };

// ============================
// カメラ更新
// ============================
void UpdateCamera()
{
	static bool isFirstUpdate = true;

	PlayerData* player = GetPlayer();

	// ============================
	// X方向（左右追従）
	// ============================
	float targetX = player->pos.x - SCREEN_WIDTH * 0.5f;
	if (targetX < 0.0f)
	{
		targetX = 0.0f;
	}

	g_CameraData.posX = targetX;

	// ============================
	// Y方向（上下追従）
	// ============================
	const float DEAD_ZONE_Y = 120.0f;
	const float START_OFFSET_Y = 200.0f;

	float desiredY = player->pos.y - SCREEN_HEIGHT * 0.5f;
	float currentY = g_CameraData.posY;

	// 初回のみカメラ初期位置を少し上に
	if (isFirstUpdate)
	{
		currentY = desiredY - START_OFFSET_Y;
		isFirstUpdate = false;
	}
	else
	{
		// デッドゾーン付き追従
		if (desiredY < currentY - DEAD_ZONE_Y)
		{
			currentY = desiredY + DEAD_ZONE_Y;
		}
		else if (desiredY > currentY + DEAD_ZONE_Y)
		{
			currentY = desiredY - DEAD_ZONE_Y;
		}
	}

	// ============================
	// マップ上下限制限
	// ============================
	const float MAP_TOP_Y = 0.0f;
	const float MAP_BOTTOM_Y = MAP_CHIP_Y_NUM * MAP_CHIP_HEIGHT;

	float cameraMinY = MAP_TOP_Y;
	float cameraMaxY = MAP_BOTTOM_Y - SCREEN_HEIGHT;

	// マップが画面より小さい場合の保険
	if (cameraMaxY < cameraMinY)
	{
		cameraMaxY = cameraMinY;
	}

	// Clamp
	if (currentY < cameraMinY)
	{
		currentY = cameraMinY;
	}
	else if (currentY > cameraMaxY)
	{
		currentY = cameraMaxY;
	}

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

// ============================
// カメラ取得
// ============================
CameraData GetCamera()
{
	return g_CameraData;
}