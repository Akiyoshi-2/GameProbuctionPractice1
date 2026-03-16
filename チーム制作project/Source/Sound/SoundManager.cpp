#include "DxLib.h"
#include "SoundManager.h"

int g_BGMHandle[BGM_TYPE_MAX] = { 0 };
int g_SEHandle[SE_TYPE_MAX] = { 0 };

// BGMをロード
void LoadBGM()
{
	g_BGMHandle[BGM_TUTORIAL] = LoadSoundMem("Data/Sound/BGM/Tutorial.mp3");
	g_BGMHandle[BGM_STAGE_1] = LoadSoundMem("Data/Sound/BGM/Stage1.mp3");
	g_BGMHandle[BGM_STAGE_2] = LoadSoundMem("Data/Sound/BGM/Stage2.mp3");
	g_BGMHandle[BGM_STAGE_3] = LoadSoundMem("Data/Sound/BGM/Stage3.mp3");
	g_BGMHandle[BGM_GAME_CLEAR] = LoadSoundMem("Data/Sound/BGM/GameClear.mp3");
	g_BGMHandle[BGM_GAME_OVER] = LoadSoundMem("Data/Sound/BGM/GameOver.mp3");

}

// BGMを再生
void PlayBGM(BGMType type)
{
	PlaySoundMem(g_BGMHandle[type], DX_PLAYTYPE_LOOP);
}

// BGMを止める
void StopBGM(BGMType type)
{
	StopSoundMem(g_BGMHandle[type]);
}

// BGMを終了
void FinBGM()
{
	for (int i = 0; i < BGM_TYPE_MAX; i++)
	{
		DeleteSoundMem(g_BGMHandle[i]);
	}
}

////////////////////

// SEをロード
void LoadSE()
{
	g_SEHandle[SE_PLAYER_DEAD] = LoadSoundMem("Data/Sound/SE/チーン1.mp3");
	g_SEHandle[SE_PLAYER_ATTACK] = LoadSoundMem("Data/Sound/SE/小パンチ.mp3");
}

// SEを再生
void PlaySE(SEType type)
{
	PlaySoundMem(g_SEHandle[type], DX_PLAYTYPE_BACK);
}

// SEを終了
void FinSE()
{
	for (int i = 0; i < SE_TYPE_MAX; i++)
	{
		DeleteSoundMem(g_SEHandle[i]);
	}
}
