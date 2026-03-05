#include "DxLib.h"
#include "GameOverScene.h"
#include "../SceneManager.h"
#include "../PlayScene/PlayScene.h"
#include "../../Input/Input.h"

// GamOver文字画像
int g_GameOverHandle = 0;
// リザルト画像
int g_ResultHandle = 0;

void InitGameOver()
{
	g_GameOverHandle = 0;

	g_ResultHandle = 0;
}

void LoadGameOver()
{
	g_GameOverHandle = LoadGraph("Data/GameOver/画像/ゲームオーバー.png");
	g_ResultHandle = LoadGraph("Data/GameOver/画像/result.png");
}

void StartGameOver()
{
//	CreateUITxet("");

//	CreateScoreUI();

//	CreateHighScoreUI();

//	PlayBGM();

}

void StepGameOver()
{
	if (IsTriggerKey(KEY_P))
	{
		ChangeScene(SCENE_TITLE);
	}

}

void UpdateGameOver()
{

}

void DrawGameOver()
{
	if (g_GameOverHandle != -1)
	{
		DrawGraph(250, 550, g_GameOverHandle, TRUE);
	}
	if (g_ResultHandle != -1)
	{
		DrawGraph(300, 450, g_ResultHandle, TRUE);
	}

//	DrawUIText();
}

void FinGameOver()
{
	DeleteGraph(g_GameOverHandle);
	DeleteGraph(g_ResultHandle);
}