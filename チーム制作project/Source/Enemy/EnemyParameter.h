#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"

enum EnemyType
{
	NORMAL_ENEMY,
	HELMET_ENEMY,
	SHIELD_ENEMY,
	YELLOW_ENEMY,
	FULLARMOR_ENEMY,
};

struct EnemyParamer
{
	float EnemyDiePosX;
};

const EnemyParamer ENEMY_PARAMETER[] =
{
	4300.0f,
};

// Normal_EnemyŠÖ˜A
enum NomalEnemyAnimationType
{
	NOMAL_ENEMY_RUN,
	NOMAL_ENEMY_CRYSH,
	NOMAL_ENEMY_STRIKE,
	NOMAL_ENEMY_MAX,
	NOMAL_ENEMY_NONE = -1
};

struct NomalEnemyData
{
	bool isTurn;
	bool isAir;
	bool active;
	VECTOR pos;
	VECTOR move;
	// AnimationData animation[NOMAL_ENEMY_MAX];
	NomalEnemyAnimationType playAnim;
	BoxCollision boxCollision;
	const EnemyParamer* param;
	NomalEnemyData* data;
};