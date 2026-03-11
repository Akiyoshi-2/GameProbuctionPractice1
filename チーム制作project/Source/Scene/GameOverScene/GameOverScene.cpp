#include "DxLib.h"
#include "GameOverScene.h"
#include "../SceneManager.h"
#include "../../Input/Input.h"
#include "../TitleScene/TitleScene.h"

int g_GameOverHandle = -1;
int g_RKyeHandle = -1;

int g_GameOverSEHandle = -1;

void InitGameOver()
{
    g_GameOverHandle = -1;
    g_RKyeHandle = -1;

    g_GameOverSEHandle = -1;
}

void LoadGameOver()
{
    g_GameOverHandle = LoadGraph("Data/GameOver/âÊëú/GameOver.png");
    g_RKyeHandle = LoadGraph("Data/GameOver/âÊëú/ReStart.png");

    g_GameOverSEHandle = LoadSoundMem("Data/Sound/BGM/GameOver.ogg");
}

void StartGameOver()
{
    PlaySoundMem(g_GameOverSEHandle, DX_PLAYTYPE_LOOP);
}

void StepGameOver()
{
    if (IsTriggerKey(KEY_R))
    {
        // É^ÉCÉgÉãÇ÷ñﬂÇÈ
        g_ReturnFromGame = true;

        StopSoundMem(g_GameOverSEHandle);

        ChangeScene(SCENE_TITLE);
    }
}

void UpdateGameOver()
{

}

void DrawGameOver()
{
    DrawGraph(0, 0, g_GameOverHandle, TRUE);
    //GameOverÇÃï∂éöÇÇ‡Ç§è≠Çµâ∫Ç…Ç∏ÇÁÇ∑
    DrawGraph(545, 500, g_RKyeHandle, TRUE);
}

void FinGameOver()
{
    DeleteGraph(g_GameOverHandle);

    DeleteGraph(g_RKyeHandle);

    DeleteSoundMem(g_GameOverSEHandle);
}