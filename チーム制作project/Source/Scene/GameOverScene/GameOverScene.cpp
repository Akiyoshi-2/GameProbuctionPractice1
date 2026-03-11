#include "DxLib.h"
#include "GameOverScene.h"
#include "../SceneManager.h"
#include "../PlayScene/PlayScene.h"
#include "../../Input/Input.h"
#include "../../Sound/SoundManager.h"
#include "../../UI/UIText.h"
#include "../../Player/Player.h"
#include "../TitleScene/TitleScene.h"

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
//	CreateUIText(650.0f, 800.0f, "P Keyでタイトルに戻る");


//	PlayBGM();

}

void StepGameOver()
{
	if (IsTriggerKey(KEY_P))
	{
		g_ReturnFromGame = true;
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