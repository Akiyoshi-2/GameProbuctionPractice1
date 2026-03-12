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
//	CreateUIText(650.0f, 800.0f, "C Keyでタイトルに戻る");

//	CreateScoreUI();

//	CreateHighScoreUI();

//	PlayBGM();

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

		ChangeScene(SCENE_TITLE);
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

//	DrawUIText();

}

void FinClearScene()
{
	DeleteGraph(g_ClearHandle);
	DeleteGraph(g_ResultHandle);

	StopBGM(BGM_GAME_CLEAR);

}

