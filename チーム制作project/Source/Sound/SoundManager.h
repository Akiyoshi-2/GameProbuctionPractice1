#pragma once

// BGM関連　////////
enum BGMType
{
	BGM_TUTORIAL,
	BGM_TITLE,
	BGM_STAGE_1,
	BGM_STAGE_2,
	BGM_STAGE_3,
	BGM_GAME_CLEAR,
	BGM_GAME_OVER,
	BGM_ALL_CLEAR,
	BGM_TYPE_MAX
};

// BGMをロード
void LoadBGM();
// BGMを再生
void PlayBGM(BGMType type);
// BGMを止める
void StopBGM(BGMType type);
// BGMを終了
void FinBGM();

////////////////////////////////


// SE関連 /////////////////////
enum SEType
{
	SE_PLAYER_DEAD,
	SE_ENEMY_DEAD,
	SE_PLAYER_ATTACK,
	SE_PLAYER_CRUSH,
	SE_TYPE_MAX
};

// SEをロード
void LoadSE();
// SEを再生
void PlaySE(SEType type);
// SEを終了
void FinSE();

//////////////////////////////