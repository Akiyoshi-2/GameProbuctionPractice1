#include "DxLib.h"
#include "ClearScene.h"
#include "../SceneManager.h"
#include "../../Input/Input.h"

//ゲームクリアの画像
int g_ClearHandle = -1;
//リザルトの画像
int g_ResultHandle = 0;




void InitClearScene()
{
	g_ClearHandle = -1;
	g_ResultHandle = 0;

}

void LoadClearScene()
{
	g_ClearHandle = LoadGraph("Data/Clear/画像/Clear.png");

	g_ResultHandle = LoadGraph("Data/Clear/画像/result.png");
}

void StartClearScene()
{

}

void StepClearScene()
{
	if (IsTriggerKey(KEY_C))
	{
		ChangeScene(SCENE_STAGE_2);
	}
	
	
}

void UpdateClearScene()
{

}

void DrawClearScene()
{
	if (g_ClearHandle != -1)
	{
		DrawGraph(0, 0, g_ClearHandle, TRUE);
	}
	if (g_ResultHandle != -1)
	{
		DrawGraph(0, 0, g_ResultHandle, TRUE);
	}

}

void FinClearScene()
{
	DeleteGraph(g_ClearHandle);
	DeleteGraph(g_ResultHandle);

}

