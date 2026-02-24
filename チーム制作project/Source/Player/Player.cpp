#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Scene/PlayScene/PlayScene.h"
#include "../Scene/SceneManager.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"

// アニメーション用パラメータ
struct PlayerAnimationParam
{
	int interval;
	int framNum;
	int width;
	int height;
};
const  PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
	8, 3, 50, 50,
	8, 2, 50, 50,
	5, 2, 50, 50,
	8, 1, 50, 50,
	8, 1, 50, 50,
	8, 1, 50, 50,
	8, 2, 50, 50,
	5, 2, 50, 50,
	8, 1, 50, 50,
	8, 1, 50, 50,
	8, 1, 50, 50,
	8, 2, 50, 50,
	5, 2, 50, 50,
	8, 1, 50, 50,
	8, 1, 50, 50,
};

PlayerData g_PlayerData[PLAYER_ANIM_MAX] = {};

PlayerData g_PrevPlayerData = { 0 };

#define PLAYER_MOVE_SPEED (4.0f)
#define PLAYER_MOVE_JUMP (12.0f)

#define PLAYER_GRAVITY (0.4f)
#define PLAYER_MAP_COLLSION_OFFSET (0.05f)
#define PLAYER_BOX_COLLISION_OFFSET_X (24)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)

#define PLAYER_BOX_COLLISION_WIDTH (20)
#define PLAYER_BOX_COLLISiON_HEIGHT (44)

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();							// アニメーション更新
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);

//void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);

void InitPlayer()
{

	g_PlayerData->posX = 0.0f;
	g_PlayerData->posY = 0.0f;
	g_PlayerData->moveX = 0.0f;
	g_PlayerData->moveY = 0.0f;

	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		InitAnimation(&g_PrevPlayerData.animation[i]);
	}

}

void LoadPlayer()
{
	//赤
	g_PrevPlayerData.animation[RRED_PLAYER_ANIM_ATTACK].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Attack.png");
	g_PrevPlayerData.animation[RED_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Idle.png");
	g_PrevPlayerData.animation[RED_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Run.png");
	g_PrevPlayerData.animation[RED_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/RedPlayer/赤player_jump1.png");
	g_PrevPlayerData.animation[RED_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/RedPlayer/赤player_fall1.png");
	g_PrevPlayerData.animation[RED_PLAYER_ANIM_DIE].handle = LoadGraph("Data/animation/RedPlayer/赤player_die.png");

	//青
	g_PrevPlayerData.animation[BLUE_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/RedPlayer/BluePlayer/BluePlayer_Idle.png");
	g_PrevPlayerData.animation[BLUE_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/RedPlayer/BluePlayer/BluePlayer_Run.png");
	g_PrevPlayerData.animation[BLUE_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/RedPlayer/BluePlayer/青player_jump1.png");
	g_PrevPlayerData.animation[BLUE_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/RedPlayer/BluePlayer/青player_fall1.png");
	g_PrevPlayerData.animation[BLUE_PLAYER_ANIM_DIE].handle = LoadGraph("Data/animation/RedPlayer/BluePlayer/青player_die.png");
	
	//黄
	g_PrevPlayerData.animation[YELLOW_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/RedPlayer/");
	g_PrevPlayerData.animation[YELLOW_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/RedPlayer/");
	g_PrevPlayerData.animation[YELLOW_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/RedPlayer/");
	g_PrevPlayerData.animation[YELLOW_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/RedPlayer/");
	
}

void StartPlayer()
{
	
}

void UpdatePlayer()
{

}

void StepPlayer()
{
	g_PlayerData->moveX = 0.0f;

	g_PlayerData->posY += PLAYER_GRAVITY;

	//左
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData->moveX = -PLAYER_MOVE_SPEED;
	}
	//右
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData->moveX = PLAYER_MOVE_SPEED;
	}
	//ジャンプ
	else if (IsInputKey(KEY_SPACE))
	{
		g_PlayerData->moveY = -PLAYER_MOVE_JUMP;
	}
}

void DrawPlayer()
{
	CameraData camera = GetCamera();
}

PlayerData* GetPlayer()
{
	return g_PlayerData;
}

void FinPlayer()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		DeleteGraph(g_PlayerData[i].handle);
	}
}

//void PlayerHitNormalBlockX(MapChipData mapChipData)
//{
//	/*PlayerData player = g_PlayerData;
//	BlockData* block = mapChipData.data;
//
//	const float POS_OFFSET = PLAYER_MAP_COLLSION_OFFSET;
//	const float SIZE_OFFSET = PLAYER_MAP_COLLSION_OFFSET * 2;
//
//	player.isTurn = g_PrevPlayerData.isTurn;
//
//	player.posX = g_PlayerData.posX;
//	player.posY = g_PrevPlayerData.posY;
//
//	float x, y, w, h;
//	CalcBoxCollision(player, x, y, w, h);
//	
//	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
//		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
//	{
//		if (player.moveX > 0.0f)
//		{
//			g_PlayerData.posX -= (x + w) - block->pos.x;
//		}
//		else if (player.moveX < 0.0f)
//		{
//			g_PrevPlayerData.posX += (block->pos.x + MAP_CHIP_WIDTH) - x;
//		}
//	}
//
//	player.posX = g_PlayerData.posX;
//	player.posY = g_PlayerData.posY;*/
//}
//
//void PlayerHitNormalBlockY(MapChipData mapChipData)
//{
//	/*PlayerData player = g_PlayerData;
//	BlockData* block = mapChipData.data;
//	const float POS_OFFSET = PLAYER_MAP_COLLSION_OFFSET;
//	const float SIZOFFSET = PLAYER_MAP_COLLSION_OFFSET * 2;
//
//	player.isTurn = g_PrevPlayerData.isTurn;
//	float x, y, w, h;
//	CalcBoxCollision(player, x, y, w, h);
//
//	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZOFFSET, h - SIZOFFSET,
//		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
//	{
//		g_PrevPlayerData.moveY = 0.0f;
//
//		if (player.moveY > 0.0f)
//		{
//			g_PlayerData.posY -= (y + h) - block->pos.y;
//			g_PlayerData.isAir = false;
//		}
//		else if (player.moveY < 0.0f)
//		{
//			g_PrevPlayerData.posY += (block->pos.y + MAP_CHIP_WIDTH) - y;
//		}
//	}*/
//}
//
//void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
//{
//	/*x = player.isTurn ?
//		player.posX + -player.boxCollision.posX - player.boxCollision.width :
//		player.posX + player.boxCollision.posX;
//	y = player.posY + player.boxCollision.posY;
//	w = player.boxCollision.width;
//	h = player.boxCollision.height;*/
//}
