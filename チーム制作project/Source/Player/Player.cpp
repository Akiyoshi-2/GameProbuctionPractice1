#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Scene/PlayScene/PlayScene.h"
#include "../Scene/SceneManager.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"

int g_PlayerHandle = -1;

PlayerData g_PlayerData = { 0 };

#define PLAYER_MOVE_SPEED (4.0f)
#define PLAYER_MOVE_JUMP (12.0f)

//#define PLAYER_GRAVITY (0.4f)

void InitPlayer()
{
	g_PlayerData.posX = 200.0f;
	g_PlayerData.posY = 200.0f;
	g_PlayerData.moveX = 0.0f;
	g_PlayerData.moveY = 0.0f;
}

void LoadPlayer()
{
	g_PlayerHandle = LoadGraph("Data/animation/BluePlayer/ê¬player_jump1.png");

	if (g_PlayerHandle == -1)
	{
		printfDx("Player LoadGraph FAILED\n");
	}
}

void StartPlayer()
{

}

void UpdatePlayer()
{
	g_PlayerData.posX += g_PlayerData.moveX;
	g_PlayerData.posY += g_PlayerData.moveY;
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

	if (IsTriggerKey(KEY_SPACE))
	{
		g_PlayerData.moveY = -PLAYER_MOVE_JUMP;
	}
}

void PlayerHitFullarmor_Enemy()
{
	g_PlayerData.hitFlag = true;
}

void PlayerHitHelmet_Enemy()
{
	g_PlayerData.hitFlag = true;

}

void PlayerHitNormal_Enemy()
{
	g_PlayerData.hitFlag = true;

}

void PlayerHitShield_Enemy()
{
	g_PlayerData.hitFlag = true;

}

void PlayerHitYellow_Enemy()
{
	g_PlayerData.hitFlag = true;

}


void DrawPlayer()
{
	if (g_PlayerHandle == -1) return;

	DrawGraph(
		(int)g_PlayerData.posX,
		(int)g_PlayerData.posY,
		g_PlayerHandle,
		TRUE
	);
}

PlayerData GetPlayer()
{
	return g_PlayerData;
}

void FinPlayer()
{
	DeleteGraph(g_PlayerHandle);
}