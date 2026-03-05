#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Scene/PlayScene/PlayScene.h"
#include "../Scene/SceneManager.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"
#include "../Map/Block.h"
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
	10, 3, 50, 50,	//攻撃
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

int JumpSEHandle = -1;

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

#define PLAYER_GRAVITY (0.35f)

#define PLAYER_MAP_COLLISION_OFFSET (0.05f)

#define PLAYER_BOX_COLLISION_OFFSET_X (16)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)

#define PLAYER_BOX_COLLISION_WIDTH (20)
#define PLAYER_BOX_COLLISiON_HEIGHT (25)

#define PLAYER_TYPE_CHANGE_COOLTIME (60) //キャラ切替クールタイム設定用
#define PLAYER_YELLOW_TIME (900) // 黄状態の時間経過 60=1秒

#define PLAYER_DIE_TIME (60)   // 1秒待ってから復活

//各ステージの落下死ライン
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
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
// アニメーション更新
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);


void InitPlayer()
{

	g_PlayerData.pos.x = 0.0f;
	g_PlayerData.pos.y = 0.0f;
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;
	g_PlayerData.playerAnim = PLAYER_ANIM_NONE;

	g_PlayerData.type = TYPE_RED;
	g_PlayerData.changeTypeCoolTime = 0;

	g_PlayerData.prevType = TYPE_RED;
	g_PlayerData.yellowRemainTime = 0;
	g_PlayerData.isAttacking = false;

	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		InitAnimation(&g_PlayerData.animation[i]);
	}

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

	// ステージごとのスポーン位置
	switch (stage)
	{
	case 0: // チュートリアル
		g_PlayerData.pos.x = 100.0f;
		g_PlayerData.pos.y = 900.0f;
		break;

	case 1: // Stage1
		g_PlayerData.pos.x = 100.0f;
		g_PlayerData.pos.y = 1255.0f;
		break;

	case 2: // Stage2
		g_PlayerData.pos.x = 50.0f;
		g_PlayerData.pos.y = 6555.0f;
		break;

	case 3: // Stage3
		g_PlayerData.pos.x = 150.0f;
		g_PlayerData.pos.y = 305.0f;
		break;

	default: //それ以外(あれば)
		g_PlayerData.pos.x = 0.0f;
		g_PlayerData.pos.y = 0.0f;
		break;
	}

	// 移動量・状態リセット
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;
	g_PlayerData.isAir = false;
	g_PlayerData.canJump = true;

	// 当たり判定
	g_PlayerData.boxCollision.posX = PLAYER_BOX_COLLISION_OFFSET_X;
	g_PlayerData.boxCollision.posY = PLAYER_BOX_COLLISION_OFFSET_Y;
	g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
	g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISiON_HEIGHT;

	// 状態初期化
	g_PlayerData.type = TYPE_RED;
	g_PlayerData.prevType = TYPE_RED;
	g_PlayerData.changeTypeCoolTime = 0;
	g_PlayerData.yellowRemainTime = 0;

	StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
}

void StepPlayer()
{
	//リスポーン待機
	if (g_PlayerData.isDead)
	{
		g_PlayerData.deadTimer--;

		if (g_PlayerData.deadTimer <= 0)
		{
			StartPlayer(g_DecidedStage);
			g_PlayerData.isDead = false;
		}

		return; // 死亡中は操作禁止
	}

	g_PrevPlayerData = g_PlayerData;

	if (g_PlayerData.changeTypeCoolTime > 0)
	{
		g_PlayerData.changeTypeCoolTime--;
	}

	g_PlayerData.move.x = 0.0f;

	g_PlayerData.move.y += PLAYER_GRAVITY;

	if (IsTriggerKey(KEY_R) && g_PlayerData.changeTypeCoolTime <= 0)
	{
		if (g_PlayerData.type == TYPE_YELLOW)
		{
		
		}
		else
		{
			// 青↔赤切り替え
			if (g_PlayerData.type == TYPE_BLUE)
			{
				g_PlayerData.type = TYPE_RED;
				StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
			}
			else if (g_PlayerData.type == TYPE_RED)
			{
				g_PlayerData.type = TYPE_BLUE;
				StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
			}

			// 前回タイプを記録
			g_PlayerData.prevType = g_PlayerData.type;

			// クールタイム開始
			g_PlayerData.changeTypeCoolTime = PLAYER_TYPE_CHANGE_COOLTIME;
		}
	}

	if (IsTriggerKey(KEY_Q))
	{
		if (g_PlayerData.type != TYPE_YELLOW)
		{
			g_PlayerData.prevType = g_PlayerData.type;

			g_PlayerData.type = TYPE_YELLOW;
			g_PlayerData.yellowRemainTime = PLAYER_YELLOW_TIME;

			StartPlayerAnimation(YELLOW_PLAYER_ANIM_IDLE);
		}
	}

	if (g_PlayerData.type == TYPE_YELLOW)
	{
		if (g_PlayerData.yellowRemainTime > 0)
		{
			g_PlayerData.yellowRemainTime--;
		}
		else
		{
			g_PlayerData.type = g_PlayerData.prevType;

			if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_IDLE);
			else if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_IDLE);
		}
	}

	//赤状態での攻撃
	if (g_PlayerData.type == TYPE_RED)
	{
		if (IsTriggerKey(KEY_F) && !g_PlayerData.isAttacking)
		{
			g_PlayerData.isAttacking = true;
			g_PlayerData.attackTimer = 30;
			StartPlayerAnimation(RED_PLAYER_ANIM_ATTACK);
		}
	}

	float moveSpeed = GetPlayerMoveSpeed();

	//左
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.move.x = -moveSpeed;
		g_PlayerData.isTurn = true;
	}
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.move.x = moveSpeed;
		g_PlayerData.isTurn = false;
	}

	if (IsTriggerKey(KEY_SPACE) &&
		!g_PrevPlayerData.isAir &&
		g_PlayerData.canJump)
	{
		g_PlayerData.move.y = -GetPlayerJumpPower();
		g_PlayerData.isAir = true;
		g_PlayerData.canJump = false;

		PlaySoundMem(JumpSEHandle, DX_PLAYTYPE_BACK);
	}

	// キーを離したら再度ジャンプ可能
	if (!IsInputKey(KEY_SPACE))
	{
		g_PlayerData.canJump = true;
	}

	g_PlayerData.isAir = true;

}

void UpdatePlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// 移動処理
	g_PlayerData.pos.x += g_PlayerData.move.x;
	g_PlayerData.pos.y += g_PlayerData.move.y;

	//// ===== 落下死判定 =====
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

	default:
		deadLine = 2000.0f;
		break;
	}

	if (!g_PlayerData.isDead && g_PlayerData.pos.y > deadLine)
	{
		g_PlayerData.isDead = true;
		g_PlayerData.deadTimer = PLAYER_DIE_TIME;
		g_PlayerData.move.x = 0.0f;
		g_PlayerData.move.y = 0.0f;

		if (g_PlayerData.type == TYPE_RED)
			StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
		else if (g_PlayerData.type == TYPE_BLUE)
			StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
	}
}



void DrawPlayer()
{
	CameraData camera = GetCamera();
	DrawFormatString(0, 100, GetColor(255, 255, 255), "プレイヤーの座標＝[%f, %f]", g_PlayerData.pos.x, g_PlayerData.pos.y);

	PlayerAnimationType animType = g_PlayerData.playerAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	DrawAnimation(animData, g_PlayerData.pos.x - camera.posX, g_PlayerData.pos.y - camera.posY, g_PlayerData.isTurn);
}

void FinPlayer()
{
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		DeleteGraph(g_PlayerData.animation[i].handle);
	}

	DeleteSoundMem(JumpSEHandle);
}

PlayerData* GetPlayer()
{
	return &g_PlayerData;
}

void StartPlayerAnimation(PlayerAnimationType anim)
{
	if (anim == g_PlayerData.playerAnim) return;

	g_PlayerData.playerAnim = anim;

	AnimationData* animData = &g_PlayerData.animation[anim];
	PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

	bool loop = true;
	if (anim == RED_PLAYER_ANIM_ATTACK)
	{
		loop = false;
	}

	StartAnimation(
		animData,
		g_PlayerData.pos.x,
		g_PlayerData.pos.y,
		animParam.interval,
		animParam.framNum,
		animParam.width,
		animParam.height,
		loop
	);
}

void UpdatePlayerAnimation()
{
	if (g_PlayerData.isDead)
	{
		AnimationData* animData =
			&g_PlayerData.animation[g_PlayerData.playerAnim];
		UpdateAnimation(animData);
		return;
	}

	// 赤の攻撃中は最優先
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
	PlayerData player = g_PlayerData;
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// ターンフラグは前回のものにしないと反転した分ずれる
	player.isTurn = g_PrevPlayerData.isTurn;

	// Y移動を戻し、横に当たっているかチェック
	player.pos.x = g_PlayerData.pos.x;
	player.pos.y = g_PrevPlayerData.pos.y;

	// 当たり判定のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (block->active)
		{
			// 左からあたったか
			if (player.move.x > 0.0f)
			{
				// 左に追い出す
				g_PlayerData.pos.x -= (x + w) - block->pos.x;
			}
			// 右からあたったか
			else if (player.move.x < 0.0f)
			{
				// 右に押し出す
				g_PlayerData.pos.x += (block->pos.x + MAP_CHIP_WIDTH) - x;
			}
		}
	}
}

void PlayerHitNormalBlockY(MapChipData mapChipData)
{
	PlayerData player = g_PlayerData;
	BlockData* block = mapChipData.data;

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// ターンフラグは前回のものにしないと反対した分ずれる
	player.isTurn = g_PrevPlayerData.isTurn;

	// 当たり判定のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	// まだ当たってないなら縦に当たっている
	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (block->active)
		{
			// Y移動量を0にする
			g_PlayerData.move.y = 0.0f;

			// 上から当たったか
			if (player.move.y > 0.0f)
			{
				// 上に押し出す
				g_PlayerData.pos.y -= (y + h) - block->pos.y;
				g_PlayerData.isAir = false;
			}
			// 下から当たったか
			else if (player.move.y < 0.0f)
			{
				// 下に押し出す
				g_PlayerData.pos.y += (block->pos.y + MAP_CHIP_WIDTH) - y;

			}
		}
	}
}

void PlayerHitThornBlockX(MapChipData mapChipData)
{
	BlockData* block = mapChipData.data;
	if (!block->active) return;

	// ===== 黄色は通常ブロック処理へ =====
	if (g_PlayerData.type == TYPE_YELLOW)
	{
		PlayerHitNormalBlockX(mapChipData);
		return;
	}

	// ===== 赤・青は即死 =====
	PlayerData player = g_PlayerData;

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	player.isTurn = g_PrevPlayerData.isTurn;
	player.pos.x = g_PlayerData.pos.x;
	player.pos.y = g_PrevPlayerData.pos.y;

	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	if (CheckSquareSquare(
		x + POS_OFFSET, y + POS_OFFSET,
		w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y,
		MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (!g_PlayerData.isDead)
		{
			g_PlayerData.isDead = true;
			g_PlayerData.deadTimer = PLAYER_DIE_TIME;
			g_PlayerData.move.x = 0.0f;
			g_PlayerData.move.y = 0.0f;

			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
		}
	}
}

void PlayerHitThornBlockY(MapChipData mapChipData)
{
	BlockData* block = mapChipData.data;
	if (!block->active) return;

	// ===== 黄色は通常ブロック処理へ =====
	if (g_PlayerData.type == TYPE_YELLOW)
	{
		PlayerHitNormalBlockY(mapChipData);
		return;
	}

	// ===== 赤・青は即死 =====
	PlayerData player = g_PlayerData;

	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	player.isTurn = g_PrevPlayerData.isTurn;

	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	if (CheckSquareSquare(
		x + POS_OFFSET, y + POS_OFFSET,
		w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y,
		MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		if (!g_PlayerData.isDead)
		{
			g_PlayerData.isDead = true;
			g_PlayerData.deadTimer = PLAYER_DIE_TIME;
			g_PlayerData.move.x = 0.0f;
			g_PlayerData.move.y = 0.0f;

			if (g_PlayerData.type == TYPE_RED)
				StartPlayerAnimation(RED_PLAYER_ANIM_DIE);
			else if (g_PlayerData.type == TYPE_BLUE)
				StartPlayerAnimation(BLUE_PLAYER_ANIM_DIE);
		}
	}
}

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
	x = player.isTurn ?
		player.pos.x + PLAYER_WIDTH - player.boxCollision.posX - player.boxCollision.width :
		player.pos.x + player.boxCollision.posX;
	y = player.pos.y + player.boxCollision.posY;
	w = player.boxCollision.width;
	h = player.boxCollision.height;
}