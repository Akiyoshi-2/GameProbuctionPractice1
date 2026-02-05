#include "OptionScene.h"
//#include "../Input/Input.h"
//#include "../SceneManager.h"

/*
ここでやるべきこと
・「C」が押されたときにTitleSceneに戻る
・サンプルの素材を流用してもいい
*/

int g_OptionBGHandle = -1;
int g_OptionUIHandle = -1;
int g_BGMUIHandle = -1;
int g_SEUIHndle = -1;
int g_GaugeHandle = -1;

void InitOptionScene()
{
}
void LoadOptionScene()
{
	g_OptionBGHandle = LoadGraph("Data/Option/BG.png");
	g_OptionBGHandle = LoadGraph("Data/Option/Option.png");
	g_BGMUIHandle = LoadGraph("Data/Option/BGM.png");
	g_SEUIHndle = LoadGraph("Data/Option/SE.png");
	g_GaugeHandle = LoadGraph("Data/Option/Gauge.png");
}
void StartOptionScene()
{
}
void StepOptionScene()
{
	//ここに「C」を押したときにタイトルに戻るように書く
	/*if (IsTriggerKey(KEY_C))
	{
		ChengeScene(SCENE_TITLE);
	}*/
}
void UpdateOptionScene()
{
}
void DrawOptionScene()
{
//#define SCREEN_WIDTH (1600)		// 画面の横幅
//#define SCREEN_HEIGHT (900)		// 画面の高さ
	DrawGraph(0, 0, g_OptionBGHandle, TRUE);
	DrawGraph(800, 150, g_OptionUIHandle, TRUE);
	DrawGraph(500, 400, g_BGMUIHandle, TRUE);
	DrawGraph(500, 600, g_SEUIHndle, TRUE);
	DrawGraph(200, 400, g_GaugeHandle, TRUE);
	DrawGraph(200, 600, g_GaugeHandle, TRUE);
}
void FinOptionScene()
{
	DeleteGraph(g_OptionBGHandle);
	DeleteGraph(g_OptionUIHandle);
	DeleteGraph(g_BGMUIHandle);
	DeleteGraph(g_SEUIHndle);
	DeleteGraph(g_GaugeHandle);
}