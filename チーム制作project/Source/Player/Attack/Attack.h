#pragma once

struct AttackData
{
    float x;
    float y;
    float w;
    float h;
    bool active;
};

void InitAttack();
void UpdateAttack();
void DrawAttack();

void StartAttack(float x, float y, bool isLeft);

bool IsAttackActive();
void GetAttackRect(float& x, float& y, float& w, float& h);