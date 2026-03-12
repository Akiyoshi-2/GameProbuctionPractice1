#include "DxLib.h"
#include "ClearScene.h"
#include "../SceneManager.h"
#include "../../Input/Input.h"
#include "../PlayScene/PlayScene.h"
#include "../../Sound/SoundManager.h"
#include "../../UI/UIText.h"
#include "../../Enemy/Goal/Goal.h"
#include "../TitleScene/TitleScene.h"
#include "../../Score/Score.h"
#include "../../SaveData/SaveData.h"
#include "../../Player/Player.h"

//タイトルに戻るの画像
int g_titleCHandle = -1;
//リザルトの画像
int g_ResultHandle = -1;

//ゲームクリアBGM　
int g_GameClearSEHandle = -1;


void InitClearScene()
{
	g_titleCHandle = -1;
	g_ResultHandle = -1;

	g_GameClearSEHandle = -1;

}

void LoadClearScene()
{
	g_titleCHandle = LoadGraph("Data/Clear/画像/titleC.png");

	g_ResultHandle = LoadGraph("Data/Clear/画像/result.png");

	g_GameClearSEHandle = LoadSoundMem("Data/Sound/BGM/GameClear.ogg");
}

void StartClearScene()
{
//	CreateScoreUI();

//	CreateHighScoreUI();

	PlaySoundMem(g_GameClearSEHandle, DX_PLAYTYPE_BACK);

	int life;
	int score;

	LoadGameData(life, score);

	// Lifeはそのまま、Scoreだけ0にする
	SaveGameData(life, 0);
}

void StepClearScene()
{
	if (IsTriggerKey(KEY_C))
	{
		g_ReturnFromGame = true;

		StopSoundMem(g_GameClearSEHandle);

		ChangeScene(SCENE_TITLE);
	}

}
void UpdateClearScene()
{

}

void DrawClearScene()
{
	DrawGraph(0, 0, g_ResultHandle, TRUE);

	DrawGraph(545, 700, g_titleCHandle, TRUE);

}

void FinClearScene()
{
	DeleteGraph(g_titleCHandle);

	DeleteGraph(g_ResultHandle);

	DeleteSoundMem(g_GameClearSEHandle);

}


