#include "DxLib.h"
#include "HelmetEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"

// アニメーション用パラメータ
struct helmetEnemyAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};

const helmetEnemyAnimationParam HELMET_ENEMYANIM_PARAM[HELMET_ENEMY_ANIM_MAX] =
{
	10, 2, 50, 50,	// RUN
	8, 10, 50, 50,	// DIE
};

// 移動速度
#define HELMET_ENEMY_MOVE_SPEED	(0.8f)

// マップ衝撃判定のサイズ補正
#define HELMET_ENEMY_COLLISION_OFFSET	(0.1f)

// 死亡エフェクトインターバル
#define HELMET_ENEMY_DEAD_EFFECT_INTERVAL	(5)

// 矩形判定サイズ
#define HELMET_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define HELMET_ENEMY_BOX_COLLISION_HEIGHT	(39.0f)

// 撃破時のスコア
#define HELMET_ENEMY_SCORE	(500)

HelmetEnemyData g_HelmetEnemyData[HELMET_ENEMY_MAX] = { 0 };
HelmetEnemyData g_PravHelmetEnemyData[HELMET_ENEMY_MAX] = { 0 };

void StartHelmetEnemyAnimation(HelmetAnimationType anim, int index);
void UpdateHelmetEnemyAnimation(int index);

void InitHelmetEnemy()
{
	HelmetEnemyData* helmet = g_HelmetEnemyData;
	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		helmet->pos.x = 0;
		helmet->pos.y = 0;
		helmet->move.x = 0;
		helmet->move.y = 0;
		helmet->active = false;
		helmet->playAnim = HELMET_ENEMY_ANIM_NONE;
		helmet->boxCollision.width = HELMET_ENEMY_BOX_COLLISION_WIDTH;
		helmet->boxCollision.height = HELMET_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < HELMET_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&helmet->animation[j]);

			memset(&helmet[i].boxCollision, 0, sizeof(helmet[i].boxCollision));
		}
	}
}

void LoadHelmetEnemy()
{
	int runHandlde = LoadGraph("Data/animation/Helmet_Enemy/helmet_enemy_run.png");
	int dieHandle = LoadGraph("Data/animation/Helmet_Enemy/helmet_enemy_die.png");

	for (int i = 0; i < HELMET_ENEMY_MAX; i++)
	{
		g_HelmetEnemyData[i].animation[HELMET_ENEMY_ANIM_RUN].handle = runHandlde;
	}
}