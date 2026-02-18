#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

//竹中　メニューの作成(画像サイズ調整から。Stage Selectに合わせる)

//画像
int g_TitleHandle = -1;
int g_KeyUIHandle = -1;

//SE関連
int g_SEHandle = -1;

//点滅関係
int g_BlinkTimer = 0;
bool g_DrawKeyUI = true;
bool g_IsBlinking = false;   // 点滅しているか


//シーン切り替えまでの時間作成
bool g_IsSceneChangeWait = false;
int g_SceneChangeTimer = 0;
const int SCENE_CHANGE_WAIT_TIME = 60;//フレーム

void InitTitleScene()
{
	g_BlinkTimer = 0;
	g_DrawKeyUI = true;

	g_IsBlinking = false;

	g_IsSceneChangeWait = false;
	g_SceneChangeTimer = 0;
}


void LoadTitleScene()
{
	//画像
	g_TitleHandle = LoadGraph("Data/Title/Title.png");
	g_KeyUIHandle = LoadGraph("Data/title/AnyKey Push.png");

	//SE
	g_SEHandle = LoadSoundMem("Data/title/タイトル決定.ogg");
}

void StartTitleScene()
{

}

void StepTitleScene()
{
	// 点滅処理（押された後のみ）
	if (g_IsBlinking)
	{
		g_BlinkTimer++;
		if (g_BlinkTimer >= 5)
		{
			g_BlinkTimer = 0;
			g_DrawKeyUI = !g_DrawKeyUI;
		}
	}

	// まだ待ちに入っていない場合
	if (!g_IsSceneChangeWait)
	{
		if (Input_IsAnyKeyPush())
		{
			//画像関連
			g_IsBlinking = true;
			g_IsSceneChangeWait = true;
			g_SceneChangeTimer = 0;
			g_BlinkTimer = 0;
			g_DrawKeyUI = true;
			Input_Reset();

			//SE
			PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
		}

	}
	else
	{
		// シーン切替待ち
		g_SceneChangeTimer++;
		if (g_SceneChangeTimer >= SCENE_CHANGE_WAIT_TIME)
		{
			ChangeScene(SCENE_MENU);
		}
	}
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	DrawGraph(0, 0, g_TitleHandle, TRUE);

	if (g_DrawKeyUI)
	{
		DrawGraph(0, 0, g_KeyUIHandle, TRUE);
	}
}

void FinTitleScene()
{
	DeleteGraph(g_TitleHandle);
	DeleteGraph(g_KeyUIHandle);

	DeleteSoundMem(g_SEHandle);
}