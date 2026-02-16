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
	g_TestHandle = LoadGraph("Data/Title/タイトルだぞ.webp");
}

void StartTitleScene()
{

}

void StepTitleScene()
{


	if (IsTriggerKey(この中))
	{
		ChangeScene(SCENE_SELECT);
	}
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	
}

void FinTitleScene()
{
	
}