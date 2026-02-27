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

const NormalEnemyAnimationParam NORMAL_ENEMY_ANIM_PARAM[NORMAL_ENEMY_ANIM_MAX] =
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
#define NORMAL_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define NORMAL_ENEMY_BOX_COLLISION_HEIGHT	(38.0f)

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
		}
		// 左に移動
		else if (normalEnemy->isTurn)
		{
			normalEnemy->move.x = -NORMAL_ENEMY_MOVE_SPEED;
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
			DrawAnimation(animData, normalEnemy->pos.x, normalEnemy->pos.y, TRUE, FALSE);
		}
		else if (normalEnemy->isTurn)
		{
			DrawAnimation(animData, normalEnemy->pos.x, normalEnemy->pos.y, FALSE, FALSE);
		}
	}
}

void FinNormalEnemy()
{
	for (int i = 0; i < NORMAL_ENEMY_ANIM_MAX; i++)
	{
		DeleteGraph(g_NormalEnemyData->animation[i].handle);
	}
}

NormalEnemyData* GetNormalEnemy()
{
	return g_NormalEnemyData;
}

void PlayerKillNormalEnemy(int index)
{
	NormalEnemyData* NormalEnemy = &g_NormalEnemyData[index];

	NormalEnemy->active = false;

	// スコア
	// int score = GetScore() + NORMAL_ENEMY_SCORE;
	// SetScore(score);

}

void CreateNomalEnemy(float posX, float posY, const EnemyParameter* param)
{
	NormalEnemyData* normalEnemy = g_NormalEnemyData;
	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normalEnemy++)
	{
		if (!normalEnemy->active)
		{
			// アクティブ
			normalEnemy->active = true;

			// アニメーション
			normalEnemy->animation->handle = g_NormalEnemyData->animation->handle;

			// 座標
			normalEnemy->pos.x = posX;
			normalEnemy->pos.y = posY;

			// 移動量
			normalEnemy->move.x = NORMAL_ENEMY_MOVE_SPEED;
			normalEnemy->move.y = 0.0f;

			// パラメータ設定
			normalEnemy->param = param;

			break;
		}
	}
}

void NormalEnemyHitBlockX(MapChipData mapChipData, int index)
{
	NormalEnemyData* normalEnemy = &g_NormalEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = NORMAL_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = NORMAL_ENEMY_COLLISION_OFFSET * 2;

	normalEnemy->pos.x = g_NormalEnemyData[index].pos.x;
	normalEnemy->pos.y = g_NormalEnemyData[index].pos.y;

	if (CheckSquareSquare(normalEnemy->pos.x + POS_OFFSET, normalEnemy->pos.y - POS_OFFSET,
		NORMAL_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, NORMAL_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (normalEnemy->move.x > 0.0f)
		{
			normalEnemy->isTurn = false;
		}
		// 右から当たった
		else if (normalEnemy->move.x < 0.0f)
		{
			normalEnemy->isTurn = true;
		}

	}
}

void NormalEnemyHitBlockY(MapChipData mapChipData, int index)
{
	NormalEnemyData* normalEnemy = &g_NormalEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = NORMAL_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = NORMAL_ENEMY_COLLISION_OFFSET * 2;

	normalEnemy->isTurn = g_PravNormalEnemyData->isTurn;

	if (CheckSquareSquare(normalEnemy->pos.x + POS_OFFSET, normalEnemy->pos.y - POS_OFFSET,
		NORMAL_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, NORMAL_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 上から当たった
		if (normalEnemy->move.y > 0.0f)
		{
			// 上に押し出す
			normalEnemy->pos.y -= (normalEnemy->pos.y * NORMAL_ENEMY_BOX_COLLISION_HEIGHT) - block->pos.y;
			normalEnemy->isAir = false;
		}
		// 下から当たったか
		else if (normalEnemy->move.y < 0.0f)
		{
			// 下に押し出す
			normalEnemy->pos.y += (block->pos.y + MAP_CHIP_WIDTH) - normalEnemy->pos.y;
		}

		// Yの移動量を0にする
		normalEnemy->move.y = 0.0f;
	}
}

void StartNormalEnemyAnimation(NormalEnemyAnimationType anim, int index)
{
	NormalEnemyData* normalEnemy = &g_NormalEnemyData[index];

	// アニメーション再生中なら何もしない
	if (anim == normalEnemy->playAnim)return;

	// 再生中アニメーション設定
	normalEnemy->playAnim = anim;

	// データを取得
	AnimationData* animData = &normalEnemy->animation[anim];
	NormalEnemyAnimationParam animParam = NORMAL_ENEMY_ANIM_PARAM[anim];

	//再生
	StartAnimation(animData, normalEnemy->pos.x, normalEnemy->pos.y,
		animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdateNormalEnemyAnimation(int index)
{
	NormalEnemyData* normalEnemy = &g_NormalEnemyData[index];

	// 歩くアニメーション
	StartNormalEnemyAnimation(NORMAL_ENEMY_RUN, index);
	
	// 増えれば追加する
	// StartNormalEnemyAnimation(???, index);



	// アニメーション更新
	NormalEnemyAnimationType animType = normalEnemy->playAnim;
	AnimationData* animData = &normalEnemy->animation[animType];
	UpdateAnimation(animData);
}