#pragma once

#define UI_TEXT_MAX (256)

struct UITextData
{
	bool active;
	char text[UI_TEXT_MAX];
	float posX;
	float posY;
};

// 各、関数
void InitUIText();
void LoadUIText();
void StepUIText();
void UpdateUIText();
void DrawUIText();
void FinUIText();

// UITextを生成する
UITextData* CreateUIText(float posX, float posY, const char* text);

// 指定したUITextに文字列を設定する
void SetUIText(UITextData* uiText, const char* text);

// 表示中のUITextを未使用にする
void ResetUIText();