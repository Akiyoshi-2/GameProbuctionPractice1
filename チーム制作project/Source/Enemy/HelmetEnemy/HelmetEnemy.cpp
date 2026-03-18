#include "DxLib.h"
#include "HelmetEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Camera/Camera.h"
#include "../../Player/Attack/Attack.h"
#include "../../Score/Score.h"
#include "../../Player/YellowSelect/YellowSelect.h"
#include "../../Player/Player.h"
#include "../../Sound/SoundManager.h"

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
	10, 6, 50, 50,	// DIE
	5, 10, 50, 50,  // STRIKE
};

// 移動速度
#define HELMET_ENEMY_MOVE_SPEED	(0.8f)

// 重力
#define HELMET_ENEMY_GRAVITY (0.3f)

// マップ衝撃判定のサイズ補正
#define HELMET_ENEMY_COLLISION_OFFSET	(1.0f)

// 死亡エフェクトインターバル
#define HELMET_ENEMY_DEAD_EFFECT_INTERVAL	(5)

// 矩形判定サイズ
#define HELMET_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define HELMET_ENEMY_BOX_COLLISION_HEIGHT	(47.0f)

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
		helmet->isDead = false;
		helmet->strike = false;
		helmet->isTurn = false;

		helmet->strikeTimer = 0;
		helmet->dieTimer = 0;

		helmet->playAnim = HELMET_ENEMY_ANIM_NONE;

		helmet->boxCollision.width = HELMET_ENEMY_BOX_COLLISION_WIDTH;
		helmet->boxCollision.height = HELMET_ENEMY_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < HELMET_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&helmet->animation[j]);
		}
	}
}

void LoadHelmetEnemy()
{
	int runHandle = LoadGraph("Data/animation/Helmet_Enemy/helmet_enemy_run.png");
	int dieHandle = LoadGraph("Data/animation/Helmet_Enemy/helmet_enemy_dead.png");
	int strikeHandle = LoadGraph("Data/animation/Helmet_Enemy/helmet_enemy_strike.png");

	for (int i = 0; i < HELMET_ENEMY_MAX; i++)
	{
		g_HelmetEnemyData[i].animation[HELMET_ENEMY_ANIM_RUN].handle = runHandle;
		g_HelmetEnemyData[i].animation[HELMET_ENEMY_ANIM_DIE].handle = dieHandle;
		g_HelmetEnemyData[i].animation[HELMET_ENEMY_ANIM_STRIKE].handle = strikeHandle;
	}
}

void StepHelmetEnemy()
{
	if (g_IsYellowSelecting) return;

	HelmetEnemyData* helmet = g_HelmetEnemyData;

	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active) continue;

		// DIE
		if (helmet->isDead) continue;

		g_PravHelmetEnemyData[i] = g_HelmetEnemyData[i];

		helmet->move.y += HELMET_ENEMY_GRAVITY;

		//helmet->move.x = (!helmet->isTurn) ? HELMET_ENEMY_MOVE_SPEED : -HELMET_ENEMY_MOVE_SPEED;
		if (!helmet->isTurn)
		{
			helmet->move.x = HELMET_ENEMY_MOVE_SPEED;
		}
		else
		{
			helmet->move.x = -HELMET_ENEMY_MOVE_SPEED;
		}
	}
}

void UpdateHelmetEnemy()
{
	if (g_IsYellowSelecting) return;

	HelmetEnemyData* helmet = g_HelmetEnemyData;

	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active) continue;

		// STRIKE死亡
		if (helmet->strike)
		{
			helmet->strikeTimer--;

			if (helmet->strikeTimer <= 0)
			{
				helmet->active = false;
				AddScore(500);
				continue;
			}

			UpdateHelmetEnemyAnimation(i);
			continue;
		}

		// DIE死亡
		if (helmet->isDead)
		{
			helmet->dieTimer--;

			if (helmet->dieTimer <= 0)
			{
				helmet->active = false;
				continue;
			}

			UpdateHelmetEnemyAnimation(i);
			continue;
		}

		helmet->pos.x += helmet->move.x;
		helmet->pos.y += helmet->move.y;

		if (IsAttackActive())
		{
			float attackx = 0.0f;
			float attacky = 0.0f;
			float attackw = 0.0f;
			float attackh = 0.0f;

			float enemyx = helmet->pos.x;
			float enemyy = helmet->pos.y;

			GetAttackRect(attackx, attacky, attackw, attackh);

			if (CheckSquareSquare(
				attackx, attacky, attackw, attackh,
				enemyx,
				enemyy,
				HELMET_ENEMY_BOX_COLLISION_WIDTH,
				HELMET_ENEMY_BOX_COLLISION_HEIGHT))
			{
				// プレイヤー取得
				PlayerData* player = GetPlayer();

				if (player != nullptr)
				{
					if (player->pos.x < helmet->pos.x)
					{
						helmet->isTurn = true;
					}
					else if (player->pos.x > helmet->pos.x)
					{
						helmet->isTurn = false;
					}
				}

				helmet->strike = true;
				helmet->strikeTimer = 50;

				StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_STRIKE, i);
			}
		}

		UpdateHelmetEnemyAnimation(i);
	}
}

void DrawHelmetEnemy()
{
	HelmetEnemyData* helmet = g_HelmetEnemyData;
	CameraData cam = GetCamera();

	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		if (!helmet->active) continue;

		HelmetAnimationType animType = helmet->playAnim;
		AnimationData* animData = &helmet->animation[animType];

		float drawX = helmet->pos.x - cam.posX;
		float drawY = helmet->pos.y - cam.posY;

		if (!helmet->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, TRUE, FALSE);
		}
		else
		{
			DrawAnimation(animData, drawX, drawY, FALSE, FALSE);
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

	if (helmet->strike) return;

	helmet->isDead = true;

	helmet->strike = true;
	helmet->strikeTimer = 50;

	helmet->move.x = 0.0f; //攻撃中は動かない
	helmet->move.y = 0.0f;

	StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_STRIKE, index);

	// スコア
	AddScore(500);
}

void PlayerKillHelmetEnemyYellow(int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];

	if (helmet->strike || helmet->isDead) return;

	helmet->isDead = true;
	helmet->dieTimer = 60;

	helmet->move.x = 0;
	helmet->move.y = 0;

	// SE
	PlaySE(SE_ENEMY_DEAD);

	StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_DIE, index);

	AddScore(500);
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

	float prevX = g_PravHelmetEnemyData[index].pos.x;

	if (CheckSquareSquare(
		helmet->pos.x,
		helmet->pos.y,
		HELMET_ENEMY_BOX_COLLISION_WIDTH + 10.0f,
		HELMET_ENEMY_BOX_COLLISION_HEIGHT - HELMET_ENEMY_COLLISION_OFFSET,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (prevX + HELMET_ENEMY_BOX_COLLISION_WIDTH <= block->pos.x)
		{
			helmet->isTurn = true;
		}
		// 右から当たった
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			helmet->isTurn = false;
		}
	}
}

void HelmetEnemyHitBlockY(MapChipData mapChipData, int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];
	HelmetEnemyData* prev = &g_PravHelmetEnemyData[index];
	BlockData* block = mapChipData.data;

	if (CheckSquareSquare(
		helmet->pos.x,
		helmet->pos.y,
		HELMET_ENEMY_BOX_COLLISION_WIDTH,
		HELMET_ENEMY_BOX_COLLISION_HEIGHT,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 上から落ちた
		if (prev->pos.y + HELMET_ENEMY_BOX_COLLISION_HEIGHT <= block->pos.y)
		{
			helmet->pos.y = block->pos.y - HELMET_ENEMY_BOX_COLLISION_HEIGHT;
			helmet->move.y = 0.0f;
			helmet->isAir = false;
		}
		// 下から当たった
		else if (prev->pos.y >= block->pos.y + MAP_CHIP_HEIGHT)
		{
			helmet->pos.y = block->pos.y + MAP_CHIP_HEIGHT;
			helmet->move.y = 0.0f;
		}
	}
}

void StartHelmetEnemyAnimation(HelmetAnimationType anim, int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];

	if (anim == helmet->playAnim) return;

	helmet->playAnim = anim;

	AnimationData* animData = &helmet->animation[anim];
	helmetEnemyAnimationParam animParam = HELMET_ENEMYANIM_PARAM[anim];

	StartAnimation(animData,
		helmet->pos.x,
		helmet->pos.y,
		animParam.interval,
		animParam.frameNum,
		animParam.width,
		animParam.height,
		true);
}

void UpdateHelmetEnemyAnimation(int index)
{
	HelmetEnemyData* helmet = &g_HelmetEnemyData[index];

	if (helmet->isDead)
	{
		StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_DIE, index);
	}
	else if (helmet->strike)
	{
		StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_STRIKE, index);
	}
	else
	{
		StartHelmetEnemyAnimation(HELMET_ENEMY_ANIM_RUN, index);
	}

	AnimationData* animData = &helmet->animation[helmet->playAnim];
	UpdateAnimation(animData);

	// STRIKE終了で active を切る
	if ((helmet->strike || helmet->isDead) && animData->isEnd)
	{
		helmet->active = false;
	}
}

