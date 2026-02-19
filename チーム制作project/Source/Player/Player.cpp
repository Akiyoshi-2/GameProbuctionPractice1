#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Scene/PlayScene/PlayScene.h"
#include "../Scene/SceneManager.h"
#include "../Camera/Camera.h"


int g_PlayerHandle = -1;

PlayerData g_PlayerData = { 0 };

#define PLAYER_MOVE_SPEED (4.0f)
#define PLAYER_MOVE_JUMP (12.0f)

void InitPlayer()
{

}

void LoadPlayer()
{
	g_PlayerHandle = LoadGraph("Data/animation/ê¬player_jump1.png");
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
	//if (!g_PlayerData.active) return;

	//CameraData camera = GetCamera();

	//PlayerAnimationType = g_PlayerData.playAnime;
	//AnimationData* animData = &g_PlayerData.animation[animType];
	//DrawAnimation(animData, g_PlayerData.posX - camera.posX, g_PlayerData.posY - camera.posY, g_PlayerData.isTurn);
}

void FinPlayer()
{
	DeleteGraph(g_PlayerHandle);
}