#include "DxLib.h"
#include "Attack.h"
#include "../../Camera/Camera.h"

#define ATTACK_WIDTH  (25)
#define ATTACK_HEIGHT (50)

#define ATTACK_OFFSET (40) 

static float g_AttackX = 0.0f;
static float g_AttackY = 0.0f;
static bool  g_AttackActive = false;
static bool  g_IsLeft = false;

void InitAttack()
{
    g_AttackActive = false;
}

void StartAttack(float x, float y, bool isLeft)
{
    g_AttackX = x;
    g_AttackY = y;
    g_IsLeft = isLeft;

    g_AttackActive = true;
}

void UpdateAttack()
{
    if (!g_AttackActive) return;
}

void DrawAttack()
{
    if (!g_AttackActive) return;

    CameraData camera = GetCamera();

    float drawX;

    if (g_IsLeft)
    {
        drawX = g_AttackX - ATTACK_WIDTH - ATTACK_OFFSET;
    }
    else
    {
        drawX = g_AttackX + ATTACK_OFFSET;
    }

    DrawBox(
        drawX - camera.posX,
        g_AttackY - camera.posY,
        drawX + ATTACK_WIDTH - camera.posX,
        g_AttackY + ATTACK_HEIGHT - camera.posY,
        GetColor(255, 0, 0),
        FALSE
    );
}