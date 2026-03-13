#pragma once


enum Effecttype
{
	PLAYER_CHANGE_YELLOW,
	EFFECT_TYPE_MAX,
};

struct EffectMasterParameter
{
	int frameNum;
	int width;
	int height;
};

const EffectMasterParameter EFFECT_MASTER_PARAM[EFFECT_TYPE_MAX] =
{
	6, 50, 50,
};

void InitEffect();
void LoadEffect();
void StepEffect();
void UpdataEffect();
void DrawEffect();
void FinEffect();

void StartEffect(Effecttype type, float posX, float posY, int interval);