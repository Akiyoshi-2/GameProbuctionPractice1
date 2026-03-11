#include "DxLib.h"
#include "GameOverScene.h"
#include "../SceneManager.h"
#include "../PlayScene/PlayScene.h"
#include "../../Input/Input.h"
#include "../../Sound/SoundManager.h"
#include "../../UI/UIText.h"
#include "../../Player/Player.h"

// GamOver文字画像
int g_GameOverHandle = 0;

void InitGameOver()
{
	g_GameOverHandle = 0;

	
}

void LoadGameOver()
{
	g_GameOverHandle = LoadGraph("Data/GameOver/画像/ゲームオーバー.png");
	
}

void StartGameOver()
{
//	CreateUITxet("");


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
	

//	DrawUIText();
}

void FinGameOver()
{
	DeleteGraph(g_GameOverHandle);

	StopBGM(BGM_GAME_OVER);
	
}