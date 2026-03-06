#include "DxLib.h"
#include "Attack.h"
#include "../../Camera/Camera.h"

#define ATTACK_WIDTH  (25)
#define ATTACK_HEIGHT (50)

#define ATTACK_OFFSET (40) 

float g_AttackX = 0.0f;
float g_AttackY = 0.0f;
bool  g_AttackActive = false;
bool  g_IsLeft = false;
int g_AttackTimer = 0;

AttackData g_Attack;

void InitAttack()
{
    g_Attack.x = 0;
    g_Attack.y = 0;
    g_Attack.w = 25;
    g_Attack.h = 50;
    g_Attack.active = false;
}

void StartAttack(float x, float y, bool isLeft)
{
    g_AttackX = x;
    g_AttackY = y - 80;
    g_IsLeft = isLeft;

    g_AttackActive = true;
    g_AttackTimer = 10;
}

void UpdateAttack()
{
    if (!g_AttackActive) return;

    g_AttackTimer--;

    if (g_AttackTimer <= 0)
    {
        g_AttackActive = false;
    }
}

void DrawAttack()
{
    if (!g_AttackActive) return;

    float ax;
    float ay;
    float aw;
    float ah;

    GetAttackRect(ax, ay, aw, ah);

    CameraData cam = GetCamera();

    DrawBox(
        ax - cam.posX,
        ay - cam.posY + 120,
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