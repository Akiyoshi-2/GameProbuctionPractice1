#include "DxLib.h"
#include "Player.h"

// 入力
#include "../Input/Input.h"

// カメラ・アニメーション
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"

// 衝突判定・マップ
#include "../Collision/Collision.h"
#include "../Map/Block.h"

// 敵
#include "../Enemy/NormalEnemy/NormalEnemy.h"
#include "../Enemy/FullarmorEnemy/FullarmorEnemy.h"
#include "../Enemy/HelmetEnemy/HelmetEnemy.h"
#include "../Enemy/ShieldEnemy/ShieldEnemy.h"
#include "../Enemy/YellowEnemy/YellowEnemy.h"

// タイマー
#include "../Timer/Timer.h"

// 攻撃
#include "Attack/Attack.h"

//仮(多分必要になると思われるもの)
#include "../Scene/SceneManager.h"
#include "../Scene/TitleScene/TitleScene.h"

// アニメーション用パラメータ
struct PlayerAnimationParam
{
	int interval;
	int framNum;
	int width;
	int height;
};
const  PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
	//赤
	8, 3, 50, 50,	//攻撃
	30, 2, 50, 50,	//待機
	10, 2, 50, 50 , //移動
	8, 1, 50, 50,	//ジャンプ
	8, 1, 50, 50,	//落下
	8, 1, 50, 50,	//死
	//青
	30, 2, 50, 50,	//待機
	10, 2, 50, 50 , //移動
	8, 1, 50, 50,	//ジャンプ
	8, 1, 50, 50,	//落下
	8, 1, 50, 50,	//死
	//黄
	30, 2, 50, 50,	//待機
	10, 2, 50, 50, //移動
	8, 1, 50, 50,	//ジャンプ
	8, 1, 50, 50,	//落下
};

// ジャンプSEハンドル
int JumpSEHandle = -1;

// 現在のプレイヤー状態
PlayerData g_PlayerData = { 0 };

// 1フレーム前のプレイヤー状態
// 当たり判定や向き補正などで使用
PlayerData g_PrevPlayerData = { 0 };

// 重力
#define PLAYER_GRAVITY (0.35f)

// マップブロックとの当たり判定の微調整
#define PLAYER_MAP_COLLISION_OFFSET (0.05f)

// プレイヤー画像からの当たり判定開始位置
#define PLAYER_BOX_COLLISION_OFFSET_X (16)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)

// プレイヤー当たり判定サイズ
#define PLAYER_BOX_COLLISION_WIDTH (20)
#define PLAYER_BOX_COLLISiON_HEIGHT (25)

// キャラクタータイプ切り替えクールタイム
#define PLAYER_TYPE_CHANGE_COOLTIME (60)

// 黄色状態の持続時間（60 = 1秒）
#define PLAYER_YELLOW_TIME (900)

// 死亡後のリスポーン待機時間
//#define PLAYER_DIE_TIME (60)->includしたいため.hのほうに移動

// プレイヤーがこのY座標より下に落ちると死亡
#define STAGE0_DEAD_LINE (1200.0f)
#define STAGE1_DEAD_LINE (1450.0f)
#define STAGE2_DEAD_LINE (7000.0f)
#define STAGE3_DEAD_LINE (1900.0f)

//ジャンプ力
float GetPlayerJumpPower()
{
	switch (g_PlayerData.type)
	{
	case TYPE_YELLOW:
	case TYPE_BLUE:
		return 13.5f;	// 青・黄
	case TYPE_RED:

	default:
		return 12.0f;	// 赤
	}
}

float GetPlayerMoveSpeed()
{
	switch (g_PlayerData.type)
	{
	case TYPE_YELLOW:
		return 5.5f;

	case TYPE_BLUE:
	case TYPE_RED:
	default:
		return 4.0f;
	}
}

// このCPPでのみ使用する関数の宣言
// アニメーション更新
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);


void InitPlayer()
{
	// 座標
	g_PlayerData.pos.x = 0.0f;
	g_PlayerData.pos.y = 0.0f;

	// 移動量
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;

	// アニメーション状態
	g_PlayerData.playerAnim = PLAYER_ANIM_NONE;

	// プレイヤータイプ
	g_PlayerData.type = TYPE_RED;
	g_PlayerData.prevType = TYPE_RED;

	// クールタイム
	g_PlayerData.changeTypeCoolTime = 0;

	// 黄色状態時間
	g_PlayerData.yellowRemainTime = 0;

	// 攻撃状態
	g_PlayerData.isAttacking = false;

	// アニメーション初期化
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		InitAnimation(&g_PlayerData.animation[i]);
	}

	// 当たり判定初期化
	memset(&g_PlayerData.boxCollision, 0, sizeof(g_PlayerData.boxCollision));
}

void LoadPlayer()
{
	//赤
	g_PlayerData.animation[RED_PLAYER_ANIM_ATTACK].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Attack.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Idle.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/RedPlayer/RedPlayer_Run.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/RedPlayer/赤player_jump1.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/RedPlayer/赤player_fall1.png");
	g_PlayerData.animation[RED_PLAYER_ANIM_DIE].handle = LoadGraph("Data/animation/RedPlayer/赤player_die.png");

	//青
	g_PlayerData.animation[BLUE_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/BluePlayer/BluePlayer_Idle.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/BluePlayer/BluePlayer_Run.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/BluePlayer/青player_jump1.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/BluePlayer/青player_fall1.png");
	g_PlayerData.animation[BLUE_PLAYER_ANIM_DIE].handle = LoadGraph("Data/animation/BluePlayer/青player_die.png");

	//黄
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_IDLE].handle = LoadGraph("Data/animation/YellowPlayer/YellowPlayer_Idle.png");
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_RUN].handle = LoadGraph("Data/animation/YellowPlayer/YellowPlayer_Run.png");
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_FALL].handle = LoadGraph("Data/animation/YellowPlayer/黄player_fall1.png");
	g_PlayerData.animation[YELLOW_PLAYER_ANIM_JUMP].handle = LoadGraph("Data/animation/YellowPlayer/黄player_jump1.png");

	//SE
	JumpSEHandle = LoadSoundMem("Data/SE/サウンド案/se_jump.ogg");

}

void StartPlayer(int stage)
{
	g_PlayerData.active = true;

	// ステージ別スポーン位置
	switch (stage)
	{
	case 0: // チュートリアル
		g_PlayerData.pos.x = 100.0f;
		g_PlayerData.pos.y = 800.0f;
		break;

	case 1: // Stage1
		g_PlayerData.pos.x = 100.0f;
		g_PlayerData.pos.y = 1255.0f;
		break;

	case 2: // Stage2
		g_PlayerData.pos.x = 50.0f;
		g_PlayerData.pos.y = 6455.0f;
		break;

	case 3: // Stage3
		g_PlayerData.pos.x = 150.0f;
		g_PlayerData.pos.y = 305.0f;
		break;

	default:
		g_PlayerData.pos.x = 0.0f;
		g_PlayerData.pos.y = 0.0f;
		break;
	}

	// 移動状態リセット
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;

	g_PlayerData.isAir = false;
	g_PlayerData.canJump = true;

	// 当たり判定設定
	g_PlayerData.boxCollision.posX = PLAYER_BOX_COLLISION_OFFSET_X;
	g_PlayerData.boxCollision.posY = PLAYER_BOX_COLLISION_OFFSET_Y;
	g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
	g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISiON_HEIGHT;

	// 状態リセット
	g_PlayerData.type = TYPE_RED;
	g_PlayerData.prevType = TYPE_RED;

	g_PlayerData.changeTypeCoolTime = 0;
	g_PlayerData.yellowRemainTime = 0;

	// アニメーション開始
	StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
}

void StepPlayer()
{
	// 死亡中処理
	// リスポーン待機
	if (g_PlayerData.isDead)
	{
		g_PlayerData.deadTimer--;

		// 復活
		if (g_PlayerData.deadTimer <= 0)
		{
			StartPlayer(g_DecidedStage);
			g_PlayerData.isDead = false;
			g_PlayerData.active = true;

			// タイマーリセット
			ResetTimer();
		}

		// 死亡中は操作禁止
		return;
	}

	// 前フレームデータ保存
	// 当たり判定などで使用
	g_PrevPlayerData = g_PlayerData;

	// タイマー処理
	// キャラクター切替クールタイム
	if (g_PlayerData.changeTypeCoolTime > 0)
	{
		g_PlayerData.changeTypeCoolTime--;
	}

	// 移動初期化
	// 横移動リセット
	g_PlayerData.move.x = 0.0f;

	// 重力
	g_PlayerData.move.y += PLAYER_GRAVITY;

	// キャラクタータイプ切り替え（Rキー）
	// 赤 ⇔ 青
	if (IsTriggerKey(KEY_R) && g_PlayerData.changeTypeCoolTime <= 0)
	{
		// 黄色状態中は切替不可
		if (g_PlayerData.type == TYPE_YELLOW)
		{

		}
		else
		{
			// 青 → 赤
			if (g_PlayerData.type == TYPE_BLUE)
			{
				g_PlayerData.type = TYPE_RED;
				StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
			}
			// 赤 → 青
			else if (g_PlayerData.type == TYPE_RED)
			{
				g_PlayerData.type = TYPE_BLUE;
				StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
			}

			// 現在のタイプ保存
			g_PlayerData.prevType = g_PlayerData.type;

			// クールタイム開始
			g_PlayerData.changeTypeCoolTime = PLAYER_TYPE_CHANGE_COOLTIME;
		}
	}

	// 黄色状態発動（Qキー）
	if (IsTriggerKey(KEY_Q))
	{
		// 黄色状態でなければ発動
		if (g_PlayerData.type != TYPE_YELLOW)
		{
			// 現在タイプ保存
			g_PlayerData.prevType = g_PlayerData.type;

			// 黄色状態へ
			g_PlayerData.type = TYPE_YELLOW;
			g_PlayerData.yellowRemainTime = PLAYER_YELLOW_TIME;

			StartPlayerAnimation(YELLOW_PLAYER_ANIM_IDLE);
		}
	}

	// 黄色状態タイマー
	if (g_PlayerData.type == TYPE_YELLOW)
	{
		if (g_PlayerData.yellowRemainTime > 0)
		{
			g_PlayerData.yellowRemainTime--;
		}
		else
		{
			// 元のタイプへ戻る
			g_PlayerData.type = g_PlayerData.prevType;

			if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
			else if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
		}
	}

	// 赤プレイヤー攻撃（Fキー）
	if (g_PlayerData.type == TYPE_RED)
	{
		if (IsTriggerKey(KEY_F) && !g_PlayerData.isAttacking)
		{
			g_PlayerData.isAttacking = true;
			g_PlayerData.attackTimer = 24;

			// 攻撃アニメーション
			StartPlayerAnimation(RED_PLAYER_ANIM_ATTACK);

			// 攻撃判定開始
			StartAttack(
				g_PlayerData.pos.x + PLAYER_WIDTH / 2,
				g_PlayerData.pos.y,
				g_PlayerData.isTurn
			);
		}
	}

	// 横移動
	float moveSpeed = GetPlayerMoveSpeed();

	// 左移動
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.move.x = -moveSpeed;
		g_PlayerData.isTurn = true;
	}

	// 右移動
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.move.x = moveSpeed;
		g_PlayerData.isTurn = false;
	}

	// ジャンプ
	if (IsTriggerKey(KEY_SPACE) &&
		!g_PrevPlayerData.isAir &&
		g_PlayerData.canJump)
	{
		g_PlayerData.move.y = -GetPlayerJumpPower();
		g_PlayerData.isAir = true;
		g_PlayerData.canJump = false;

		// ジャンプSE
		PlaySoundMem(JumpSEHandle, DX_PLAYTYPE_BACK);
	}

	// キーを離したら再ジャンプ可能
	if (!IsInputKey(KEY_SPACE))
	{
		g_PlayerData.canJump = true;
	}

	// 空中状態設定
	g_PlayerData.isAir = true;
}

void UpdatePlayer()
{
	// プレイヤーが非アクティブなら処理しない（死亡状態など）
	if (!g_PlayerData.active) return;

	// プレイヤーの移動処理
	// 現在の速度(move)を位置(pos)に加算する
	g_PlayerData.pos.x += g_PlayerData.move.x;
	g_PlayerData.pos.y += g_PlayerData.move.y;

	// 落下死判定
	// ステージごとに設定されたラインより下に落ちたら死亡
	float deadLine = 0.0f;

	switch (g_DecidedStage)
	{
	case 0:
		deadLine = STAGE0_DEAD_LINE;
		break;

	case 1:
		deadLine = STAGE1_DEAD_LINE;
		break;

	case 2:
		deadLine = STAGE2_DEAD_LINE;
		break;

	case 3:
		deadLine = STAGE3_DEAD_LINE;
		break;

		// 想定外のステージ番号の場合
	default:
		deadLine = 2000.0f;
		break;
	}

	// プレイヤーが落下死ラインを超えたら死亡処理
	if (!g_PlayerData.isDead && g_PlayerData.pos.y > deadLine)
	{
		// プレイヤーを非アクティブにする
		g_PlayerData.active = false;

		// 死亡状態フラグ
		g_PlayerData.isDead = true;

		// 死亡タイマー設定
		g_PlayerData.deadTimer = PLAYER_DIE_TIME;

		// 移動停止
		g_PlayerData.move.x = 0.0f;
		g_PlayerData.move.y = 0.0f;

		// プレイヤータイプごとの死亡アニメーション開始
		if (g_PlayerData.type == TYPE_RED)
			StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
		else if (g_PlayerData.type == TYPE_BLUE)
			StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
	}

	// 攻撃処理更新
	// 攻撃の持続時間や状態を更新する
	UpdateAttack();
}

void DrawPlayer()
{
	// デバッグ：プレイヤー座標表示
	CameraData camera = GetCamera();
	DrawFormatString(
		0, 20,
		GetColor(255, 255, 255),
		"プレイヤーの座標＝[%f, %f]",
		g_PlayerData.pos.x,
		g_PlayerData.pos.y
	);

	// 現在のアニメーション取得
	PlayerAnimationType animType = g_PlayerData.playerAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];

	// アニメーション描画
	DrawAnimation(
		animData,
		g_PlayerData.pos.x - camera.posX,
		g_PlayerData.pos.y - camera.posY,
		g_PlayerData.isTurn
	);

}

void FinPlayer()
{
	// アニメーション画像解放
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		DeleteGraph(g_PlayerData.animation[i].handle);
	}

	// SE解放
	DeleteSoundMem(JumpSEHandle);
}

PlayerData* GetPlayer()
{
	return &g_PlayerData;
}

void StartPlayerAnimation(PlayerAnimationType anim)
{
	// すでに同じアニメーションなら何もしない
	if (anim == g_PlayerData.playerAnim) return;

	// 再生するアニメーションを設定
	g_PlayerData.playerAnim = anim;

	// アニメーションデータ取得
	AnimationData* animData = &g_PlayerData.animation[anim];

	// アニメーションのパラメータ取得
	PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

	// ループ設定
	// 攻撃アニメーションはループしない
	bool loop = true;

	if (anim == RED_PLAYER_ANIM_ATTACK)
	{
		loop = false;
	}

	// アニメーション再生開始
	StartAnimation(
		animData,
		g_PlayerData.pos.x,
		g_PlayerData.pos.y,
		animParam.interval,   // フレーム間隔
		animParam.framNum,    // フレーム数
		animParam.width,      // 画像幅
		animParam.height,     // 画像高さ
		loop                  // ループするか
	);
}

void UpdatePlayerAnimation()
{
	// 死亡中
	if (g_PlayerData.isDead)
	{
		AnimationData* animData =
			&g_PlayerData.animation[g_PlayerData.playerAnim];

		UpdateAnimation(animData);
		return;
	}

	// 攻撃中
	if (g_PlayerData.isAttacking)
	{
		g_PlayerData.attackTimer--;

		AnimationData* animData =
			&g_PlayerData.animation[g_PlayerData.playerAnim];

		UpdateAnimation(animData);

		if (g_PlayerData.attackTimer <= 0)
		{
			g_PlayerData.isAttacking = false;
			StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
		}

		return;
	}

	// 地上
	if (!g_PlayerData.isAir)
	{
		if (g_PlayerData.move.x != 0.0f)
		{
			// 移動
			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_RUN);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_RUN);
			else if (g_PlayerData.type == TYPE_YELLOW)
				StartPlayerAnimation(YELLOW_PLAYER_ANIM_RUN);
		}
		else
		{
			// 待機
			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
			else if (g_PlayerData.type == TYPE_YELLOW)
				StartPlayerAnimation(YELLOW_PLAYER_ANIM_IDLE);
		}
	}

	// 空中
	else
	{
		if (g_PlayerData.move.y < 0.0f)
		{
			// ジャンプ上昇
			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_JUMP);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_JUMP);
			else if (g_PlayerData.type == TYPE_YELLOW)
				StartPlayerAnimation(YELLOW_PLAYER_ANIM_JUMP);
		}
		else
		{
			// 落下
			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_FALL);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_FALL);
			else if (g_PlayerData.type == TYPE_YELLOW)
				StartPlayerAnimation(YELLOW_PLAYER_ANIM_FALL);
		}
	}

	// アニメーション更新
	AnimationData* animData =
		&g_PlayerData.animation[g_PlayerData.playerAnim];

	UpdateAnimation(animData);
}

void PlayerHitNormalBlockX(MapChipData mapChipData)
{
	// データ取得
	PlayerData player = g_PlayerData;        // プレイヤー情報コピー
	BlockData* block = mapChipData.data;     // 対象ブロック取得

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// 向きは前フレームのものを使用
	// （反転直後だと当たり判定がずれるため）
	player.isTurn = g_PrevPlayerData.isTurn;

	// 横方向の当たり判定チェック
	// Y座標は前フレームに戻して判定する
	player.pos.x = g_PlayerData.pos.x;
	player.pos.y = g_PrevPlayerData.pos.y;

	// プレイヤーの当たり判定ボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	// プレイヤーとブロックの矩形当たり判定
	if (CheckSquareSquare(
		x + POS_OFFSET,
		y + POS_OFFSET,
		w - SIZE_OFFSET,
		h - SIZE_OFFSET,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		// ブロックが有効なら処理
		if (block->active)
		{
			// 左からブロックに当たった場合
			if (player.move.x > 0.0f)
			{
				// プレイヤーを左へ押し戻す
				g_PlayerData.pos.x -= (x + w) - block->pos.x;
			}

			// 右からブロックに当たった場合
			else if (player.move.x < 0.0f)
			{
				// プレイヤーを右へ押し戻す
				g_PlayerData.pos.x +=
					(block->pos.x + MAP_CHIP_WIDTH) - x;
			}
		}
	}
}

void PlayerHitNormalBlockY(MapChipData mapChipData)
{
	// データ取得
	PlayerData player = g_PlayerData;        // プレイヤーデータコピー
	BlockData* block = mapChipData.data;     // 対象ブロック取得

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// 向きは前フレームのものを使用
	// （反転直後だと当たり判定がずれるため）
	player.isTurn = g_PrevPlayerData.isTurn;

	// プレイヤーの当たり判定ボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	// プレイヤーとブロックの矩形当たり判定
	if (CheckSquareSquare(
		x + POS_OFFSET,
		y + POS_OFFSET,
		w - SIZE_OFFSET,
		h - SIZE_OFFSET,
		block->pos.x,
		block->pos.y,
		MAP_CHIP_WIDTH,
		MAP_CHIP_HEIGHT))
	{
		if (block->active)
		{
			// 縦方向の移動を停止
			g_PlayerData.move.y = 0.0f;

			// 上からブロックに乗った場合
			if (player.move.y > 0.0f)
			{
				// プレイヤーをブロックの上へ押し出す
				g_PlayerData.pos.y -= (y + h) - block->pos.y;

				// 地面に接触したので空中フラグを解除
				g_PlayerData.isAir = false;
			}

			// 下からブロックにぶつかった場合
			else if (player.move.y < 0.0f)
			{
				// プレイヤーを下へ押し出す
				g_PlayerData.pos.y +=
					(block->pos.y + MAP_CHIP_WIDTH) - y;
			}
		}
	}
}

void PlayerHitThornBlockX(MapChipData mapChipData)
{
	// 対象ブロック取得
	BlockData* block = mapChipData.data;
	if (!block->active) return; // 非アクティブブロックは処理しない

	// 黄色プレイヤーは通常ブロック判定に移行
	if (g_PlayerData.type == TYPE_YELLOW)
	{
		PlayerHitNormalBlockX(mapChipData);
		return;
	}

	// 赤・青プレイヤーはトゲで即死
	PlayerData player = g_PlayerData;

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// 向きは前フレームのものを使用
	player.isTurn = g_PrevPlayerData.isTurn;

	// Y座標は前フレームを使用して横方向の判定
	player.pos.x = g_PlayerData.pos.x;
	player.pos.y = g_PrevPlayerData.pos.y;

	// 当たり判定用のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	// プレイヤーとブロックの矩形判定
	if (CheckSquareSquare(
		x + POS_OFFSET, y + POS_OFFSET,
		w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y,
		MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 死亡処理
		if (!g_PlayerData.isDead)
		{
			g_PlayerData.active = false;           // 操作不可
			g_PlayerData.isDead = true;            // 死亡フラグ
			g_PlayerData.deadTimer = PLAYER_DIE_TIME; // 復活タイマー
			g_PlayerData.move.x = 0.0f;            // 移動停止
			g_PlayerData.move.y = 0.0f;

			// 死亡アニメーション開始
			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
		}
	}
}

void PlayerHitThornBlockY(MapChipData mapChipData)
{
	// 対象ブロック取得
	BlockData* block = mapChipData.data;
	if (!block->active) return; // 非アクティブブロックは処理しない

	// 黄色プレイヤーは通常ブロック判定に移行
	if (g_PlayerData.type == TYPE_YELLOW)
	{
		PlayerHitNormalBlockY(mapChipData);
		return;
	}

	// 赤・青プレイヤーはトゲで即死
	PlayerData player = g_PlayerData;

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// 向きは前フレームのものを使用
	player.isTurn = g_PrevPlayerData.isTurn;

	// 当たり判定用のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	// プレイヤーとブロックの矩形判定
	if (CheckSquareSquare(
		x + POS_OFFSET, y + POS_OFFSET,
		w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y,
		MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 死亡処理
		if (!g_PlayerData.isDead)
		{
			g_PlayerData.active = false;           // 操作不可
			g_PlayerData.isDead = true;            // 死亡フラグ
			g_PlayerData.deadTimer = PLAYER_DIE_TIME; // 復活タイマー
			g_PlayerData.move.x = 0.0f;            // 移動停止
			g_PlayerData.move.y = 0.0f;

			// 死亡アニメーション開始
			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
		}
	}
}

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
	// X座標：向きによってボックス位置を反転
	x = player.isTurn ?
		player.pos.x + PLAYER_WIDTH - player.boxCollision.posX - player.boxCollision.width :
		player.pos.x + player.boxCollision.posX;

	// Y座標は固定
	y = player.pos.y + player.boxCollision.posY;

	// 幅・高さ
	w = player.boxCollision.width;
	h = player.boxCollision.height;
}

void PlayerHitEnemy()
{
	PlayerData* player = GetPlayer();

	// 黄色は無敵
	if (player->type == TYPE_YELLOW) return;

	// 既に死んでいたら無視
	if (player->isDead) return;

	//プレイヤー死亡
	player->isDead = true;
	player->deadTimer = PLAYER_DIE_TIME;
	player->active = false;

	player->move.x = 0;
	player->move.y = 0;

	// 死亡アニメーション
	if (player->type == TYPE_RED)
		StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
	else if (player->type == TYPE_BLUE)
		StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
}