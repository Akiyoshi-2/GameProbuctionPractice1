#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

//竹中　メニューの作成(画像サイズ調整から。Stage Selectに合わせる)

//画像
int g_TitleHandle = -1;
TitleUIData g_TitleUIData[TITLE_MUX] = {};

//MenuのUI座標
const float MENU_SELECT_Y = 470.0f;
const float MENU_TUTORIAL_Y = 610.0f;
//矢印
int g_MenuCursor = MENU_SELECT;

//SE関連
int g_SEHandle = -1;

//点滅関係
int  g_BlinkTimer = 0;
bool g_DrawKeyUI = true;
bool g_IsBlinking = true;   // 点滅しているか

bool g_IsDecided = false; // AnyKey押されたか
bool g_IsShowMenu = false; // メニュー表示中か

//シーン切り替えまでの時間作成
bool g_IsSceneChangeWait = false;
int g_SceneChangeTimer = 0;
const int SCENE_CHANGE_WAIT_TIME = 60;//フレーム

void InitTitleScene()
{
	g_BlinkTimer = 0;

	g_DrawKeyUI = true;
	g_IsBlinking = false;

	g_IsDecided = false;

	g_IsShowMenu = false;

	g_IsSceneChangeWait = false;
	g_SceneChangeTimer = 0;

	g_MenuCursor = MENU_SELECT;
}

void LoadTitleScene()
{
	// 背景
	g_TitleHandle = LoadGraph("Data/Title/Title.png");

	// UI
	g_TitleUIData[TITLE_KEYUI].handle = LoadGraph("Data/Title/AnyKey Push.png");
	g_TitleUIData[MENU_SELECT].handle = LoadGraph("Data/Title/Menu/StageSelect.png");
	g_TitleUIData[MENU_TUTORIAL].handle = LoadGraph("Data/Title/Menu/Tutorial.png");
	g_TitleUIData[MENU_ARROW].handle = LoadGraph("Data/Title/Menu/矢印.png");

	// SE
	g_SEHandle = LoadSoundMem("Data/title/タイトル決定.ogg");
}

void StartTitleScene()
{
	g_TitleUIData[TITLE_KEYUI].pos = VGet(0.0f, 0.0f, 0.0f);
	g_TitleUIData[MENU_SELECT].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
	g_TitleUIData[MENU_TUTORIAL].pos = VGet(600.0f, MENU_TUTORIAL_Y, 0.0f);
	g_TitleUIData[MENU_ARROW].pos = VGet(420.0f, MENU_SELECT_Y - 12.0f, 0.0f);
}
void StepTitleScene()
{
    // AnyKey待ち
	if (!g_IsDecided)
	{
		if (Input_IsAnyKeyPush())
		{
			g_IsDecided = true;

			// 押した瞬間だけ点滅開始
			g_IsBlinking = true;
			g_DrawKeyUI = true;
			g_BlinkTimer = 0;

			// 待ち開始
			g_IsSceneChangeWait = true;
			g_SceneChangeTimer = 0;

			PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
			Input_Reset();
		}
	}
    // 点滅処理（押した後だけ）
	if (g_IsBlinking)
	{
		g_BlinkTimer++;
		if (g_BlinkTimer >= 5)
		{
			g_BlinkTimer = 0;
			g_DrawKeyUI = !g_DrawKeyUI;
		}
	}
    // 待ち時間 → メニュー表示
	if (g_IsSceneChangeWait)
	{
		g_SceneChangeTimer++;

		if (g_SceneChangeTimer >= SCENE_CHANGE_WAIT_TIME)
		{
			// AnyKeyを完全に消す
			g_IsBlinking = false;
			g_DrawKeyUI = false;

			// メニュー表示
			g_IsShowMenu = true;

			g_IsSceneChangeWait = false;
		}
	}
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	// 背景は常に表示
	DrawGraph(0, 0, g_TitleHandle, TRUE);

	// AnyKey表示
	if (g_DrawKeyUI)
	{
		DrawGraph(
			(int)g_TitleUIData[TITLE_KEYUI].pos.x,
			(int)g_TitleUIData[TITLE_KEYUI].pos.y,
			g_TitleUIData[TITLE_KEYUI].handle,
			TRUE
		);
	}

	// メニュー表示
	if (g_IsShowMenu)
	{
		DrawGraph(
			(int)g_TitleUIData[MENU_SELECT].pos.x,
			(int)g_TitleUIData[MENU_SELECT].pos.y,
			g_TitleUIData[MENU_SELECT].handle,
			TRUE
		);

		DrawGraph(
			(int)g_TitleUIData[MENU_TUTORIAL].pos.x,
			(int)g_TitleUIData[MENU_TUTORIAL].pos.y,
			g_TitleUIData[MENU_TUTORIAL].handle,
			TRUE
		);

		DrawGraph(
			(int)g_TitleUIData[MENU_ARROW].pos.x,
			(int)g_TitleUIData[MENU_ARROW].pos.y,
			g_TitleUIData[MENU_ARROW].handle,
			TRUE
		);
	}
}

void FinTitleScene()
{
	DeleteGraph(g_TitleHandle);

	for (int i = 0; i < TITLE_MUX; i++)
	{
		DeleteGraph(g_TitleUIData[i].handle);
	}

	DeleteSoundMem(g_SEHandle);
}