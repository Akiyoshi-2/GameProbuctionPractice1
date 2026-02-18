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

	g_MenuCursor = MENU_SELECT;
}


void LoadTitleScene()
{
	//画像
	g_TitleHandle = LoadGraph("Data/Title/Title.png");

	g_TitleUIData[TITLE_KEYUI].handle = LoadGraph("Data/Title/AnyKey Push.png");
	g_TitleUIData[MENU_SELECT].handle = LoadGraph("Data/Title/Menu/StageSelect.png");
	g_TitleUIData[MENU_TUTORIAL].handle = LoadGraph("Data/Title/Menu/Tutorial.png");
	g_TitleUIData[MENU_ARROW].handle = LoadGraph("Data/Title/矢印.png");

	//SE
	g_SEHandle = LoadSoundMem("Data/title/タイトル決定.ogg");
}

void StartTitleScene()
{
	g_TitleUIData[TITLE_KEYUI].pos = VGet(0.0f, 0.0f, 0.0f);
	g_TitleUIData[MENU_SELECT].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
	g_TitleUIData[MENU_TUTORIAL].pos = VGet(600.0f, MENU_TUTORIAL_Y, 0.0f);
	g_TitleUIData[MENU_ARROW].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
}


//一旦エラー直せ
void StepTitleScene()
{
    switch (g_TitlePhase)
    {
    case TITLE_PHASE_LOGO:
        // 点滅
        if (g_IsBlinking)
        {
            g_BlinkTimer++;
            if (g_BlinkTimer >= 5)
            {
                g_BlinkTimer = 0;
                g_DrawKeyUI = !g_DrawKeyUI;
            }
        }

        // AnyKey
        if (Input_IsAnyKeyPush())
        {
            // 点滅停止
            g_IsBlinking = false;
            g_DrawKeyUI = false;

            // 決定音
            PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);

            // 入力リセット
            Input_Reset();
        }
        break;

    }
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	DrawGraph(0, 0, g_TitleHandle, TRUE);

	for (int i = 0; i < TITLE_MUX; i++)
	{
		DrawGraph(
			(int)g_TitleUIData[i].pos.x,
			(int)g_TitleUIData[i].pos.y,
			g_TitleUIData[i].handle,
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