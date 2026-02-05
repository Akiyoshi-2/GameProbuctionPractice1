#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

int g_TitleBGHandle = -1;
int g_TitleTextHandle = -1;

void InitTitleScene()
{

}

void LoadTitleScene()
{
	int bgHandle = LoadGraph("Data/Title/BG.png");
	int textHandle = LoadGraph("Data/Title/TitleText.png");

	g_TitleBGHandle = bgHandle;
	g_TitleTextHandle = textHandle;
}

void StartTitleScene()
{

}

void StepTitleScene()
{
	if (IsTriggerKey(KEY_Z))
	{
		ChangeScene(SCENE_PLAY);
	}
	else if (IsTriggerKey(KEY_X))
	{
		ChangeScene(SCENE_OPTION);
	}
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	DrawGraph(0, 0, g_TitleBGHandle, TRUE);
	DrawGraph(200, 200, g_TitleTextHandle, TRUE);
}

void FinTitleScene()
{
	DeleteGraph(g_TitleBGHandle);
	DeleteGraph(g_TitleTextHandle);
}