#include "PlayScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"

/*
ここでやること
・「C」が押された時にTitleSceneに戻る
*/

int g_PlayBGHandle = -1;
int g_PlayerHandle = -1;
int g_DragonFlyHandle = -1;
int g_FliesHandle = -1;

void InitPlayScene()
{
	
}

void LoadPlayScene()
{
	g_PlayBGHandle = LoadGraph("Data/Play/BG.png");
	g_PlayerHandle = LoadGraph("Data/Player/Player.png");
	g_DragonFlyHandle = LoadGraph("Data/Enemy/DragonFly.png");
	g_FliesHandle = LoadGraph("Data/Enemy/Flies.png");
}

// スタートは始まる時に起こる処理
void StartPlayScene()
{

}

void StepPlayScene()
{
	//ここに「C」を押した時にタイトルに戻る
	if (IsTriggerKey(KEY_C))
	{
		ChangeScene(SCENE_TITLE);
	}
}

void UpdatePlayScene()
{

}

void DrawPlayScene()
{
	DrawGraph(0, 0, g_PlayBGHandle, TRUE);
	DrawGraph(300, 250, g_PlayerHandle, TRUE);
	DrawGraph(700, 472, g_DragonFlyHandle, TRUE);
	DrawGraph(825, 658, g_FliesHandle, TRUE);
}

void FinPlayScene()
{
	DeleteGraph(g_PlayBGHandle);
	DeleteGraph(g_PlayerHandle);
	DeleteGraph(g_DragonFlyHandle);
	DeleteGraph(g_FliesHandle);

}
