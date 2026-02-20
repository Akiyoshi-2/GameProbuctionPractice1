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
PlayerData g_PrevPlayerData = { 0 };

#define PLAYER_MOVE_SPEED (4.0f)
#define PLAYER_MOVE_JUMP (12.0f)

#define PLAYER_GRAVITY (0.4f)
#define PLAYER_MAP_COLLSION_OFFSET (0.05f)
#define PLAYER_BOX_COLLISION_OFFSET_X (24)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)

#define PLAYER_BOX_COLLISION_WIDTH (20)
#define PLAYER_BOX_COLLISiON_HEIGHT (44)

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);

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
	g_PlayerData.boxCollision.posX = PLAYER_BOX_COLLISION_OFFSET_X;
	g_PlayerData.boxCollision.posY = PLAYER_BOX_COLLISION_OFFSET_Y;
	g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
	g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISiON_HEIGHT;
}

void UpdatePlayer()
{
	g_PlayerData.posX += g_PlayerData.moveX;
	g_PlayerData.posY += g_PlayerData.moveY;
	if (!g_PlayerData.active) return;

	if (g_PlayerData.moveY < 0.0f || g_PlayerData.moveY > PLAYER_GRAVITY)
	{
		g_PlayerData.isAir = true;
	}
}

void StepPlayer()
{

	if (!g_PlayerData.active)
	{
		return;
	}

	g_PrevPlayerData = g_PlayerData;

	
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
	if (!g_PlayerData.active) return;

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

void PlayerHitNormalBlockX(MapChipData mapChipData)
{
	PlayerData player = g_PlayerData;
	BlockData* block = mapChipData.data;

	const float POS_OFFSET = PLAYER_MAP_COLLSION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLSION_OFFSET * 2;

	player.isTurn = g_PrevPlayerData.isTurn;

	player.posX = g_PlayerData.posX;
	player.posY = g_PrevPlayerData.posY;

	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);
	
	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (player.moveX > 0.0f)
		{
			g_PlayerData.posX -= (x + w) - block->pos.x;
		}
		else if (player.moveX < 0.0f)
		{
			g_PrevPlayerData.posX += (block->pos.x + MAP_CHIP_WIDTH) - x;
		}
	}

	player.posX = g_PlayerData.posX;
	player.posY = g_PlayerData.posY;
}

void PlayerHitNormalBlockY(MapChipData mapChipData)
{
	PlayerData player = g_PlayerData;
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = PLAYER_MAP_COLLSION_OFFSET;
	const float SIZOFFSET = PLAYER_MAP_COLLSION_OFFSET * 2;

	player.isTurn = g_PrevPlayerData.isTurn;
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZOFFSET, h - SIZOFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		g_PrevPlayerData.moveY = 0.0f;

		if (player.moveY > 0.0f)
		{
			g_PlayerData.posY -= (y + h) - block->pos.y;
			g_PlayerData.isAir = false;
		}
		else if (player.moveY < 0.0f)
		{
			g_PrevPlayerData.posY += (block->pos.y + MAP_CHIP_WIDTH) - y;
		}
	}
}

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
	x = player.isTurn ?
		player.posX + -player.boxCollision.posX - player.boxCollision.width :
		player.posX + player.boxCollision.posX;
	y = player.posY + player.boxCollision.posY;
	w = player.boxCollision.width;
	h = player.boxCollision.height;
}
