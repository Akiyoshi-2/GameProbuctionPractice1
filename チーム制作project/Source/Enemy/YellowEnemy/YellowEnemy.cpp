#include "Dxlib.h"
#include "YellowEnemy.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Map/Block.h"
#include "../../Camera/Camera.h"
#include "../../Player/Attack/Attack.h"
#include "../../Score/Score.h"
#include "../../Player/Player.h"
#include "../../Player/YellowSelect/YellowSelect.h"
#include "../../Player/YellowStock/YellowStock.h"
#include "../../Sound/SoundManager.h"

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
	5, 6, 50, 50,	// STUN
	7, 1, 50, 50,	// STUNSTOP
	5, 10, 50, 50,	// CRUSH
	5, 10, 50, 50,	// STRIKE
	10, 6, 50, 50,	//DIE
	10, 6, 50, 50,	//DIE2
};

// 移動速度
#define YELLOW_ENEMY_MOVE_SPEED	(1.2f)

// マップ衝撃判定のサイズ補正
#define YELLOW_ENEMY_COLLISION_OFFSET	(1.0f)

// 死亡エフェクトインターバル
#define YELLOW_ENEMY_DEAD_EFFECT_INTERVAL	(8)

// 重力
#define YELLOW_ENEMY_GRAVITY (0.35f)

// 矩形判定サイズ
#define YELLOW_ENEMY_BOX_COLLISION_WIDTH	(38.0f)
#define YELLOW_ENEMY_BOX_COLLISION_HEIGHT	(47.0f)

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
		// 座標
		yellow->pos.x = 0;
		yellow->pos.y = 0;

		// 移動量
		yellow->move.x = 0;
		yellow->move.y = 0;

		// 状態
		yellow->active = false;

		yellow->isTurn = false;
		yellow->isAir = false;

		yellow->helmet = true;
		yellow->stun = false;
		yellow->stopStun = false;

		yellow->crush = false;
		yellow->strike = false;
		yellow->die = false;

		// タイマー
		yellow->stunTimer = 0;
		yellow->stopStunTimer = 0;
		yellow->strikeTimer = 0;
		yellow->crushTimer = 0;
		yellow->dieTimer = 0;

		// 再生アニメーション
		yellow->playAnim = YELLOW_ENEMY_ANIM_NONE;

		// 当たり判定サイズ
		yellow->boxCollision.width = YELLOW_ENEMY_WIDTH;
		yellow->boxCollision.height = YELLOW_ENEMY_HEIGHT;

		// アニメーション初期化
		for (int j = 0; j < YELLOW_ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&yellow->animation[j]);
		}
	}
}

void LoadYellowEnemy()
{
	//歩き(ヘルメット)
	int runHelmetHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy2_run.png");	 //yellow_enemy2_run.png
	//歩き(ヘルメット無し)
	int runHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_run.png");			 //yellow_enemy_run.png
	//スタン
	int stunHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_stun.png");		 //yellow_enemy_stun.png
	//スタン中
	int stunStopHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_stopstun.png"); //yellow_enemy_stopstun.png
	//クラッシュ
	int crushHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_die.png");		 //yellow_enemy_die.png
	//ストライク
	int strikeHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy_die2.png");		 //yellow_enemy_die2.png
	//Die(ヘルメット無し)
	int dieHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy2_die.png");			 //yellow_enemy2_die.png
	//Die(ヘルメット)
	int dieHelmetHandle = LoadGraph("Data/animation/Yellow_Enemy/yellow_enemy2_die2.png");	 //yellow_enemy2_die2.png

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++)
	{
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_RUN_1].handle = runHelmetHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_RUN_2].handle = runHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_STUN].handle = stunHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_STUNSTOP].handle = stunStopHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_CRUSH].handle = crushHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_STRIKE].handle = strikeHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_DIE].handle = dieHandle;
		g_YellowEnemyDate[i].animation[YELLOW_ENEMY_DIE2].handle = dieHelmetHandle;
	}
}

void StepYellowEnemy()
{
	if (g_IsYellowSelecting) return;

	YellowEnemyData* enemy = GetYellowEnemy();

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, enemy++)
	{
		if (!enemy->active) continue;

		// 前フレーム保存
		g_PravYellowEnemyDate[i] = *enemy;

		// 死亡中は停止
		if (enemy->crush || enemy->strike || enemy->die)
		{
			enemy->move.x = 0;
			enemy->move.y = 0;
			continue;
		}

		// 重力
		enemy->move.y += YELLOW_ENEMY_GRAVITY;

		// 横移動
		enemy->move.x = enemy->isTurn ?
			-YELLOW_ENEMY_MOVE_SPEED :
			YELLOW_ENEMY_MOVE_SPEED;
	}
}

void UpdateYellowEnemy()
{
	if (g_IsYellowSelecting) return;

	YellowEnemyData* yellow = g_YellowEnemyDate;

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active) continue;

		// 踏みつけ死亡
		if (yellow->crush)
		{
			yellow->crushTimer--;

			UpdateYellowEnemyAnimation(i);

			if (yellow->crushTimer <= 0)
			{
				yellow->active = false;
			}

			continue;
		}

		// 攻撃死亡
		if (yellow->strike)
		{
			yellow->strikeTimer--;

			UpdateYellowEnemyAnimation(i);

			if (yellow->strikeTimer <= 0)
			{
				yellow->active = false;
				AddScore(200);

				AddYellowStock(1);
			}

			continue;
		}

		// 落下死亡
		if (yellow->die)
		{
			yellow->dieTimer--;

			UpdateYellowEnemyAnimation(i);

			if (yellow->dieTimer <= 0)
			{
				yellow->active = false;
			}

			continue;
		}

		// スタン
		if (yellow->stun)
		{
			yellow->stunTimer--;

			UpdateYellowEnemyAnimation(i);

			if (yellow->stunTimer <= 0)
			{
				yellow->stun = false;
				yellow->stopStun = true;
				yellow->stopStunTimer = 40;

				//スタン後の停止アニメーション開始
				StartYellowEnemyAnimation(YELLOW_ENEMY_STUNSTOP, i);
			}

			continue;
		}

		// スタン後停止
		if (yellow->stopStun)
		{
			yellow->move.x = 0;
			yellow->move.y = 0;

			yellow->stopStunTimer--;

			UpdateYellowEnemyAnimation(i);

			if (yellow->stopStunTimer <= 0)
			{
				yellow->stopStun = false;
			}
		}
		
		// 移動
		yellow->pos.x += yellow->move.x;
		yellow->pos.y += yellow->move.y;

		// 攻撃判定
		if (IsAttackActive())
		{
			float attackx = 0.0f;
			float attacky = 0.0f;
			float attackw = 0.0f;
			float attackh = 0.0f;

			float enemyx = yellow->pos.x;
			float enemyy = yellow->pos.y;

			GetAttackRect(attackx, attacky, attackw, attackh);

			if (CheckSquareSquare(
				attackx, attacky, attackw, attackh,
				enemyx,
				enemyy,
				YELLOW_ENEMY_WIDTH,
				YELLOW_ENEMY_HEIGHT))
			{
				if (yellow->helmet)
				{
					yellow->helmet = false;
					yellow->stun = true;
					yellow->stunTimer = 30;

					StartYellowEnemyAnimation(YELLOW_ENEMY_STUN, i);
				}
				else
				{
					PlayerData* player = GetPlayer();

					if (player != nullptr)
					{
						if (player->pos.x < yellow->pos.x)
						{
							yellow->isTurn = true;
						}
						else if (player->pos.x > yellow->pos.x)
						{
							yellow->isTurn = false;
						}
					}

					yellow->strike = true;
					yellow->strikeTimer = 50;

					StartYellowEnemyAnimation(YELLOW_ENEMY_STRIKE, i);
				}
			}
		}

		UpdateYellowEnemyAnimation(i);
	}
}

void DrawYellowEnemy()
{
	YellowEnemyData* yellow = g_YellowEnemyDate;
	CameraData cam = GetCamera();

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active) continue;

		YellowEnemyAnimationType animType = yellow->playAnim;
		AnimationData* animData = &yellow->animation[animType];

		float drawX = yellow->pos.x - cam.posX;
		float drawY = yellow->pos.y - cam.posY;

		// 描画
		if (!yellow->isTurn)
		{
			DrawAnimation(animData, drawX, drawY, TRUE, FALSE);
		}
		else
		{
			DrawAnimation(animData, drawX, drawY, FALSE, FALSE);
		}
	}
}

void FinYellowEnemy()
{
	for (int i = 0; i < YELLOW_ENEMY_ANIM_MAX; i++)
	{
		DeleteGraph(g_YellowEnemyDate->animation[i].handle);
	}
}

YellowEnemyData* GetYellowEnemy()
{
	return g_YellowEnemyDate;
}

void CreateYellowEnemy(float posX, float posY, const EnemyParameter* param)
{
	YellowEnemyData* yellow = g_YellowEnemyDate;

	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		if (!yellow->active)
		{
			// アクティブ
			yellow->active = true;

			// アニメーション
			yellow->animation->handle = g_YellowEnemyDate->animation->handle;

			// 座標
			yellow->pos.x = posX;
			yellow->pos.y = posY;

			// 移動量
			yellow->move.x = YELLOW_ENEMY_MOVE_SPEED;
			yellow->move.y = 0.0f;

			// 初期状態
			yellow->helmet = true;
			yellow->stun = false;
			yellow->stopStun = false;
			yellow->crush = false;
			yellow->strike = false;
			yellow->die = false;

			// タイマー
			yellow->stunTimer = 0;
			yellow->stopStunTimer = 0;
			yellow->strikeTimer = 0;
			yellow->crushTimer = 0;
			yellow->dieTimer = 0;

			// 向き
			yellow->isTurn = false;

			// パラメータ
			yellow->param = param;

			break;
		}
	}
}

void PlayerKillYellowEnemy(int index)
{
	YellowEnemyData* enemy = &g_YellowEnemyDate[index];

	// すでに死亡処理中なら何もしない
	if (enemy->crush || enemy->strike || enemy->die) return;

	// ヘルメットあり
	if (enemy->helmet)
	{
		enemy->helmet = false;

		// スタン状態
		enemy->stun = true;
		enemy->stunTimer = 30;

		// 移動停止
		enemy->move.x = 0.0f;
		enemy->move.y = 0.0f;

		// スタンアニメーション
		StartYellowEnemyAnimation(YELLOW_ENEMY_STUN, index);

		return;
	}

	// ヘルメット無しなら死亡
	enemy->crush = true;
	enemy->crushTimer = 50;

	// 移動停止
	enemy->move.x = 0.0f;
	enemy->move.y = 0.0f;

	// 踏みつけアニメーション
	StartYellowEnemyAnimation(YELLOW_ENEMY_CRUSH, index);

	// スコア
	AddScore(200);

	AddYellowStock(1);
}

void PlayerKillYellowEnemyYellow(int index)
{
	YellowEnemyData* enemy = &g_YellowEnemyDate[index];

	if (enemy->crush || enemy->strike || enemy->die) return;

	enemy->die = true;
	enemy->dieTimer = 60;

	// 移動停止
	enemy->move.x = 0.0f;
	enemy->move.y = 0.0f;

	// SE
	PlaySE(SE_ENEMY_DEAD);

	// ヘルメット状態でアニメーション変更
	if (enemy->helmet)
	{
		StartYellowEnemyAnimation(YELLOW_ENEMY_DIE2, index);
	}
	else
	{
		StartYellowEnemyAnimation(YELLOW_ENEMY_DIE, index);
	}

	AddScore(200);

	AddYellowStock(1);
}

void YellowEnemyHitBlockX(MapChipData mapChipData, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];
	BlockData* block = mapChipData.data;

	float prevX = g_PravYellowEnemyDate[index].pos.x;
	float prevY = g_PravYellowEnemyDate[index].pos.y;

	if (CheckSquareSquare(yellow->pos.x, yellow->pos.y,
		YELLOW_ENEMY_BOX_COLLISION_WIDTH + 10.0f,
		YELLOW_ENEMY_BOX_COLLISION_HEIGHT - YELLOW_ENEMY_COLLISION_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (prevX + YELLOW_ENEMY_BOX_COLLISION_WIDTH <= block->pos.x)
		{
			yellow->isTurn = true;
		}
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			yellow->isTurn = false;
		}
	}
}

void YellowEnemyHitBlockY(MapChipData mapChipData, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];
	YellowEnemyData* prevYellow = &g_PravYellowEnemyDate[index];
	BlockData* block = mapChipData.data;

	yellow->isTurn = g_YellowEnemyDate->isTurn;

	if (CheckSquareSquare(yellow->pos.x, yellow->pos.y,
		YELLOW_ENEMY_BOX_COLLISION_WIDTH, YELLOW_ENEMY_BOX_COLLISION_HEIGHT,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 上から当たった
		if (prevYellow->pos.y + YELLOW_ENEMY_BOX_COLLISION_HEIGHT <= block->pos.y)
		{
			yellow->pos.y = block->pos.y - YELLOW_ENEMY_BOX_COLLISION_HEIGHT;
			yellow->move.y = 0.0f;
			yellow->isAir = false;
		}
		// 下から当たったか
		else if (prevYellow->pos.y >= block->pos.y + MAP_CHIP_HEIGHT)
		{
			yellow->pos.y = block->pos.y + MAP_CHIP_HEIGHT;
			yellow->move.y = 0.0f;
		}
	}
}

void YellowEnemyHitStopBlockX(MapChipData mapChipData, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];
	BlockData* block = mapChipData.data;

	float prevX = g_PravYellowEnemyDate[index].pos.x;
	float prevY = g_PravYellowEnemyDate[index].pos.y;

	if (CheckSquareSquare(
		yellow->pos.x,
		yellow->pos.y,
		YELLOW_ENEMY_WIDTH + 10.0f,
		YELLOW_ENEMY_HEIGHT - YELLOW_ENEMY_COLLISION_OFFSET,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// 左から当たった
		if (prevX + YELLOW_ENEMY_WIDTH <= block->pos.x)
		{
			yellow->isTurn = true;
		}
		// 右から当たった
		else if (prevX >= block->pos.x + MAP_CHIP_WIDTH)
		{
			yellow->isTurn = false;
		}
	}
}

void StartYellowEnemyAnimation(YellowEnemyAnimationType anim, int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];

	// アニメーション再生中なら何もしない
	if (anim == yellow->playAnim) return;

	// 再生中アニメーション設定
	yellow->playAnim = anim;

	// データを取得
	AnimationData* animData = &yellow->animation[anim];
	YellowEnemyAnimationParam animParam = YELLOW_ENEMY_ANIM_PARAM[anim];

	// 再生
	StartAnimation(animData,
		yellow->pos.x,
		yellow->pos.y,
		animParam.interval,
		animParam.frameNum,
		animParam.width,
		animParam.height,
		true);
}

void UpdateYellowEnemyAnimation(int index)
{
	YellowEnemyData* yellow = &g_YellowEnemyDate[index];

	if (yellow->die)
	{
		if (yellow->helmet)
		{
			StartYellowEnemyAnimation(YELLOW_ENEMY_DIE2, index);
		}
		else
		{
			StartYellowEnemyAnimation(YELLOW_ENEMY_DIE, index);
		}
	}
	else if (yellow->crush)
	{
		StartYellowEnemyAnimation(YELLOW_ENEMY_CRUSH, index);
	}
	else if (yellow->strike)
	{
		StartYellowEnemyAnimation(YELLOW_ENEMY_STRIKE, index);
	}
	else if (yellow->stun)
	{
		StartYellowEnemyAnimation(YELLOW_ENEMY_STUN, index);
	}
	else if (yellow->stopStun)
	{
		StartYellowEnemyAnimation(YELLOW_ENEMY_STUNSTOP, index);
	}
	else
	{
		if (yellow->helmet)
		{
			StartYellowEnemyAnimation(YELLOW_ENEMY_RUN_1, index);
		}
		else
		{
			StartYellowEnemyAnimation(YELLOW_ENEMY_RUN_2, index);
		}
	}

	AnimationData* animData = &yellow->animation[yellow->playAnim];
	UpdateAnimation(animData);
}

extern PlayerData g_PrevPlayerData;

bool UpdateYellowCrush(int enemyIndex)
{
	PlayerData* player = GetPlayer();
	YellowEnemyData* enemy = GetYellowEnemy();

	// 下向きに落ちていないなら踏めない
	if (player->move.y <= 0.0f) return false;

	YellowEnemyData* e = &enemy[enemyIndex];

	if (!e->active) return false;

	float playerBottomPrev = g_PrevPlayerData.pos.y + PLAYER_HEIGHT;
	float enemyTop = e->pos.y;

	// 上から来たか
	if (playerBottomPrev <= enemyTop)
	{
		PlayerKillYellowEnemy(enemyIndex);

		// SE
		PlaySE(SE_PLAYER_CRUSH);

		// プレイヤー跳ねる
		player->move.y = -8.0f;

		return true;
	}

	return false;
}