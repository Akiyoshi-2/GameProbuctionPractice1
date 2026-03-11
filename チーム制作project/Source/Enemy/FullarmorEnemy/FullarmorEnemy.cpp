#include "DxLib.h"
#include "FullarmorEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Camera/Camera.h"

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
	10, 2, 50, 50,
	8, 6, 50, 50,
};

#define FULLARMOR_ENEMY_MOVE_SPEED	(0.5f)

#define FULLARMOR_ENEMY_COLLISION_OFFSET	(0.1f)

#define FULLARMOR_ENEMY_DEAD_EFFECT_INTARVAL	(5)

#define FULLARMOR_ENEMY_BOX_COLLISION_WIDHT		(40)
#define FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT	(43)

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
		fullArmor->pos.x = 0;
		fullArmor->pos.y = 0;
		fullArmor->move.x = 0;
		fullArmor->move.y = 0;
		fullArmor->active = false;
		fullArmor->playAnim = FULLARMOR_ENEMY_ANIM_NONE;
		fullArmor->boxCollision.width = FULLARMOR_ENEMY_BOX_COLLISION_WIDHT;
		fullArmor->boxCollision.height = FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < FULLARMOR_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&fullArmor->animation[j]);
		}

		memset(&fullArmor[i].boxCollision, 0, sizeof(fullArmor[i].boxCollision));
	}
}

void LoadFullArmorEnemy()
{
	int runHandle = LoadGraph("Data/animation/Fullarmor_Enemy/fullarmor_enemy_run.png");

	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++)
	{
		g_FullArmEnemyData[i].animation[FULLARMOR_ENEMY_ANIM_RUN].handle = runHandle;
	}
}

void StepFullArmorEnemy()
{
	FullArmEnemyData* fullArmor = g_FullArmEnemyData;
	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArmor++)
	{
		if (!fullArmor->active)continue;

		g_PravFullArmEnemyData[i] = g_FullArmEnemyData[i];

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

	fullArmor->active = false;

	// スコア
	// int score = GetScore() + FULLARMOR_ENEMY_SCORE;
	// SetScore(score);
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
	const float POS_OFFSET = FULLARMOR_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = FULLARMOR_ENEMY_COLLISION_OFFSET * 2;

	fullArmor->pos.x = g_FullArmEnemyData[index].pos.x;
	fullArmor->pos.y = g_FullArmEnemyData[index].pos.y;

	if (CheckSquareSquare(fullArmor->pos.x + POS_OFFSET, fullArmor->pos.y + POS_OFFSET,
		FULLARMOR_ENEMY_BOX_COLLISION_WIDHT - SIZE_OFFSET, FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (fullArmor->move.x > 0.0f)
		{
			fullArmor->isTurn = false;
		}
		else if (fullArmor->move.x < 0.0f)
		{
			fullArmor->isTurn = true;
		}
	}
}

void FullArmorEnemyHitBlockY(MapChipData mapChipData, int index)
{
	FullArmEnemyData* fullArmor = &g_FullArmEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = FULLARMOR_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = FULLARMOR_ENEMY_COLLISION_OFFSET * 2;

	fullArmor->isTurn = g_PravFullArmEnemyData->isTurn;

	if (CheckSquareSquare(fullArmor->pos.x + POS_OFFSET, fullArmor->pos.y + POS_OFFSET,
		FULLARMOR_ENEMY_BOX_COLLISION_WIDHT - SIZE_OFFSET, FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (fullArmor->move.y > 0.0f)
		{
			fullArmor->pos.y -= (fullArmor->pos.y * FULLARMOR_ENEMY_BOX_COLLISION_HEIGHT) - block->pos.y;
			fullArmor->isAir = false;
		}
		else if (fullArmor->move.y < 0.0f)
		{
			fullArmor->pos.y += (block->pos.y + MAP_CHIP_WIDTH) - fullArmor->pos.y;
		}
	}

	fullArmor->move.y = 0.0f;
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

	StartFullArmEnemyAnimation(FULLARMOR_ENEMY_ANIM_RUN, index);
	//増えれば追加する

	FullArmEnemyAnimationType animType = fullArmor->playAnim;
	AnimationData* animData = &fullArmor->animation[animType];
	UpdateAnimation(animData);

}