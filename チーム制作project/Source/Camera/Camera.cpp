#include "DxLib.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Player/Player.h"
#include "../Scene/PlayScene/PlayScene.h"
#include "../Scene/TutorialScene/TutorialScene.h"
#include "../Scene/SceneManager.h"
#include "../GameSetting/GameSetting.h"

#define DEBUG_CAMERA_SPEED (4.0f)
#define CAMERA_SCROLL_START_X (200.0f)

CameraData g_CameraData = { 0 };

void InitCamera()
{
}

void StepCamera()
{
	PlayerData player = GetPlayer();

	if (player.posX >= CAMERA_SCROLL_START_X)
	{
		g_CameraData.posX = player.posX - CAMERA_SCROLL_START_X;
	}
	else
	{
		g_CameraData.posX = 0.0f;
	}
}

void DrawCamera()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "ÉJÉÅÉâÇÃç¿ïW=[%f, %f]", g_CameraData.posX, g_CameraData.posY);
}

CameraData GetCamera()
{
	return g_CameraData;
}
