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
		g_HelmetEnemyData[i].animation[HELMET_ENEMY_ANIM_DIE].handle = dieHandle;
	}
}

void StepHelmetEnemy()
{
	HelmetEnemyData* helmet = g_HelmetEnemyData;
	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active)continue;

		g_PravHelmetEnemyData[i] = g_HelmetEnemyData[i];

		helmet->move.x = 0.0f;

		// 右に移動
		if (!helmet->isTurn)
		{
			helmet->move.x = HELMET_ENEMY_MOVE_SPEED;
		}
		// 左に移動
		else if (helmet->isTurn)
		{
			helmet->move.x = -HELMET_ENEMY_MOVE_SPEED;
		}
	}
}

void UpdateHelmetEnemy()
{
	HelmetEnemyData* helmet = g_HelmetEnemyData;
	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active)continue;

		helmet->pos.x += helmet->move.x;
		helmet->pos.y += helmet->move.y;

		UpdateHelmetEnemyAnimation(i);
	}
}

void DrawHelmetEnemy()
{
	HelmetEnemyData* helmet = g_HelmetEnemyData;
	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active)continue;

		HelmetAnimationType animType = helmet->playAnim;
		AnimationData* animData = &helmet->animation[animType];

		if (!helmet->isTurn)
		{
			DrawAnimation(animData, helmet->pos.x, helmet->pos.y, FALSE, FALSE);
		}
	}
}

void FinHelmetEnemy()
{
	for (int i = 0; i < HELMET_ENEMY_ANIM_MAX; i++)
	{
		DeleteGraph(g_HelmetEnemyData->animation[i].handle);
	}
}

HelmetEnemyData* GetHelmetEnemy()
{
	return g_HelmetEnemyData;
}

void PlayerKillHelmetEnemy(int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];

	helmet->active = false;

	// スコア
	// int score = GetScore() + HELMET_ENEMY_SCORE;
	// SetScore(score)
}

void CreateHelmetEnemy(float posX, float posY, const EnemyParameter* param)
{
	HelmetEnemyData* helmet = g_HelmetEnemyData;
	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active)
		{
			// アクティブ
			helmet->active = true;

			// アニメーション
			helmet->animation->handle = g_HelmetEnemyData->animation->handle;

			// 座標
			helmet->pos.x = posX;
			helmet->pos.y = posY;

			// 移動量
			helmet->move.x = HELMET_ENEMY_MOVE_SPEED;
			helmet->move.y = 0.0f;

			// パラメータ設定
			helmet->param = param;

			break;
		}
	}
}

void HelmetEnemyHitBlockX(MapChipData mapChipData, int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = HELMET_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = HELMET_ENEMY_COLLISION_OFFSET * 2;

	helmet->pos.x = g_HelmetEnemyData[index].pos.x;
	helmet->pos.y = g_HelmetEnemyData[index].pos.y;

	if (CheckSquareSquare(helmet->pos.x + POS_OFFSET, helmet->pos.y - POS_OFFSET,
		HELMET_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, HELMET_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (helmet->move.x > 0.0f)
		{
			helmet->isTurn = false;
		}
		// 右から当たった
		else if (helmet->move.x < 0.0f)
		{
			helmet->isTurn = true;
		}

	}
}

void HelmetEnemyHitBlockY(MapChipData mapChipData, int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = HELMET_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = HELMET_ENEMY_COLLISION_OFFSET * 2;

	helmet->isTurn = g_HelmetEnemyData->isTurn;

	if (CheckSquareSquare(helmet->pos.x + POS_OFFSET, helmet->pos.y - POS_OFFSET,
		HELMET_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, HELMET_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 上から当たった
		if (helmet->move.y > 0.0f)
		{
			// 上に押し出す
			helmet->pos.y -= (helmet->pos.y * HELMET_ENEMY_BOX_COLLISION_HEIGHT) - block->pos.y;
			helmet->isAir = false;
		}
		// 下から当たったか
		else if (helmet->move.y < 0.0f)
		{
			// 下に押し出す
			helmet->pos.y += (block->pos.y + MAP_CHIP_WIDTH) - helmet->pos.y;
		}

		// Yの移動量を0にする
		helmet->move.y = 0.0f;
	}
}

void StartHelmetEnemyAnimation(HelmetAnimationType anim, int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];

	// アニメーション再生中なら何もしない
	if (anim == helmet->playAnim)return;

	// 再生中アニメーション設定
	helmet->playAnim = anim;

	// データを取得
	AnimationData* animData = &helmet->animation[anim];
	helmetEnemyAnimationParam animParam = HELMET_ENEMYANIM_PARAM[anim];

	//再生
	StartAnimation(animData, helmet->pos.x, helmet->pos.y,
		animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdateHelmetEnemyAnimation(int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];

	// 歩くアニメーション
	StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_RUN, index);

	// 増えれば追加する
	// StartHelmetEnemyAnimation(???, index);



	// アニメーション更新
	HelmetAnimationType animType = helmet->playAnim;
	AnimationData* animData = &helmet->animation[animType];
	UpdateAnimation(animData);
}