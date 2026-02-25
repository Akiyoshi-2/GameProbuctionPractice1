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
	//赤
	8, 3, 50, 50, //攻撃
	30, 2, 50, 50, //待機
	10, 2, 50, 50, //移動
	8, 1, 50, 50, //ジャンプ
	8, 1, 50, 50, //落下
	8, 1, 50, 50, //死
	//青
	30, 2, 50, 50, //待機
	10, 2, 50, 50, //移動
	8, 1, 50, 50, //ジャンプ
	8, 1, 50, 50, //落下
	8, 1, 50, 50, //死
	//黄
	8, 2, 50, 50, //待機
	5, 2, 50, 50, //移動
	8, 1, 50, 50, //ジャンプ
	8, 1, 50, 50, //落下
};

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

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();							// アニメーション更新

//void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);

void InitPlayer()
{

	g_PlayerData.posX = 0.0f;
	g_PlayerData.posY = 0.0f;
	g_PlayerData.moveX = 0.0f;
	g_PlayerData.moveY = 0.0f;
	g_PlayerData.playerAnim = PLAYER_ANIM_NONE;

	g_PlayerData.type = TYPE_BLUE;

	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		InitAnimation(&g_PlayerData.animation[i]);
	}

}

void LoadPlayer()
{
	//赤
	g_PlayerData.animation[RED_PLAYER_ANIM_ATTACK].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Attack.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Idle.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Run.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/RedPlayer/赤player_jump1.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/RedPlayer/赤player_fall1.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_DIE].handle = LoadGraph("Data/animation/RedPlayer/赤player_die.png");

	//青
	g_PlayerData.animation[BLUE_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/BluePlayer/BluePlayer_Idle.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/BluePlayer/BluePlayer_Run.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/BluePlayer/青player_jump1.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/BluePlayer/青player_fall1.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_DIE].handle = LoadGraph("Data/animation/BluePlayer/青player_die.png");
	
	//黄
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/YellowPlayer/YellowPlayer_Idle.png");
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/YellowPlayer/YellowPlayer_Run.png");
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/YellowPlayer/黄player_fall1.png");
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/YellowPlayer/黄player_jump1.png");
	
}

void StartPlayer()
{
	StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
}

void UpdatePlayer()
{
	g_PlayerData.posX += g_PlayerData.moveX;
	g_PlayerData.posY += g_PlayerData.moveY;

	UpdatePlayerAnimation();
}

void StepPlayer()
{
	g_PlayerData.moveX = 0.0f;

	//g_PlayerData.posY += PLAYER_GRAVITY;

	if (IsTriggerKey(KEY_Q))
	{
		if (g_PlayerData.type == TYPE_BLUE)
		{
			g_PlayerData.type = TYPE_RED;
			StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
		}
		else
		{
			g_PlayerData.type = TYPE_BLUE;
			StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
		}
	}

	//左
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.moveX = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}
	//右
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.moveX = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}
	//ジャンプ
	else if (IsInputKey(KEY_SPACE))
	{
		g_PlayerData.moveY = -PLAYER_MOVE_JUMP;
	}
}

void DrawPlayer()
{
	CameraData camera = GetCamera();
	DrawFormatString(0, 100, GetColor(255, 255, 255), "プレイヤーの座標＝[%f, %f]", g_PlayerData.posX, g_PlayerData.posY);

	PlayerAnimationType animType = g_PlayerData.playerAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	DrawAnimation(animData, g_PlayerData.posX - camera.posX, g_PlayerData.posY - camera.posY, g_PlayerData.isTurn);
}

PlayerData* GetPlayer()
{
	return &g_PlayerData;
}
void StartPlayerAnimation(PlayerAnimationType anim)
{
	// 再生中のアニメーションであれば何もしない
	if (anim == g_PlayerData.playerAnim) return;

	// 再生中アニメーション設定
	g_PlayerData.playerAnim = anim;

	// 再生に必要なデータを取得
	AnimationData* animData = &g_PlayerData.animation[anim];
	PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

	// アニメーション再生
	StartAnimation(animData, g_PlayerData.posX, g_PlayerData.posY, animParam.interval, animParam.framNum, animParam.width, animParam.height, true);
}

void UpdatePlayerAnimation()
{
	// 地上にいるか
	if (!g_PlayerData.isAir)
	{
		if (g_PlayerData.moveX != 0.0f)
		{
			if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_RUN);
			else
				StartPlayerAnimation(RED_PLAYER_ANIM_RUN);
		}
		else
		{
			if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
			else
				StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
		}
	}
	// 空中にいる
	else
	{
		if (g_PlayerData.moveY < 0.0f)
		{
			if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_JUMP);
			else
				StartPlayerAnimation(RED_PLAYER_ANIM_JUMP);
		}
		else
		{
			if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_FALL);
			else
				StartPlayerAnimation(RED_PLAYER_ANIM_FALL);
		}
	}

	// アニメーション更新
	PlayerAnimationType animType = g_PlayerData.playerAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	UpdateAnimation(animData);
}

void FinPlayer()
{
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		DeleteGraph(g_PlayerData.animation[i].handle);
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
