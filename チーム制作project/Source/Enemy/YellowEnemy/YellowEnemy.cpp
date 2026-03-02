#include "Dxlib.h"
#include "YellowEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"

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
	8, 6, 50, 50,	// STUN
	8, 10, 50, 50,	// CRUSH
	8, 10, 50, 50,	// STRIKE
};

// 移動速度
#define YELLOW_ENEMY_MOVE_SPEED	(1.2f)

// マップ衝撃判定のサイズ補正
#define YELLOW_ENEMY_COLLISION_OFFSET	(0.1f)

// 死亡エフェクトインターバル
#define YELLOW_ENEMY_DEAD_EFFECT_INTERVAL	(8)

// 重力
#define YELLOW_ENEMY_GRAVITY (0.35f)

// 矩形判定サイズ
#define YELLOW_ENEMY_BOX_COLLISION_WIDTH	(38.8f)
#define YELLOW_ENEMY_BOX_COLLISION_HEIGHT	(38.8f)

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
		yellow->pos.x = 0;
		yellow->pos.y = 0;
		yellow->move.x = 0;
		yellow->move.y = 0;
		yellow->enemyHP = 0;
		yellow->active = false;
		yellow->stun = false;
		yellow->crush = false;
		yellow->strike = false;
		yellow->playAnim = YELLOW_ENEMY_ANIM_NONE;
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
	int runHandle1 = LoadGraph("Data/animation/Normal_Enemy/yellow_enemy2_run.png");
	int runHandle2 = LoadGraph("Data/animation/Normal_Enemy/yellow_enemy_run.png");
	int crushHandle = LoadGraph("Data/animation/Normal_Enemy/yellow_enemy_die.png");
	int strikeHandlde = LoadGraph("Data/animation/Normal_Enemy/yellow_enemy_die2.png");
	int stunHandle = LoadGraph("Data/animation/Normal_Enemy/yellow_enemy_stun.png");

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

		yellow->pos.x += yellow->move.x;
		yellow->pos.y += YELLOW_ENEMY_GRAVITY;

		if (yellow->enemyHP < 3)
		{
			yellow->enemyHP = 2;
		}

		UpdateYellowEnemyAnimation(i);
	}
}

void DrawYellowEnemy()
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active)continue;

		YellowEnemyAnimationType animType = yellow->playAnim;
		AnimationData* animdata = &yellow->animation[animType];

		if (!yellow->isTurn)
		{
			DrawAnimation(animdata, yellow->pos.x, yellow->pos.y, TRUE, FALSE);
		}
		else if (yellow->isTurn)
		{
			DrawAnimation(animdata, yellow->pos.x, yellow->pos.y, FALSE, FALSE);
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
	const float POS_OFFSET = YELLOW_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = YELLOW_ENEMY_COLLISION_OFFSET * 2;

	yellow->pos.x = g_YellowEnemyDate[index].pos.x;
	yellow->pos.y = g_YellowEnemyDate[index].pos.y;

	if (CheckSquareSquare(yellow->pos.x + POS_OFFSET, yellow->pos.y + POS_OFFSET,
		YELLOW_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, YELLOW_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))

	{
		// 右から当たった
		if (yellow->move.x > 0.0f)
		{
			yellow->isTurn = false;
		}
		// 左から当たった
		else if (yellow->move.x < 0.0f)
		{
			yellow->isTurn = true;
		}
	}
}

void YellowEnemyHitBlockY(MapChipData mapChipData, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = YELLOW_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = YELLOW_ENEMY_COLLISION_OFFSET * 2;

	yellow->isTurn = g_YellowEnemyDate->isTurn;

	if (CheckSquareSquare(yellow->pos.x + POS_OFFSET, yellow->pos.y + POS_OFFSET,
		YELLOW_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, YELLOW_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 上から当たった
		if (yellow->move.y > 0.0f)
		{
			yellow->pos.y -= (yellow->pos.y * YELLOW_ENEMY_BOX_COLLISION_HEIGHT) - block->pos.y;
			yellow->isAir = false;
		}
		// 下から当たったか
		else if (yellow->move.y < 0.0f)
		{
			yellow->pos.y += (block->pos.y + MAP_CHIP_WIDTH) - yellow->pos.y;
		}

		// Yの移動量を0にする
		yellow->move.y = 0.0f;
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

		// アニメーション更新
		YellowEnemyAnimationType animType = yellow->playAnim;
		AnimationData* animData = &yellow->animation[animType];
		UpdateAnimation(animData);
	}
}