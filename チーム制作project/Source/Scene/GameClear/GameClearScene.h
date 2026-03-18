#pragma once
#include "../../Animation/Animation.h"

enum GameClearAnimationType
{
	RED_PLAYER_WIN,
	BLUE_PLAYER_WIN,
	YELLOW_PLAYER_WIN,
	CLEAR_ANIM_MAX,
	CLEAR_ANIM_NONE = -1
};

struct GameClearData
{
	AnimationData animation[CLEAR_ANIM_MAX];

	VECTOR pos[CLEAR_ANIM_MAX];

	GameClearAnimationType clearAnim;
};

void InitGameClear();
void LoadGameClear();
void StartGameClear();
void StepGameClear();
void UpdateGameClear();
void DrawGameClear();
void FinGameClear();