#include "DxLib.h"
#include "Input.h"

// 入力ビットフラグ
int g_InputState = 0;
// 前回の入力ビット
int g_PrevInputState = 0;

static char key[256];		//今の状態
static char oldkey[256];	//前フレームの状態

void InitInput()
{
	g_InputState = 0;
	g_PrevInputState = 0;
}

void UpdateInput()
{
	// 前回の入力を覚えておく
	g_PrevInputState = g_InputState;

	// 入力状態をクリア
	g_InputState = 0;

	// 入力状態をビットフラグで設定
	if (CheckHitKey(KEY_INPUT_UP))
	{
		g_InputState |= KEY_UP;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		g_InputState |= KEY_DOWN;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		g_InputState |= KEY_LEFT;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		g_InputState |= KEY_RIGHT;
	}
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		g_InputState |= KEY_SPACE;
	}
	if (CheckHitKey(KEY_INPUT_F))
	{
		g_InputState |= KEY_F;
	}
	if (CheckHitKey(KEY_INPUT_Q))
	{
		g_InputState |= KEY_Q;
	}
}

void DrawInput()
{
}

void FinInput()
{
}

bool IsInputKey(InputKey key)
{
	return g_InputState & key;
}

bool IsTriggerKey(InputKey key)
{
	return (g_InputState & key) && !(g_PrevInputState & key);
}

bool Input_IsAnyKeyPush()
{
	// 前フレーム保存
	memcpy(oldkey, key, sizeof(key));
	// 今フレーム取得
	GetHitKeyStateAll(key);
	for (int i = 0; i < 256; i++)
	{
		if (key[i] == 1 && oldkey[i] == 0)
		{
			return true;
		}
	}
}
void Input_Reset()
{
	memset(key, 0, sizeof(key));
	memset(oldkey, 0, sizeof(oldkey));
}