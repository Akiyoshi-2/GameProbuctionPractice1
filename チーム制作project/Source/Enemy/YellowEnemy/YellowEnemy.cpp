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


	}
}

void UpdateYellowEnemy()
{

}

void DrawYellowEnemy()
{

}

void FinYellowEnemy()
{

}