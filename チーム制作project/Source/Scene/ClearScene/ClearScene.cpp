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
#include "../../Timer/Timer.h"

//タイトルに戻るの画像
int g_titleCHandle = -1;
//リザルトの画像
int g_ResultHandle = -1;
//クリアタイムの画像
int g_ClearTimeFontHandle = -1;

//ゲームクリアBGM　
int g_GameClearSEHandle = -1;

int g_ClearTimeHandle = 0;


void InitClearScene()
{
	g_titleCHandle = -1;
	g_ResultHandle = -1;
	g_ClearTimeFontHandle = -1;

	g_GameClearSEHandle = -1;

	g_ClearTimeHandle = 0;

}

void LoadClearScene()
{
	g_titleCHandle = LoadGraph("Data/Clear/画像/titleC.png");

	g_ResultHandle = LoadGraph("Data/Clear/画像/result.png");

	g_ClearTimeFontHandle = LoadGraph("Data/Clear/画像/ClearTime.png");

	g_ClearTimeHandle = LoadGraph("Data/Clear/画像/ClearTime.png");

	g_GameClearSEHandle = LoadSoundMem("Data/Sound/BGM/GameClear.ogg");
}

void StartClearScene()
{
	//	CreateScoreUI();

	//	CreateHighScoreUI();

	PlaySoundMem(g_GameClearSEHandle, DX_PLAYTYPE_BACK);

	if (!g_IsTutorialMode)
	{
		int life;
		int score;
		int yellow;

		LoadGameData(life, score, yellow);
		SaveGameData(life, 0, yellow);
	}

	g_ClearTimeFontHandle = CreateFontToHandle("Arial", 80, 3);

}

void StepClearScene()
{
	if (IsTriggerKey(KEY_F))
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

	DrawGraph(450, 250, g_ClearTimeHandle, TRUE);

	DrawGraph(535, 600, g_titleCHandle, TRUE);

	int clearTime = GetLimitTime() - GetRemainTime();

	int min = clearTime / 60;
	int sec = clearTime % 60;

	DrawFormatStringToHandle(
		1000,
		355,
		GetColor(255, 255, 255),
		g_ClearTimeFontHandle,
		": %d",
		clearTime
	);

	//DrawFormatString(600, 500, GetColor(255, 255, 255), "SCORE : %d", score);
}

void FinClearScene()
{
	DeleteGraph(g_titleCHandle);

	DeleteGraph(g_ResultHandle);

	DeleteGraph(g_ClearTimeFontHandle);


	DeleteSoundMem(g_GameClearSEHandle);

	DeleteSoundMem(g_ClearTimeHandle);
}


