#include "DxLib.h"
#include "ShieldEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../Score/Score.h"
#include "../../Player/YellowSelect/YellowSelect.h"
#include "../../Sound/SoundManager.h"

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
	10, 6, 50, 50,	// DIE
	5, 10, 50, 50,	// CRUSH
};

//移動速度
#define SHIELD_ENEMY_MOVE_SPEED	(0.8f)

// 重力
#define SHIELD_ENEMY_GRAVITY (0.3f)

// マップ衝撃判定のサイズ補正
#define SHIELD_ENEMY_COLLISION_OFFSET	(1.0f)

// 死亡エフェクトインターバル
#define SHIELD_ENEMY_DEAD_EFFECT_INTARVAL	(5)

// 矩形判定サイズ
#define SHIELD_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define SHIELD_ENEMY_BOX_COLLISION_HEIGHT	(47.0f)

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
		shieldEnemy->crush = false;
		shieldEnemy->die = false;
		shieldEnemy->isTurn = false;

		shieldEnemy->crushTimer = 0;
		shieldEnemy->dieTimer = 0;

		shieldEnemy->playAnim = SHIELD_ENEMY_ANIM_NONE;

		shieldEnemy->boxCollision.width = SHIELD_ENEMY_BOX_COLLISION_WIDTH;
		shieldEnemy->boxCollision.height = SHIELD_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < SHIELD_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&shieldEnemy->animation[j]);
		}

	}
}


void LoadShieldEnemy()
{
	int runHandle = LoadGraph("Data/animation/Shield_Enemy/shield_enemy_run.png");
	int crushHandle = LoadGraph("Data/animation/Shield_Enemy/shield_enemy_crush.png");
	int dieHandle = LoadGraph("Data/animation/Shield_Enemy/shield_enemy_dead.png");

	for (int i = 0; i < SHIELD_ENEMY_MAX; i++)
	{
		g_ShieldEnemyData[i].animation[SHIELD_ENEMY_RUN].handle = runHandle;
		g_ShieldEnemyData[i].animation[SHIELD_ENEMY_CRUSH].handle = crushHandle;
		g_ShieldEnemyData[i].animation[SHIELD_ENEMY_DIE].handle = dieHandle;
	}
}


void StepShieldEnemy()
{
	if (g_IsYellowSelecting) return;

	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;

	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)continue;

		if (shieldEnemy->crush) continue;

		g_PravShieldEnemyData[i] = g_ShieldEnemyData[i];

		// 重力
		shieldEnemy->move.y += SHIELD_ENEMY_GRAVITY;

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
	if (g_IsYellowSelecting) return;

	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;

	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)continue;

		if (shieldEnemy->crush)
		{
			shieldEnemy->crushTimer--;

			if (shieldEnemy->crushTimer <= 0)
			{
				shieldEnemy->active = false;
				continue;
			}

			UpdateShieldEnemyAnimation(i);
			continue;
		}

		if (shieldEnemy->die)
		{
			shieldEnemy->dieTimer--;

			if (shieldEnemy->dieTimer <= 0)
			{
				shieldEnemy->active = false;
				continue;
			}

			UpdateShieldEnemyAnimation(i);
			continue;
		}

		shieldEnemy->pos.x += shieldEnemy->move.x;
		shieldEnemy->pos.y += shieldEnemy->move.y;

		UpdateShieldEnemyAnimation(i);
	}

}


void DrawShieldEnemy()
{
	ShieldEnemyData* shieldEnemy = g_ShieldEnemyData;
	CameraData cam = GetCamera();

	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shieldEnemy++)
	{
		if (!shieldEnemy->active)continue;

		ShieldEnemyAnimationType animType = shieldEnemy->playAnim;
		AnimationData* animData = &shieldEnemy->animation[animType];

		float drawX = shieldEnemy->pos.x - cam.posX;
		float drawY = shieldEnemy->pos.y - cam.posY;

		if (!shieldEnemy->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, TRUE, FALSE);
		}
		else if (shieldEnemy->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, FALSE, FALSE);
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
	ShieldEnemyData* shieldEnemy = &g_ShieldEnemyData[index];
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = SHIELD_ENEMY_COLLISION_OFFSET;
	const float SIZE_OFFSET = SHIELD_ENEMY_COLLISION_OFFSET * 2;

	float prevX = g_PravShieldEnemyData[index].pos.x;

	if (CheckSquareSquare(
		shieldEnemy->pos.x,
		shieldEnemy->pos.y,
		SHIELD_ENEMY_BOX_COLLISION_WIDTH + 10.0f,
		SHIELD_ENEMY_BOX_COLLISION_HEIGHT - SIZE_OFFSET,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (prevX + SHIELD_ENEMY_BOX_COLLISION_WIDTH <= block->pos.x)
		{
			shieldEnemy->isTurn = true;
		}
		// 右から当たった
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			shieldEnemy->isTurn = false;
		}
	}

}

void ShieldEnemyHitBlockY(MapChipData mapChipData, int index)
{
	ShieldEnemyData* shieldEnemy = &g_ShieldEnemyData[index];
	ShieldEnemyData* prev = &g_PravShieldEnemyData[index];
	BlockData* block = mapChipData.data;
	if (CheckSquareSquare(
		shieldEnemy->pos.x,
		shieldEnemy->pos.y,
		SHIELD_ENEMY_BOX_COLLISION_WIDTH,
		SHIELD_ENEMY_BOX_COLLISION_HEIGHT,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 上から落ちた
		if (prev->pos.y + SHIELD_ENEMY_BOX_COLLISION_HEIGHT <= block->pos.y)
		{
			shieldEnemy->pos.y = block->pos.y - SHIELD_ENEMY_BOX_COLLISION_HEIGHT;
			shieldEnemy->move.y = 0.0f;
			shieldEnemy->isAir = false;
		}
		// 下から当たった
		else if (prev->pos.y >= block->pos.y + MAP_CHIP_HEIGHT)
		{
			shieldEnemy->pos.y = block->pos.y + MAP_CHIP_HEIGHT;
			shieldEnemy->move.y = 0.0f;
		}
	}
}

void PlayerKillShieldEnemy(int index)
{
	ShieldEnemyData* ShieldEnemy = &g_ShieldEnemyData[index];

	if (ShieldEnemy->crush) return;

	ShieldEnemy->crush = true;
	ShieldEnemy->crushTimer = 50;

	ShieldEnemy->move.x = 0.0f;
	ShieldEnemy->move.y = 0.0f;

	StartShieldEnemyAnimation(SHIELD_ENEMY_CRUSH, index);

	// スコア
	AddScore(500);
}

void PlayerKillShieldEnemyYellow(int index)
{
	ShieldEnemyData* enemy = &g_ShieldEnemyData[index];

	if (enemy->crush || enemy->die) return;

	enemy->die = true;
	enemy->dieTimer = 60;

	enemy->move.x = 0;
	enemy->move.y = 0;

	// SE
	PlaySE(SE_ENEMY_DEAD);

	StartShieldEnemyAnimation(SHIELD_ENEMY_DIE, index);

	AddScore(500);
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

	if (shieldEnemy->die)
	{
		StartShieldEnemyAnimation(SHIELD_ENEMY_DIE, index);
	}
	else if (shieldEnemy->crush)
	{
		StartShieldEnemyAnimation(SHIELD_ENEMY_CRUSH, index);
	}
	else
	{
		StartShieldEnemyAnimation(SHIELD_ENEMY_RUN, index);
	}

	// アニメーション更新
	ShieldEnemyAnimationType animType = shieldEnemy->playAnim;
	AnimationData* animData = &shieldEnemy->animation[animType];
	UpdateAnimation(animData);
}

//踏みつけ系↓
extern PlayerData g_PrevPlayerData;

bool UpdateShieldCrush(int enemyIndex)
{
	PlayerData* player = GetPlayer();
	ShieldEnemyData* enemy = GetShieldEnemy();

	if (player->type != TYPE_BLUE) return false;

	if (player->move.y <= 0.0f) return false;

	ShieldEnemyData* e = &enemy[enemyIndex];

	if (!e->active) return false;

	float playerBottomPrev = g_PrevPlayerData.pos.y + PLAYER_HEIGHT - 10.0f;
	float enemyTop = e->pos.y;

	if (playerBottomPrev <= enemyTop)
	{
		PlayerKillShieldEnemy(enemyIndex);

		// SE
		PlaySE(SE_PLAYER_CRUSH);

		player->move.y = -8.0f;

		return true;
	}

	return false;
}