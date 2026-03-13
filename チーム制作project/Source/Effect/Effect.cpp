#include "DxLib.h"
#include "Effect.h"
#include "../Camera/Camera.h"

#define EFFECT_MAX	100

struct EffectData
{
	bool active;
	int handle;
	int nowFrame;
	int frameNum;
	int width;
	int height;
	int timer;
	int interval;
	float posX;
	float posY;
};
EffectData g_EffectData[EFFECT_MAX] = { 0 };

int g_EffectHandle[EFFECT_TYPE_MAX];

void InitEffect()
{
	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		effect->active = false;
		effect->handle = 0;
		effect->nowFrame = 0;
		effect->width = 0;
		effect->height = 0;
		effect->timer = 0;
		effect->posX = 0.0f;
		effect->posY = 0.0f;
	}
}

void LoadEffect()
{
	g_EffectHandle[PLAYER_CHANGE_YELLOW] = LoadGraph("Data/animation/effect/player_effect.png");
}

void StepEffect()
{
	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (!effect->active) continue;

		if (effect->timer <= 0)
		{
			effect->nowFrame++;

			effect->timer = effect->interval;

			if (effect->nowFrame >= effect->frameNum)
			{
				effect->active = false;

				continue;
			}
		}

		effect->timer--;
	}
}

void UpdataEffect()
{

}

void DrawEffect()
{
	EffectData* effect = g_EffectData;
	CameraData camera = GetCamera();

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (!effect->active) continue;

		int frameX = effect->width * effect->nowFrame;

		int DrawX = effect->posX - camera.posX;
		int DrawY = effect->posY - camera.posY;

		DrawRectGraph(DrawX, DrawY, frameX, 0, effect->width, effect->height, effect->handle, TRUE);
	}
}

void FinEffect()
{
	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
	{
		DeleteGraph(g_EffectHandle[i]);
	}
}

void StartEffect(Effecttype type, float posX, float posY, int interval)
{
	EffectMasterParameter param = EFFECT_MASTER_PARAM[type];

	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (!effect->active)
		{
			effect->active = true;

			effect->posX = posX;
			effect->posY = posY;

			effect->interval = interval;

			effect->handle = g_EffectHandle[type];

			effect->nowFrame = 0;

			effect->frameNum = param.frameNum;

			effect->width = param.width;
			effect->height = param.height;

			break;
		}
	}
}