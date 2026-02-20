#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

//画像
int g_TitleHandle = -1;
TitleUIData g_TitleUIData[TITLE_MUX] = {};

//MenuのUI座標
const float MENU_SELECT_Y = 470.0f;
const float MENU_TUTORIAL_Y = 610.0f;

//SelectのUI座標
const float SELECT_STAGE_Y = 470.0;
const float SELECT_STAGE1_X = 170.0f;
const float SELECT_STAGE2_X = 670.0f;
const float SELECT_STAGE3_X = 1170.0f;

const float SELECT_BACK_Y = 700.0f;
const float SELECT_BACK_X = 700.0f;

const float SELECT_ARROW_X = 3.0f;
const float SELECT_ARROW_Y = 460.0f;

//矢印
int g_MenuCursor = MENU_SELECT;
int g_SelectCursor = SELECT_STAGE1;

//SE関連
int g_SEHandle = -1;
int g_MoveSEHandle = -1;

//点滅関係
int  g_BlinkTimer = 0;
bool g_DrawKeyUI = true;
bool g_IsBlinking = true;   // 点滅しているか

bool g_IsDecided = false; // AnyKey押されたか
bool g_IsShowMenu = false; // メニュー表示中か
bool g_IsStageSelectMode = false; //セレクト表示中か

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

	g_TitleUIData->stage = 0;

	g_MenuCursor = MENU_SELECT;

	g_SelectCursor = SELECT_STAGE1;
}

void LoadTitleScene()
{
	// 背景
	g_TitleHandle = LoadGraph("Data/Title/画像/Title.png");

	// UI
	g_TitleUIData[TITLE_KEYUI].handle = LoadGraph("Data/Title/画像/AnyKey Push.png");
	g_TitleUIData[MENU_SELECT].handle = LoadGraph("Data/Title/Menu/StageSelect.png");
	g_TitleUIData[MENU_TUTORIAL].handle = LoadGraph("Data/Title/Menu/Tutorial.png");
	g_TitleUIData[MENU_ARROW].handle = LoadGraph("Data/Title/Menu/矢印.png");
	g_TitleUIData[SELECT_STAGE1].handle = LoadGraph("Data/Title/Select/Stage1.png");
	g_TitleUIData[SELECT_STAGE2].handle = LoadGraph("Data/Title/Select/Stage2.png");
	g_TitleUIData[SELECT_STAGE3].handle = LoadGraph("Data/Title/Select/Stage3.png");
	g_TitleUIData[SELECT_ARROW].handle = LoadGraph("Data/Title/Menu/矢印.png");
	g_TitleUIData[SELECT_BACK].handle = LoadGraph("Data/Title/Select/Buck.png");

	// SE
	g_SEHandle = LoadSoundMem("Data/title/Sound/SE/決定(案1).ogg");
	g_MoveSEHandle = LoadSoundMem("Data/title/Sound/SE/カーソル移動案(1).ogg");
}

void StartTitleScene()
{
	g_TitleUIData[TITLE_KEYUI].pos = VGet(0.0f, 0.0f, 0.0f);
	g_TitleUIData[MENU_SELECT].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
	g_TitleUIData[MENU_TUTORIAL].pos = VGet(600.0f, MENU_TUTORIAL_Y, 0.0f);
	g_TitleUIData[MENU_ARROW].pos = VGet(420.0f, 458.0f, 0.0f);
	g_TitleUIData[SELECT_STAGE1].pos = VGet(SELECT_STAGE1_X, SELECT_STAGE_Y, 0.0f);
	g_TitleUIData[SELECT_STAGE2].pos = VGet(SELECT_STAGE2_X, SELECT_STAGE_Y, 0.0f);
	g_TitleUIData[SELECT_STAGE3].pos = VGet(SELECT_STAGE3_X, SELECT_STAGE_Y, 0.0f);
	g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X, SELECT_ARROW_Y, 0.0f);
	g_TitleUIData[SELECT_BACK].pos = VGet(SELECT_BACK_X, SELECT_BACK_Y, 0.0f);
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
	//========================================
	// メニュー操作
	//========================================
	if (g_IsShowMenu)
	{
		// ↓キー
		if (IsTriggerKey(KEY_DOWN))
		{
			if (g_MenuCursor == MENU_TUTORIAL)
			{
				g_MenuCursor = MENU_SELECT;
			}
			else
			{
				g_MenuCursor = MENU_TUTORIAL;
			}

			PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
		}

		// ↑キー
		if (IsTriggerKey(KEY_UP))
		{
			if (g_MenuCursor == MENU_SELECT)
			{
				g_MenuCursor = MENU_TUTORIAL;
			}
			else
			{
				g_MenuCursor = MENU_SELECT;
			}

			PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
		}
		// カーソルに応じて矢印位置更新
		if (g_MenuCursor == MENU_SELECT)
		{
			g_TitleUIData[MENU_ARROW].pos.y = 458.0f;
		}
		else if (g_MenuCursor == MENU_TUTORIAL)
		{
			g_TitleUIData[MENU_ARROW].pos.y = 598.0f;
		}

		// Fキー決定
		if (IsTriggerKey(KEY_F))
		{
			// チュートリアルは即シーン遷移
			if (g_MenuCursor == MENU_TUTORIAL)
			{
				g_TitleUIData->stage = 1;

				ChangeScene(TEST_SCENE_A);
			}
			// MENU_SELECT 確定
			else if (g_MenuCursor == MENU_SELECT)
			{
				g_IsShowMenu = false;        // メニュー非表示
				g_IsStageSelectMode = true; // ステージセレクト表示

				g_SelectCursor = SELECT_STAGE1;
				Input_Reset();

				PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
			}
		}
	}
	//========================================
	// セレクト操作
	//========================================
	if (g_IsStageSelectMode)
	{
		// →キー
		if (IsTriggerKey(KEY_RIGHT))
		{
			if (g_SelectCursor == SELECT_STAGE1)
			{
				g_SelectCursor = SELECT_STAGE2;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			else if (g_SelectCursor == SELECT_STAGE2)
			{
				g_SelectCursor = SELECT_STAGE3;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			else if (g_SelectCursor == SELECT_STAGE3)
			{
				g_SelectCursor = SELECT_STAGE1;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
	
		}

		// ←キー
		if (IsTriggerKey(KEY_LEFT))
		{
			if (g_SelectCursor == SELECT_STAGE1)
			{
				g_SelectCursor = SELECT_STAGE3;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			else if (g_SelectCursor == SELECT_STAGE2)
			{
				g_SelectCursor = SELECT_STAGE1;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			else if (g_SelectCursor == SELECT_STAGE3)
			{
				g_SelectCursor = SELECT_STAGE2;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}

		}

		// ↓キー
		if (IsTriggerKey(KEY_DOWN))
		{
			if (g_SelectCursor == SELECT_STAGE1)
			{
				g_SelectCursor = SELECT_BACK;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			else if (g_SelectCursor == SELECT_STAGE2)
			{
				g_SelectCursor = SELECT_BACK;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			else if (g_SelectCursor == SELECT_STAGE3)
			{
				g_SelectCursor = SELECT_BACK;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
		}

		// ↑キー
		if (IsTriggerKey(KEY_UP))
		{
			if (g_SelectCursor == SELECT_BACK)
			{
				g_SelectCursor = SELECT_STAGE1;

				PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
			}
			
		}

		//================================
		// 矢印位置更新
		//================================
		switch (g_SelectCursor)
		{
		case SELECT_STAGE1:
			g_TitleUIData[SELECT_ARROW].pos.x = SELECT_ARROW_X;
			g_TitleUIData[SELECT_ARROW].pos.y = SELECT_ARROW_Y;
			break;

		case SELECT_STAGE2:
			g_TitleUIData[SELECT_ARROW].pos.x = SELECT_ARROW_X + 500.0f;
			g_TitleUIData[SELECT_ARROW].pos.y = SELECT_ARROW_Y;
			break;

		case SELECT_STAGE3:
			g_TitleUIData[SELECT_ARROW].pos.x = SELECT_ARROW_X + 1000.0f;
			g_TitleUIData[SELECT_ARROW].pos.y = SELECT_ARROW_Y;
			break;

		case SELECT_BACK:
			g_TitleUIData[SELECT_ARROW].pos.x = SELECT_ARROW_X + 520.0f;
			g_TitleUIData[SELECT_ARROW].pos.y = SELECT_ARROW_Y + 210.0f;
			break;
		}

		// Fキー決定
		if (IsTriggerKey(KEY_F))
		{
			if (g_SelectCursor == SELECT_STAGE1)
			{
				//ChangeScene(SCENE_STAGE1);

				g_TitleUIData->stage = 1;
			}
			else if (g_SelectCursor == SELECT_STAGE2)
			{
				//ChangeScene(SCENE_STAGE2);

				g_TitleUIData->stage = 2;
			}
			else if (g_SelectCursor == SELECT_STAGE3)
			{
				//ChangeScene(SCENE_STAGE3);

				g_TitleUIData->stage = 3;
			}
			else if (g_SelectCursor == SELECT_BACK)
			{
				g_IsStageSelectMode = false;
				g_IsShowMenu = true;
				g_MenuCursor = MENU_SELECT;

				Input_Reset();

				PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
			}
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

	// ステージセレクト表示
	if (g_IsStageSelectMode)
	{
		DrawGraph(
			(int)g_TitleUIData[SELECT_STAGE1].pos.x,
			(int)g_TitleUIData[SELECT_STAGE1].pos.y,
			g_TitleUIData[SELECT_STAGE1].handle,
			TRUE
		);
		DrawGraph(
			(int)g_TitleUIData[SELECT_STAGE2].pos.x,
			(int)g_TitleUIData[SELECT_STAGE2].pos.y,
			g_TitleUIData[SELECT_STAGE2].handle,
			TRUE
		);
		DrawGraph(
			(int)g_TitleUIData[SELECT_STAGE3].pos.x,
			(int)g_TitleUIData[SELECT_STAGE3].pos.y,
			g_TitleUIData[SELECT_STAGE3].handle,
			TRUE
		);
		DrawGraph(
			(int)g_TitleUIData[SELECT_ARROW].pos.x,
			(int)g_TitleUIData[SELECT_ARROW].pos.y,
			g_TitleUIData[SELECT_ARROW].handle,
			TRUE
		);
		DrawGraph(
			(int)g_TitleUIData[SELECT_BACK].pos.x,
			(int)g_TitleUIData[SELECT_BACK].pos.y,
			g_TitleUIData[SELECT_BACK].handle,
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
	DeleteSoundMem(g_MoveSEHandle);
}