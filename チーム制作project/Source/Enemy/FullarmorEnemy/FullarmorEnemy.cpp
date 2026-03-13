#include "DxLib.h"
#include "FullarmorEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../Score/Score.h"

// アニメーション用パラメータ
struct FullArmEnemyAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};

const FullArmEnemyAnimationParam FULLARMOR_ENEMY_ANIM_PARAM[FULLARMOR_ENEMY_ANIM_MAX] =
{
	10, 2, 50, 50,	// RUN
	10, 6, 50, 50,	// DIE
};

// 移動速度
#define FULLARMOR_ENEMY_MOVE_SPEED	(0.5f)

// 重力
#define FULLARMOR_ENEMY_GRAVITY		(0.5f)

// マップ衝突判定のサイズ補正
#define FULLARMOR_ENEMY_COLLISION_OFFSET	(1.0f)

// 死亡エフェクトインターバル
#define FULLARMOR_ENEMY_DEAD_EFFECT_INTARVAL	(5)

// 矩形判定サイズ
#define FULLARMOR_ENEMY_BOX_COLLISION_WIDHT		(38.0f)
#define FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT	(47.0f)

// 撃破時のスコア
#define FULLARMOR_ENEMY_SCORE	(5000)

FullArmEnemyData g_FullArmEnemyData[FULLARMOR_ENEMY_MAX] = { 0 };
FullArmEnemyData g_PravFullArmEnemyData[FULLARMOR_ENEMY_MAX] = { 0 };

void StartFullArmEnemyAnimation(FullArmEnemyAnimationType anim, int index);
void UpdateFullArmEnemyAnimation(int index);

void InitFullArmorEnemy()
{
	FullArmEnemyData* fullArmor = g_FullArmEnemyData;
	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArmor++)
	{
		// 座標
		fullArmor->pos.x = 0;
		fullArmor->pos.y = 0;
		// 移動量
		fullArmor->move.x = 0;
		fullArmor->move.y = 0;
		// 状態
		fullArmor->active = false;
		fullArmor->die = false;
		// タイマー
		fullArmor->dieTimer = 0;
		// 再生中アニメーション
		fullArmor->playAnim = FULLARMOR_ENEMY_ANIM_NONE;
		// 当たり判定サイズ
		fullArmor->boxCollision.width = FULLARMOR_ENEMY_BOX_COLLISION_WIDHT;
		fullArmor->boxCollision.height = FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < FULLARMOR_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&fullArmor->animation[j]);
		}
	}
}

void LoadFullArmorEnemy()
{
	int runHandle = LoadGraph("Data/animation/Fullarmor_Enemy/fullarmor_enemy_run.png");
	int dieHandle = LoadGraph("Data/animation/Fullarmor_Enemy/fullarmor_enemy_die.png");

	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++)
	{
		g_FullArmEnemyData[i].animation[FULLARMOR_ENEMY_ANIM_RUN].handle = runHandle;
		g_FullArmEnemyData[i].animation[FULLARMOR_ENEMY_ANIM_DIE].handle = dieHandle;
	}
}

void StepFullArmorEnemy()
{
	FullArmEnemyData* fullArmor = g_FullArmEnemyData;
	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArmor++)
	{
		if (!fullArmor->active)continue;

		g_PravFullArmEnemyData[i] = g_FullArmEnemyData[i];

		fullArmor->move.y += FULLARMOR_ENEMY_GRAVITY;

		fullArmor->move.x = 0.0f;

		if (!fullArmor->isTurn)
		{
			fullArmor->move.x = FULLARMOR_ENEMY_MOVE_SPEED;
		}
		else if (fullArmor->isTurn)
		{
			fullArmor->move.x = -FULLARMOR_ENEMY_MOVE_SPEED;
		}
	}
}

void UpdateFullArmorEnemy()
{
	FullArmEnemyData* fullArmor = g_FullArmEnemyData;
	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArmor++)
	{
		if (!fullArmor->active)continue;

		// 死亡処理
		if (fullArmor->die)
		{
			fullArmor->dieTimer--;

			if (fullArmor->dieTimer <= 0)
			{
				fullArmor->active = false;
				continue;
			}

			UpdateFullArmEnemyAnimation(i);
			continue;
		}

		fullArmor->pos.x += fullArmor->move.x;
		fullArmor->pos.y += fullArmor->move.y;

		UpdateFullArmEnemyAnimation(i);
	}
}

void DrawFullArmorEnemy()
{
	FullArmEnemyData* fullArmor = g_FullArmEnemyData;
	CameraData cam = GetCamera();

	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArmor++)
	{
		if (!fullArmor->active)continue;

		FullArmEnemyAnimationType animType = fullArmor->playAnim;
		AnimationData* animData = &fullArmor->animation[animType];

		float drawX = fullArmor->pos.x - cam.posX;
		float drawY = fullArmor->pos.y - cam.posY;

		if (!fullArmor->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, TRUE, FALSE);
		}
		else if (fullArmor->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, FALSE, FALSE);
		}
	}

}

void FinFullArmorEnemy()
{
	for (int i = 0; i < FULLARMOR_ENEMY_ANIM_MAX; i++)
	{
		DeleteGraph(g_FullArmEnemyData->animation[i].handle);
	}
}

FullArmEnemyData* GetFullArmorEnemy()
{
	return g_FullArmEnemyData;
}

void PlayerKillFullArmorEnemy(int index)
{
	FullArmEnemyData* fullArmor = &g_FullArmEnemyData[index];

	if (fullArmor->die) return;

	fullArmor->die = true;
	fullArmor->dieTimer = 60;

	fullArmor->move.x = 0.0f;
	fullArmor->move.y = 0.0f;

	StartFullArmEnemyAnimation(FULLARMOR_ENEMY_ANIM_DIE, index);

	AddScore(5000);
}

void CreateFullArmorEnemy(float posX, float posY, const EnemyParameter* param)
{
	FullArmEnemyData* fullArmor = g_FullArmEnemyData;
	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArmor++)
	{
		if (!fullArmor->active)
		{
			fullArmor->active = true;

			fullArmor->animation->handle = g_FullArmEnemyData->animation->handle;

			fullArmor->pos.x = posX;
			fullArmor->pos.y = posY;

			fullArmor->move.x = FULLARMOR_ENEMY_MOVE_SPEED;
			fullArmor->move.y = 0.0f;

			fullArmor->param = param;

			break;
		}
	}
}

void FullArmorEnemyHitBlockX(MapChipData mapChipData, int index)
{
	FullArmEnemyData* fullArmor = &g_FullArmEnemyData[index];
	BlockData* block = mapChipData.data;

	float prevX = g_PravFullArmEnemyData[index].pos.x;
	float prevY = g_PravFullArmEnemyData[index].pos.y;

	if (CheckSquareSquare(fullArmor->pos.x, fullArmor->pos.y,
		FULLARMOR_ENEMY_BOX_COLLISION_WIDHT + FULLARMOR_ENEMY_COLLISION_OFFSET,
		FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT - FULLARMOR_ENEMY_COLLISION_OFFSET,
		block->pos.x, block->pos.y,
		MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (prevX + FULLARMOR_ENEMY_BOX_COLLISION_WIDHT <= block->pos.x)
		{
			fullArmor->isTurn = true;
		}
		// 右から当たった
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			fullArmor->isTurn = false;
		}
	}
}

void FullArmorEnemyHitBlockY(MapChipData mapChipData, int index)
{
	FullArmEnemyData* fullArmor = &g_FullArmEnemyData[index];
	FullArmEnemyData* prevFullArmor = &g_PravFullArmEnemyData[index];
	BlockData* block = mapChipData.data;

	if (CheckSquareSquare(fullArmor->pos.x, fullArmor->pos.y,
		FULLARMOR_ENEMY_BOX_COLLISION_WIDHT,
		FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT,
		block->pos.x, block->pos.y,
		MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 上から落ちてきた
		if (prevFullArmor->pos.y + FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT <= block->pos.y)
		{
			fullArmor->pos.y = block->pos.y - FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT;
			fullArmor->move.y = 0.0f;
			fullArmor->isAir = false;
		}
		// 下から当たった
		else if (prevFullArmor->pos.y >= block->pos.y + MAP_CHIP_HEIGHT)
		{
			fullArmor->pos.y = block->pos.y + MAP_CHIP_HEIGHT;
			fullArmor->move.y = 0.0f;
		}
	}
}

void StartFullArmEnemyAnimation(FullArmEnemyAnimationType anim, int index)
{
	FullArmEnemyData* fullArmor = &g_FullArmEnemyData[index];

	if (anim == fullArmor->playAnim)return;

	fullArmor->playAnim = anim;

	AnimationData* animData = &fullArmor->animation[index];
	FullArmEnemyAnimationParam animParam = FULLARMOR_ENEMY_ANIM_PARAM[index];

	StartAnimation(animData, fullArmor->pos.x, fullArmor->pos.y,
		animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdateFullArmEnemyAnimation(int index)
{
	FullArmEnemyData* fullArmor = &g_FullArmEnemyData[index];

	if (fullArmor->die)
	{
		StartFullArmEnemyAnimation(FULLARMOR_ENEMY_ANIM_DIE, index);
	}
	else
	{
		StartFullArmEnemyAnimation(FULLARMOR_ENEMY_ANIM_RUN, index);
	}

	AnimationData* animData = &fullArmor->animation[fullArmor->playAnim];
	UpdateAnimation(animData);

}