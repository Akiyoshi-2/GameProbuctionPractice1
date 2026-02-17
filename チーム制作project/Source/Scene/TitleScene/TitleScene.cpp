#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

int g_TItleHandle = -1;
int g_KeyUIHandle = -1;

//‰æ‘œØ‘Ö‚©‚ç

void InitTitleScene()
{

}

void LoadTitleScene()
{
	g_TItleHandle = LoadGraph("Data/Title/ƒ^ƒCƒgƒ‹‚¾‚¼.png");
}

void StartTitleScene()
{

}

void StepTitleScene()
{
	if (Input_IsAnyKeyPush())
	{
		ChangeScene(SCENE_SELECT);
		Input_Reset();	//‰Ÿ‚µ‚Á‚Ï‚È‚µ–h~
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