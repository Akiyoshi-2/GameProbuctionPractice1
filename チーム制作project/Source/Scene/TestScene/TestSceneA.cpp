#include "TestSceneA.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"
#include "../../Player/Player.h"


int g_TestHandle = -1;

void InitTestSceneA()
{
    g_TestHandle = -1;

    InitPlayer();
}

void LoadTestSceneA()
{

    g_TestHandle = LoadGraph("Data/Title/Select/StageSelect.png");

    LoadPlayer();
}

void StartTestSceneA()
{
    StartPlayer();
}

void StepTestSceneA()
{
    // Pでタイトルに戻る（デバッグ用）
    if (IsTriggerKey(KEY_P))
    {
        ChangeScene(SCENE_TITLE);
    }

    StepPlayer();
}

void UpdateTestSceneA()
{
    UpdatePlayer();
}

void DrawTestSceneA()
{

    if (g_TestHandle != -1)
    {
        DrawGraph(0, 0, g_TestHandle, TRUE);
    }

    DrawPlayer();
}

void FinTestSceneA()
{
    if (g_TestHandle != -1)
    {
        DeleteGraph(g_TestHandle);
        g_TestHandle = -1;
    }

    FinPlayer();
}