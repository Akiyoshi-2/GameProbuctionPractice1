#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

int g_TestHandle = -1;

//シーン切り替えをAnyKeyで行えるようにする

void InitTitleScene()
{

}

void LoadTitleScene()
{
	g_TestHandle = LoadGraph("Data/Title/タイトルだぞ.png");
}

void StartTitleScene()
{

}

void StepTitleScene()
{
	if (Input_IsAnyKeyPush())
	{
		ChangeScene(SCENE_SELECT);
		Input_Reset();	//押しっぱなし防止
	}
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	DrawGraph(0, 0, g_TestHandle, TRUE);
}

void FinTitleScene()
{
	DeleteGraph(g_TestHandle);
}