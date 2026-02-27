#include "DxLib.h"
#include "ShieldEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"

// アニメーション用パラメータ
struct ShieldEnemyAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};

const ShieldEnemyAnimationParam SHIELD_ENEMY_ANIM_PARAM[SHIELD_ENEMY_ANIM_MAX] =
{
	10, 2, 50, 50,	// RUN
	8, 10, 50, 50,	// DIE
};

//移動速度
#define SHIELD_ENEMY_MOVE_SPEED	(0.8f)

// マップ衝撃判定のサイズ補正
#define SHIELD_ENEMY_COLLISION_OFFSET	(0.1f)

// 死亡エフェクトインターバル
#define SHIELD_ENEMY_DEAD_EFFECT_INTARVAL	(5)

// 矩形判定サイズ
#define SHIELD_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define SHIELD_ENEMY_BOX_COLLISION_HEIGHT	(38.0f)

// 撃破時のスコア
#define SHIELD_ENEMY_SCORE	(500)

ShieldEnemyData g_ShieldEnemyData[SHIELD_ENEMY_MAX] = { 0 };
ShieldEnemyData g_PravShieldEnemyData[SHIELD_ENEMY_MAX] = { 0 };

void StartShieldEnemyAnimation(ShieldEnemyAnimationType anim, int index);
void UpdateShieldEnemyAnimation(int index);

void InitShieldEnemy()
{
	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;
	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		shieldEnemy->pos.x = 0;
		shieldEnemy->pos.y = 0;
		shieldEnemy->move.x = 0;
		shieldEnemy->move.y = 0;
		shieldEnemy->active = false;
		shieldEnemy->playAnim = SHIELD_ENEMY_ANIM_NONE;
		shieldEnemy->boxCollision.width = SHIELD_ENEMY_BOX_COLLISION_WIDTH;
		shieldEnemy->boxCollision.height = SHIELD_ENEMY_BOX_COLLISION_HEIGHT;
		for (int j = 0; j < SHIELD_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&shieldEnemy->animation[j]);
		}

		memset(&shieldEnemy[i].boxCollision, 0, sizeof(shieldEnemy[i].boxCollision));
	}
}


	void LoadShieldEnemy()
	{
		int runHandle = LoadGraph("Data/animation/Shield_Enemy/shield_enemy_run.png");
		int dieHandle = LoadGraph("Data/animation/Shield_Enemy/shield_enemy_die.png");

		for (int i = 0; i < SHIELD_ENEMY_MAX; i++)
		{
			g_ShieldEnemyData[i].animation[SHIELD_ENEMY_RUN].handle = runHandle;
			g_ShieldEnemyData[i].animation[SHIELD_ENEMY_DIE].handle = dieHandle;
		}
	}


void StepShieldEnemy()
{
	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;
	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)continue;

		g_PravShieldEnemyData[i] = g_ShieldEnemyData[i];

		shieldEnemy->move.x = 0.0f;

		if (!shieldEnemy->isTurn)
		{
			shieldEnemy->move.x = SHIELD_ENEMY_MOVE_SPEED;
		}
		else if (shieldEnemy->isTurn)
		{
			shieldEnemy->move.x = -SHIELD_ENEMY_MOVE_SPEED;
		}
	}
}


void UpdateShieldEnemy()
{
	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;
	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)continue;

		shieldEnemy->pos.x += shieldEnemy->move.x;
		shieldEnemy->pos.y += shieldEnemy->move.y;

		UpdateShieldEnemyAnimation(i);
	}

}


void DrawShieldEnemy()
{
	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;
	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)continue;

		ShieldEnemyAnimationType animType = shieldEnemy->playAnim;
		AnimationData* animData = &shieldEnemy->animation[animType];

		if (!shieldEnemy->isTurn)
		{
			DrawAnimation(animData, shieldEnemy->pos.x, shieldEnemy->pos.y, TRUE, FALSE);
		}
		else if (shieldEnemy->isTurn)
		{
			DrawAnimation(animData, shieldEnemy->pos.x, shieldEnemy->pos.y, FALSE, FALSE);
		}
	}
}


void FinShieldEnemy()
{
	for (int i = 0; i < SHIELD_ENEMY_ANIM_MAX; i++)
	{
		DeleteGraph(g_ShieldEnemyData->animation[i].handle);
	}
}

ShieldEnemyData* GetShieldEnemy()
{
	return g_ShieldEnemyData;
}

void PlayerKillShieldEnemy(int index)
{
	ShieldEnemyData* ShieldEnemy = &g_ShieldEnemyData[index];

	ShieldEnemy->active = false;
}

void CreateShieldEnemy(float posX, float posY, const EnemyParameter* param)
{
	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;
	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)
		{
			shieldEnemy->active = true;

			shieldEnemy->animation->handle = g_ShieldEnemyData->animation->handle;

			shieldEnemy->pos.x = posX;
			shieldEnemy->pos.y = posY;

			shieldEnemy->move.x = SHIELD_ENEMY_MOVE_SPEED;
			shieldEnemy->move.y = 0.0f;

			shieldEnemy->param = param;

			break;
		}
	}
}

void ShieldEnemyHitBlockX(MapChipData mapChipData, int index)
{
	ShieldEnemyData* shieldEnemy =  &g_ShieldEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = SHIELD_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = SHIELD_ENEMY_COLLISION_OFFSET * 2;

	shieldEnemy->pos.x = g_ShieldEnemyData[index].pos.x;
	shieldEnemy->pos.y = g_ShieldEnemyData[index].pos.y;

	if (CheckSquareSquare(shieldEnemy->pos.x + POS_OFFSET, shieldEnemy->pos.y - POS_OFFSET,
		SHIELD_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, SHIELD_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (shieldEnemy->move.x > 0.0f)
		{
			shieldEnemy->isTurn = false;
		}
		else if (shieldEnemy->move.x < 0.0f)
		{
			shieldEnemy->isTurn = true;
		}
	}
		
}

void ShieldEnemyHitBlockY(MapChipData mapChipData, int index)
{
	ShieldEnemyData* shieldEnemy = &g_ShieldEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = SHIELD_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = SHIELD_ENEMY_COLLISION_OFFSET * 2;

	shieldEnemy->isTurn = g_PravShieldEnemyData->isTurn;

	if (CheckSquareSquare(shieldEnemy->pos.x + POS_OFFSET, shieldEnemy->pos.y - POS_OFFSET,
		SHIELD_ENEMY_BOX_COLLISION_WIDTH - SIZE_OFFSET, SHIELD_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (shieldEnemy->move.y > 0.0f)
		{
			// 上に押し出す
			shieldEnemy->pos.y -= (shieldEnemy->pos.y * SHIELD_ENEMY_BOX_COLLISION_HEIGHT) - block->pos.y;
			shieldEnemy->isAir = false;
		}
		else if (shieldEnemy->move.y < 0.0f)
		{
			// 下に押し出す
			shieldEnemy->pos.y += (block->pos.y + MAP_CHIP_WIDTH) - shieldEnemy->pos.y;

		}

		shieldEnemy->move.y = 0.0f;
	}
	
}

void StartShieldEnemyAnimation(ShieldEnemyAnimationType anim, int index)
{
	ShieldEnemyData* shieldEnemy = &g_ShieldEnemyData[index];

	// アニメーション再生中なら何もしない
	if (anim == shieldEnemy->playAnim)return;

	// 再生中アニメーション設定
	shieldEnemy->playAnim = anim;

	// データを取得
	AnimationData* animData = &shieldEnemy->animation[anim];
	ShieldEnemyAnimationParam animParam = SHIELD_ENEMY_ANIM_PARAM[anim];

	//再生
	StartAnimation(animData, shieldEnemy->pos.x, shieldEnemy->pos.y,
		animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdateShieldEnemyAnimation(int index)
{
	ShieldEnemyData* shieldEnemy = &g_ShieldEnemyData[index];

	// 歩くアニメーション
	StartShieldEnemyAnimation(SHIELD_ENEMY_RUN, index);

	// 増えれば追加する
	// StartShieldEnemyAnimation(???, index);



	// アニメーション更新
	ShieldEnemyAnimationType animType = shieldEnemy->playAnim;
	AnimationData* animData = &shieldEnemy->animation[animType];
	UpdateAnimation(animData);
}