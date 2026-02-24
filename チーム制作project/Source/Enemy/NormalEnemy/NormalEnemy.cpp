#include "DxLib.h"
#include "NormalEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"

// アニメーション用パラメータ
struct NormalEnemyAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};

const NormalEnemyAnimationParam NORMAL_ENEMY_PARAM[NORMAL_ENEMY_ANIM_MAX] =
{
	10, 2, 50, 50,	// RUN
	8, 10, 50, 10,	// CRUSH
	8, 10, 50, 10,	// STRIKE
};

// 移動速度
#define NORMAL_ENEMY_MOVE_SPEED	(0.8f)

// マップ衝撃判定のサイズ補正
#define NORMAL_ENEMY_COLLISION_OFFSET	(0.1f)

// 死亡エフェクトインターバル
#define NORMAL_ENEMY_DEAD_EFFECT_INTARVAL	(5)

// 矩形判定サイズ
#define NORMAL_ENEMY_BOX_COLLISION_WIDTH	(38)
#define NORMAL_ENEMY_BOX_COLLISION_HEIGHT	(38)

// 撃破時のスコア
#define NORMAL_ENEMY_SCORE	(200)

NormalEnemyData g_NormalEnemyData[NORMAL_ENEMY_MAX] = { 0 };
NormalEnemyData g_PravNormalEnemyData[NORMAL_ENEMY_MAX] = { 0 };

void StartNormalEnemyAnimation(NormalEnemyAnimationType anim, int index);
void UpdateNormalEnemyAnimation(int index);

void InitNormalEnemy()
{
	NormalEnemyData* normalEnemy = g_NormalEnemyData;
	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normalEnemy++)
	{
		normalEnemy->pos.x = 0;
		normalEnemy->pos.y = 0;
		normalEnemy->move.x = 0;
		normalEnemy->move.y = 0;
		normalEnemy->active = false;
		normalEnemy->crush = false;
		normalEnemy->strike = false;
		normalEnemy->playAnim = NORMAL_ENEMY_ANIM_NONE;
		normalEnemy->boxCollision.width = NORMAL_ENEMY_BOX_COLLISION_WIDTH;
		normalEnemy->boxCollision.height = NORMAL_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < NORMAL_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&normalEnemy->animation[j]);
		}

		memset(&normalEnemy[i].boxCollision, 0, sizeof(normalEnemy[i].boxCollision));
	}
}

void LoadNormalEnemy()
{
	int runHandle = LoadGraph("Data/animation/Normal_Enemy/Normal_Enemy_run.png");
	int crushHandle = LoadGraph("Data/animation/Normal_Enemy/enemy_crush.png");
	int strikeHandle = LoadGraph("Data/animation/Normal_Enemy/enemy_strile.png");

	for (int i = 0; i < NORMAL_ENEMY_MAX; i++)
	{
		g_NormalEnemyData[i].animation[NORMAL_ENEMY_RUN].handle = runHandle;
		g_NormalEnemyData[i].animation[NORMAL_ENEMY_CRUSH].handle = crushHandle;
		g_NormalEnemyData[i].animation[NORMAL_ENEMY_STRIKE].handle = strikeHandle;
	}
}

void StepNormalEnemy()
{
	NormalEnemyData* normalEnemy = g_NormalEnemyData;
	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normalEnemy++)
	{
		if (!normalEnemy->active)continue;

		g_PravNormalEnemyData[i] = g_NormalEnemyData[i];

		normalEnemy->move.x = 0.0f;

		// 右に移動
		if (!normalEnemy->isTurn)
		{
			normalEnemy->move.x = NORMAL_ENEMY_MOVE_SPEED;
			normalEnemy->isTurn = false;
		}
		// 左に移動
		else if (normalEnemy->isTurn)
		{
			normalEnemy->move.x = -NORMAL_ENEMY_MOVE_SPEED;
			normalEnemy->isTurn = true;
		}
	}
}

void UpdateNormalEnemy()
{
	NormalEnemyData* normalEnemy = g_NormalEnemyData;
	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normalEnemy++)
	{
		if (!normalEnemy->active)continue;

		normalEnemy->pos.x += normalEnemy->move.x;
		normalEnemy->pos.y += normalEnemy->move.y;

		UpdateNormalEnemyAnimation(i);
	}
}

void DrawNormalEnemy()
{
	NormalEnemyData* normalEnemy = g_NormalEnemyData;
	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normalEnemy++)
	{
		if (!normalEnemy->active)continue;

		NormalEnemyAnimationType animType = normalEnemy->playAnim;
		AnimationData* animData = &normalEnemy->animation[animType];

		if (!normalEnemy->isTurn)
		{
			DrawAnimation(animData, normalEnemy->pos.x, normalEnemy->pos.y, FALSE, FALSE);
		}
	}
}

void FinNormalEnemy()
{

}
