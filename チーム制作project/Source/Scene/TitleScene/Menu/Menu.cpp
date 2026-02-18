#include "Menu.h"
#include "../../../Input/Input.h"
#include "../../SceneManager.h"

int g_MenuBGHandle = -1;
MenuUIData g_MenuUIData[MENU_MAX] = {};

// 各メニューのY座標
const float MENU_SELECT_Y = 0.0f;
const float MENU_TUTORIAL_Y = 0.0f;

//矢印
int g_MenuCursor = MENU_SELECT;

void InitMenuScene()
{
	g_MenuCursor = MENU_SELECT;
}

void LoadMenuScene()
{
	g_MenuBGHandle = LoadGraph("Data/Title/Title.png");

	g_MenuUIData[MENU_SELECT].handle = LoadGraph("Data/Title/Menu/StageSelect.png");
	g_MenuUIData[MENU_TUTORIAL].handle = LoadGraph("Data/Title/Menu/Tutorial.png");
	g_MenuUIData[MENU_ARROW].handle = LoadGraph("Data/Title/矢印.png");
}

void StartMenuScene()
{
	g_MenuUIData[MENU_SELECT].pos = VGet(0.0f, 0.0f, 0.0f);
	g_MenuUIData[MENU_TUTORIAL].pos = VGet(0.0f, 0.0f, 0.0f);
	g_MenuUIData[MENU_ARROW].pos = VGet(0.0f, MENU_SELECT_Y, 0.0f);
}

void StepMenuScene()
{
	// ↓キー
	if (IsTriggerKey(KEY_DOWN))
	{
		g_MenuCursor++;
		if (g_MenuCursor > MENU_TUTORIAL)
		{
			g_MenuCursor = MENU_SELECT;
		}
	}

	// ↑キー
	if (IsTriggerKey(KEY_UP))
	{
		g_MenuCursor--;
		if (g_MenuCursor < MENU_SELECT)
		{
			g_MenuCursor = MENU_TUTORIAL;
		}
	}

	// 矢印位置更新
	if (g_MenuCursor == MENU_SELECT)
	{
		g_MenuUIData[MENU_ARROW].pos.y = MENU_SELECT_Y;
	}
	else if (g_MenuCursor == MENU_TUTORIAL)
	{
		g_MenuUIData[MENU_ARROW].pos.y = MENU_TUTORIAL_Y;
	}

	// 決定キー
	if (IsTriggerKey(KEY_F))
	{
		if (g_MenuCursor == MENU_SELECT)
		{
			ChangeScene(SCENE_SELECT);
		}
		/*else if (g_MenuCursor == MENU_TUTORIAL)
		{
			ChangeScene(SCENE_TUTORIAL);
		}*/
	}
}

void UpdateMenuScene()
{

}

void DrawMenuScene()
{
	DrawGraph(0, 0, g_MenuBGHandle, TRUE);

	for (int i = 0; i < MENU_MAX; i++)
	{
		DrawGraph((int)g_MenuUIData[i].pos.x, (int)g_MenuUIData[i].pos.y, g_MenuUIData[i].handle, TRUE);
	}
}

void FinMenuScene()
{
	DeleteGraph(g_MenuBGHandle);

	for (int i = 0; i < MENU_MAX; i++)
	{
		DeleteGraph(g_MenuUIData[i].handle);
	}
}
