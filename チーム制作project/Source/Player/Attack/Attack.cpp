#include "DxLib.h"
#include "Attack.h"
#include "../../Camera/Camera.h"

#define ATTACK_WIDTH   (25)
#define ATTACK_HEIGHT  (50)
#define ATTACK_OFFSET  (20)

// 攻撃の基準座標（プレイヤー位置）
float g_AttackX = 0.0f;
float g_AttackY = 0.0f;

// 攻撃状態
bool g_AttackActive = false;

// プレイヤーの向き
bool g_IsLeft = false;

// 攻撃持続時間
int g_AttackTimer = 0;


// 攻撃データ
AttackData g_Attack;

void InitAttack()
{
    g_Attack.x = 0;
    g_Attack.y = 0;
    g_Attack.w = ATTACK_WIDTH;
    g_Attack.h = ATTACK_HEIGHT;
    g_Attack.active = false;
}

void StartAttack(float x, float y, bool isLeft)
{
    // プレイヤー位置を基準に攻撃位置を設定
    g_AttackX = x;
    g_AttackY = y - 20;

    // プレイヤーの向き
    g_IsLeft = isLeft;

    // 攻撃開始
    g_AttackActive = true;

    // 攻撃持続フレーム
    g_AttackTimer = 10;
}

void UpdateAttack()
{
    if (!g_AttackActive) return;

    // タイマー減少
    g_AttackTimer--;

    // タイマー終了で攻撃終了
    if (g_AttackTimer <= 0)
    {
        g_AttackActive = false;
    }
}

// 攻撃描画（デバッグ用）
void DrawAttack()
{
    if (!g_AttackActive) return;

    float ax;
    float ay;
    float aw;
    float ah;

    // 攻撃範囲取得
    GetAttackRect(ax, ay, aw, ah);

    CameraData cam = GetCamera();

    // 攻撃当たり判定表示
    DrawBox(
        ax - cam.posX,
        ay - cam.posY + 20,
        ax + aw - cam.posX,
        ay + ah - cam.posY,
        GetColor(255, 0, 0),
        FALSE
    );
}

bool IsAttackActive()
{
    return g_AttackActive;
}

void GetAttackRect(float& x, float& y, float& w, float& h)
{
    float attackX;

    // 向きによって攻撃位置変更
    if (g_IsLeft)
    {
        attackX = g_AttackX - ATTACK_WIDTH - ATTACK_OFFSET;
    }
    else
    {
        attackX = g_AttackX + ATTACK_OFFSET;
    }

    x = attackX;
    y = g_AttackY;

    w = ATTACK_WIDTH;
    h = ATTACK_HEIGHT;
}