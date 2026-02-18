#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Scene/PlayScene/PlayScene.h"
#include "../Scene/SceneManager.h"

int g_PlayerHandle = -1;

PlayerData g_PlayerData = { 0 };

#define PLAYER_MOVE_SPEED (4.0f)
#define PLAYER_MOVE_JUMP (12.0f)

void InitPlayer()
{

}

void LoadPlayer()
{
	g_PlayerHandle = LoadGraph("Data/Player/");
}

void StartPlayer()
{

}

void UpdatePlayer()
{
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.posX -= g_PlayerData.moveX;
	}
	if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.moveX += g_PlayerData.moveX;
	}

	if (IsTriggerKey(KEY_SPACE))
	{
		g_PlayerData.moveY -= PLAYER_MOVE_JUMP;
	}
}

void StepPlayer()
{
	g_PlayerData.moveX = 0.0f;
	g_PlayerData.moveY = 0.0f;

	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.moveX = -PLAYER_MOVE_SPEED;
	}

	if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.moveX = PLAYER_MOVE_SPEED;
	}
}


void DrawPlayer()
{
	DrawGraph(0, 0, g_PlayerHandle, TRUE);
}

void FinPlayer()
{
	DeleteGraph(g_PlayerHandle);
}