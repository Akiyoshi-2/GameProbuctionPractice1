#pragma once

// 入力ボタン定義
enum InputKey
{
	KEY_UP		= (1 << 0),	// 0x0000 0001
	KEY_DOWN	= (1 << 1),	// 0x0000 0010
	KEY_LEFT	= (1 << 2), // 0x0000 0100
	KEY_RIGHT	= (1 << 3), // 0x0000 1000
	KEY_SPACE	= (1 << 4), // 0x0001 0000
	KEY_F		= (1 << 5), // 0x0010 0000
	KEY_Q		= (1 << 6), // 0x0100 0000
	KEY_R		= (1 << 7), //カラーチェンジ
	KEY_P		= (1 << 8), //デバック用
};

// 関数のプロトタイプ宣言 
void InitInput();
void UpdateInput();
void DrawInput();
void FinInput();

bool IsInputKey(InputKey key);		// キー入力判定
bool IsTriggerKey(InputKey key);	// キー押した瞬間判定

bool Input_IsAnyKeyPush();	//Anyキーを1回だけ検出
void Input_Reset();