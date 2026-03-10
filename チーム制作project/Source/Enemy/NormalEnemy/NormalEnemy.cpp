#include "DxLib.h"
#include "NormalEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Player/Attack/Attack.h"
#include "../../Collision/Collision.h"
#include "../../Camera/Camera.h"
#include "../../Score/Score.h"

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
	5, 10, 50, 50,	// CRUSH
	5, 10, 50, 50,	// STRIKE
};

// 移動速度
#define NORMAL_ENEMY_MOVE_SPEED (0.8f)

//重力
#define NORMAL_ENEMY_GRAVITY (0.3f)

// マップ衝撃判定のサイズ補正
#define NORMAL_ENEMY_COLLISION_OFFSET (1.0f)

// 死亡エフェクトインターバル
#define NORMAL_ENEMY_DEAD_EFFECT_INTARVAL (5)

// 矩形判定サイズ
#define NORMAL_ENEMY_BOX_COLLISION_WIDTH (38.0f)
#define NORMAL_ENEMY_BOX_COLLISION_HEIGHT (47.0f)

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
		// 座標
		normalEnemy->pos.x = 0;
		normalEnemy->pos.y = 0;

		// 移動量
		normalEnemy->move.x = 0;
		normalEnemy->move.y = 0;

		// 状態
		normalEnemy->active = false;	//敵がゲーム内で有効かどうか
		normalEnemy->crush = false;		//踏みつけられて死亡中か
		normalEnemy->strike = false;	//攻撃されて死亡中か

		// タイマー
		normalEnemy->crushTimer = 0;
		normalEnemy->strikeTimer = 0;

		// 再生アニメーション
		normalEnemy->playAnim = NORMAL_ENEMY_ANIM_NONE;

		// 当たり判定サイズ
		normalEnemy->boxCollision.width = NORMAL_ENEMY_BOX_COLLISION_WIDTH;
		normalEnemy->boxCollision.height = NORMAL_ENEMY_BOX_COLLISION_HEIGHT;

		// アニメーション初期化
		for (int j = 0; j < NORMAL_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&normalEnemy->animation[j]);
		}
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
		if (!normalEnemy->active) continue;

		if (normalEnemy->crush || normalEnemy->strike) continue;

		g_PravNormalEnemyData[i] = g_NormalEnemyData[i];

		// 重力
		normalEnemy->move.y += NORMAL_ENEMY_GRAVITY;

		// 横移動初期化
		normalEnemy->move.x = 0.0f;

		// 向きによる移動
		if (!normalEnemy->isTurn)
		{
			normalEnemy->move.x = NORMAL_ENEMY_MOVE_SPEED;
		}
		else
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
		if (!normalEnemy->active) continue;

		// 踏みつけ死亡処理
		if (normalEnemy->crush)
		{
			normalEnemy->crushTimer--;

			if (normalEnemy->crushTimer <= 0)
			{
				normalEnemy->active = false;
				continue;
			}

			UpdateNormalEnemyAnimation(i);
			continue;
		}

		// 攻撃死亡処理
		if (normalEnemy->strike)
		{
			normalEnemy->strikeTimer--;

			if (normalEnemy->strikeTimer <= 0)
			{
				normalEnemy->active = false;
				AddScore(200);
				continue;
			}

			UpdateNormalEnemyAnimation(i);
			continue;
		}

		// 移動
		normalEnemy->pos.x += normalEnemy->move.x;
		normalEnemy->pos.y += normalEnemy->move.y;

		// 攻撃判定
		if (IsAttackActive())
		{
			float attackx = 0.0f;
			float attacky = 0.0f;
			float attackw = 0.0f;
			float attackh = 0.0f;

			float enemyx = normalEnemy->pos.x;
			float enemyy = normalEnemy->pos.y;

			GetAttackRect(attackx, attacky, attackw, attackh);

			if (CheckSquareSquare(
				attackx, attacky, attackw, attackh,
				enemyx,
				enemyy,
				NORMAL_ENEMY_BOX_COLLISION_WIDTH,
				NORMAL_ENEMY_BOX_COLLISION_HEIGHT))
			{
				normalEnemy->strike = true;
				normalEnemy->strikeTimer = 50;

				StartNormalEnemyAnimation(NORMAL_ENEMY_STRIKE, i);
			}
		}

		UpdateNormalEnemyAnimation(i);
	}
}

void DrawNormalEnemy()
{
	NormalEnemyData* normalEnemy = g_NormalEnemyData;
	CameraData cam = GetCamera();

	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normalEnemy++)
	{
		if (!normalEnemy->active) continue;

		NormalEnemyAnimationType animType = normalEnemy->playAnim;
		AnimationData* animData = &normalEnemy->animation[animType];

		float drawX = normalEnemy->pos.x - cam.posX;
		float drawY = normalEnemy->pos.y - cam.posY;

		// 描画
		if (!normalEnemy->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, TRUE, FALSE);
		}
		else
		{
			DrawAnimation(animData, drawX, drawY, FALSE, FALSE);
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

			// パラメータ
			normalEnemy->param = param;

			break;
		}
	}
}

NormalEnemyData* GetNormalEnemy()
{
	return g_NormalEnemyData;
}

void NormalEnemyHitBlockX(MapChipData mapChipData, int index)
{
	NormalEnemyData* normalEnemy = &g_NormalEnemyData[index];
	BlockData* block = mapChipData.data;

	float prevX = g_PravNormalEnemyData[index].pos.x;
	float prevY = g_PravNormalEnemyData[index].pos.y;

	if (CheckSquareSquare(
		normalEnemy->pos.x,
		normalEnemy->pos.y,
		NORMAL_ENEMY_BOX_COLLISION_WIDTH + 10.0f,
		NORMAL_ENEMY_BOX_COLLISION_HEIGHT - NORMAL_ENEMY_COLLISION_OFFSET,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (prevX + NORMAL_ENEMY_BOX_COLLISION_WIDTH <= block->pos.x)
		{
			normalEnemy->isTurn = true;
		}
		// 右から当たった
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			normalEnemy->isTurn = false;
		}
	}
}

void NormalEnemyHitBlockY(MapChipData mapChipData, int index)
{
	NormalEnemyData* normalEnemy = &g_NormalEnemyData[index];
	NormalEnemyData* prevEnemy = &g_PravNormalEnemyData[index];
	BlockData* block = mapChipData.data;

	if (CheckSquareSquare(
		normalEnemy->pos.x,
		normalEnemy->pos.y,
		NORMAL_ENEMY_BOX_COLLISION_WIDTH,
		NORMAL_ENEMY_BOX_COLLISION_HEIGHT,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 上から落ちてきた
		if (prevEnemy->pos.y + NORMAL_ENEMY_BOX_COLLISION_HEIGHT <= block->pos.y)
		{
			normalEnemy->pos.y = block->pos.y - NORMAL_ENEMY_BOX_COLLISION_HEIGHT;
			normalEnemy->move.y = 0.0f;
			normalEnemy->isAir = false;
		}
		// 下から当たった
		else if (prevEnemy->pos.y >= block->pos.y + MAP_CHIP_HEIGHT)
		{
			normalEnemy->pos.y = block->pos.y + MAP_CHIP_HEIGHT;
			normalEnemy->move.y = 0.0f;
		}
	}
}

void PlayerKillNormalEnemy(int index)
{
	NormalEnemyData* enemy = &g_NormalEnemyData[index];

	// すでに死亡処理中なら何もしない
	if (enemy->crush || enemy->strike) return;

	enemy->crush = true;
	enemy->crushTimer = 50;

	// 移動停止
	enemy->move.x = 0.0f;
	enemy->move.y = 0.0f;

	// 踏みつけアニメーション
	StartNormalEnemyAnimation(NORMAL_ENEMY_CRUSH, index);

	// スコア
	AddScore(200);
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

	if (normalEnemy->crush)
	{
		StartNormalEnemyAnimation(NORMAL_ENEMY_CRUSH, index);
	}
	else if (normalEnemy->strike)
	{
		StartNormalEnemyAnimation(NORMAL_ENEMY_STRIKE, index);
	}
	else
	{
		StartNormalEnemyAnimation(NORMAL_ENEMY_RUN, index);
	}

	AnimationData* animData = &normalEnemy->animation[normalEnemy->playAnim];
	UpdateAnimation(animData);
}