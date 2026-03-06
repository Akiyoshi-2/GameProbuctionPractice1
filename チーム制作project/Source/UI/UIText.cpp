#include "DxLib.h"
#include "UIText.h"
#include <string.h>

#define UI_TEXT_MAX (32)

// UITextデータ
UITextData g_UITextData[UI_TEXT_MAX] = { 0 };

// プレイシーンで使うフォントのハンドル
int g_PlayerSceneFontHandle = 0;

void InitUIText()
{
	UITextData* uiText = g_UITextData;
	for (int i = 0; i < UI_TEXT_MAX; i++, uiText++)
	{
		uiText->active = false;
		uiText->posX = 0.0f;
		uiText->posY = 0.0f;
		memset(uiText->text, 0, sizeof(uiText->text));
	}
}

void LoadUIText()
{
//	g_PlayerSceneFontHandle = CreateFontToHandle();
}

void StepUIText()
{
}

void UpdateUIText()
{
}

void DrawUIText()
{
	UITextData* uiText = g_UITextData;
	for (int i = 0; i < UI_TEXT_MAX; i++, uiText++)
	{
		if (!uiText->active) continue;

		// 描画
		DrawStringToHandle((int)uiText-> posX, (int)uiText->posY,
			uiText->text, GetColor(255, 255, 255),
			g_PlayerSceneFontHandle);
	}
}

void FinUIText()
{
}

// UITextを生成
UITextData* CreateUIText(float posX, float posY, const char* text)
{
	UITextData* uiText = g_UITextData;
	for (int i = 0; i < UI_TEXT_MAX; i++, uiText++)
	{
		// 未使用枠を探す
		if (!uiText->active)
		{
			// アクティブ
			uiText->active = true;

			// 座標
			uiText->posX = posX;
			uiText->posY = posY;

			// テキスト
			strcpy_s(uiText->text, text);

			return uiText;
		}
	}

	// 空枠がなかった
	return nullptr;
}

// 指定したUITextに文字列を設定
void SetUIText(UITextData* uiText, const char* text)
{
	strcpy_s(uiText->text, text);
}

// 表示中のUITextを未使用にする
void ResetUIText()
{
	UITextData* uiText = g_UITextData;
	for (int i = 0; i < UI_TEXT_MAX; i++, uiText++)
	{
		uiText->active = false;
	}
}