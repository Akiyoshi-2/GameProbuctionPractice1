#include "Dxlib.h"
#include "YellowEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Camera/Camera.h"

// アニメーション用パラメータ
struct YellowEnemyAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};

const YellowEnemyAnimationParam YELLOW_ENEMY_ANIM_PARAM[YELLOW_ENEMY_ANIM_MAX] =
{
	10, 2, 50, 50,	// RUN1
	10, 2, 50, 50,	// RUN2
	5, 6, 50, 50,	// STUN
	5, 10, 50, 50,	// CRUSH
	5, 10, 50, 50,	// STRIKE
};

// 移動速度
#define YELLOW_ENEMY_MOVE_SPEED	(1.2f)

// マップ衝撃判定のサイズ補正
#define YELLOW_ENEMY_COLLISION_OFFSET	(1.0f)

// 死亡エフェクトインターバル
#define YELLOW_ENEMY_DEAD_EFFECT_INTERVAL	(8)

// 重力
#define YELLOW_ENEMY_GRAVITY (0.35f)

// 矩形判定サイズ
#define YELLOW_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define YELLOW_ENEMY_BOX_COLLISION_HEIGHT	(47.0f)

// 撃破時のスコア
#define YELLOW_ENEMY_SCORE	(200)

YellowEnemyData g_YellowEnemyDate[YELLOW_ENEMY_MAX] = { 0 };
YellowEnemyData g_PravYellowEnemyDate[YELLOW_ENEMY_MAX] = { 0 };

void StartYellowEnemyAnimation(YellowEnemyAnimationType anim, int index);
void UpdateYellowEnemyAnimation(int index);

void InitYellowEnemy()
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		// 座標
		yellow->pos.x = 0;
		yellow->pos.y = 0;
		// 移動量
		yellow->move.x = 0;
		yellow->move.y = 0;
		// HP
		yellow->enemyHP = 0;
		// 状態
		yellow->active = false;
		yellow->stun = false;
		yellow->crush = false;
		yellow->strike = false;
		// アニメーション
		yellow->playAnim = YELLOW_ENEMY_ANIM_NONE;
		// 当たり判定サイズ
		yellow->boxCollision.width = YELLOW_ENEMY_BOX_COLLISION_WIDTH;
		yellow->boxCollision.height = YELLOW_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < YELLOW_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&yellow->animation[j]);
		}

		memset(&yellow[i].boxCollision, 0, sizeof(yellow[i].boxCollision));
	}
}

void LoadYellowEnemy()
{
	int runHandle1 = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy2_run.png");
	int runHandle2 = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_run.png");
	int crushHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_die.png");
	int strikeHandlde = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_die2.png");
	int stunHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_stun.png");

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++)
	{
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_RUN_1].handle = runHandle1;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_RUN_2].handle = runHandle2;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_CRUSH].handle = crushHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_STRIKE].handle = strikeHandlde;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_STUN].handle = stunHandle;
	}
}

void StepYellowEnemy()
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active)continue;

		g_PravYellowEnemyDate[i] = g_YellowEnemyDate[i];

		yellow->move.y += YELLOW_ENEMY_GRAVITY;

		yellow->move.x = 0.0f;

		// 右に移動
		if (!yellow->isTurn)
		{
			yellow->move.x = YELLOW_ENEMY_MOVE_SPEED;
		}
		// 左に移動
		else if (yellow->isTurn)
		{
			yellow->move.x = -YELLOW_ENEMY_MOVE_SPEED;
		}

		if (yellow->enemyHP == 1)
		{
			yellow->stun = true;
		}
	}
}

void UpdateYellowEnemy()
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active)continue;

		if (yellow->enemyHP < 3)
		{
			yellow->enemyHP = 2;
		}

		yellow->pos.x += yellow->move.x;
		yellow->pos.y += yellow->move.y;

		UpdateYellowEnemyAnimation(i);
	}
}

void DrawYellowEnemy()
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	CameraData cam = GetCamera();

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active)continue;

		YellowEnemyAnimationType animType = yellow->playAnim;
		AnimationData* animdata = &yellow->animation[animType];

		float drawX = yellow->pos.x - cam.posX;
		float drawY = yellow->pos.y - cam.posY;

		if (!yellow->isTurn)
		{
			DrawAnimation(animdata, drawX, drawY, TRUE, FALSE);
		}
		else if (yellow->isTurn)
		{
			DrawAnimation(animdata, drawX, drawY, FALSE, FALSE);
		}
	}
}

void FinYellowEnemy()
{
	for (int i = 0; i < YELLOW_ENEMY_ANIM_MAX; i++)
	{
		DeleteGraph(g_YellowEnemyDate[i].animation[i].handle);
	}
}

YellowEnemyData* GetYellowEnemy()
{
	return g_YellowEnemyDate;
}

void PlayerKillYellowEnemy(int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];

	yellow->active = false;

	// スコア
	// int score = GetScore() + YELLOW_ENEMY_SCORE;
	// SetScore(score);
}

void CreateYellowEnemy(float posX, float posY, const EnemyParameter* param)
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active)
		{
			yellow->active = true;

			yellow->animation->handle = g_YellowEnemyDate->animation->handle;

			yellow->pos.x = posX;
			yellow->pos.y = posY;

			yellow->move.x = YELLOW_ENEMY_MOVE_SPEED;
			yellow->move.y = 0.0f;

			yellow->enemyHP = 2;

			yellow->param = param;

			break;
		}
	}
}

void YellowEnemyHitBlockX(MapChipData mapChipData, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];
	BlockData* block = mapChipData.data;

	float prevX = g_PravYellowEnemyDate[index].pos.x;
	float prevY = g_PravYellowEnemyDate[index].pos.y;

	if (CheckSquareSquare(yellow->pos.x, yellow->pos.y,
		YELLOW_ENEMY_BOX_COLLISION_WIDTH + 10.0f,
		YELLOW_ENEMY_BOX_COLLISION_HEIGHT - YELLOW_ENEMY_COLLISION_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (prevX + YELLOW_ENEMY_BOX_COLLISION_WIDTH <= block->pos.x)
		{
			yellow->isTurn = true;
		}
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			yellow->isTurn = false;
		}
	}
}

void YellowEnemyHitBlockY(MapChipData mapChipData, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];
	YellowEnemyData* prevYellow = &g_PravYellowEnemyDate[index];
	BlockData* block = mapChipData.data;

	yellow->isTurn = g_YellowEnemyDate->isTurn;

	if (CheckSquareSquare(yellow->pos.x, yellow->pos.y,
		YELLOW_ENEMY_BOX_COLLISION_WIDTH, YELLOW_ENEMY_BOX_COLLISION_HEIGHT,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 上から当たった
		if (prevYellow->pos.y + YELLOW_ENEMY_BOX_COLLISION_HEIGHT <= block->pos.y)
		{
			yellow->pos.y = block->pos.y - YELLOW_ENEMY_BOX_COLLISION_HEIGHT;
			yellow->move.y = 0.0f;
			yellow->isAir = false;
		}
		// 下から当たったか
		else if (prevYellow->pos.y >= block->pos.y + MAP_CHIP_HEIGHT)
		{
			yellow->pos.y = block->pos.y + MAP_CHIP_HEIGHT;
			yellow->move.y = 0.0f;
		}
	}
}

void StartYellowEnemyAnimation(YellowEnemyAnimationType anim, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];

	// アニメーション再生中なら何もしない
	if (anim == yellow->playAnim)return;

	// 再生中アニメーション設定
	yellow->playAnim = anim;

	// データを取得
	AnimationData* animData = &yellow->animation[anim];
	YellowEnemyAnimationParam animParam = YELLOW_ENEMY_ANIM_PARAM[anim];

	// 再生
	StartAnimation(animData, yellow->pos.x, yellow->pos.y,
		animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdateYellowEnemyAnimation(int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];

	if (!yellow->stun)
	{
		// 通常歩きアニメーション
		StartYellowEnemyAnimation(YELLOW_ENEMY_RUN_1, index);
	}
	else if (yellow->stun)
	{
		// ヘルメットなし歩きアニメーション
		StartYellowEnemyAnimation(YELLOW_ENEMY_RUN_2, index);
	}

	// アニメーション更新
	YellowEnemyAnimationType animType = yellow->playAnim;
	AnimationData* animData = &yellow->animation[animType];
	UpdateAnimation(animData);
}