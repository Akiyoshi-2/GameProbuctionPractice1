#include "DxLib.h"
#include "SoundManager.h"

int g_BGMHandle[BGM_TYPE_MAX] = { 0 };
int g_SEHandle[SE_TYPE_MAX] = { 0 };

// BGMをロード
void LoadBGM()
{
//	g_BGMHandle[BGM_TUTORIAL] = LoadSoundMem("");
//	g_BGMHandle[BGM_TITLE] = LoadSoundMem("");
//	g_BGMHandle[BGM_PLAY] = LoadSoundMem("");
//	g_BGMHandle[BGM_GAME_CLEAR] = LoadSoundMem("");
	g_BGMHandle[BGM_GAME_OVER] = LoadSoundMem("Data/Sound/BGM/GameOver.mp3");

}

// BGMを再生
void PlayBGM(BGMType type)
{
	PlaySoundMem(g_BGMHandle[type], DX_PLAYTYPE_LOOP);
}

// BGMを止める
void SotpBGM(BGMType type)
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
//	g_SEHandle[] = LoadSoundMem("");
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
