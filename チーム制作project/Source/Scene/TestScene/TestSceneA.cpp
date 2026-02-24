#include "TestSceneA.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"
#include "../../Player/Player.h"
#include "../../Map/MapManager.h"
#include "../../Camera/Camera.h"
#include "../../Animation/Animation.h"

int g_TestHandle = -1;

void InitTestSceneA()
{
    g_TestHandle = -1;

    InitPlayer();
    InitMap();
}

void LoadTestSceneA()
{

    g_TestHandle = LoadGraph("Data/Title/Select/StageSelect.png");

    LoadPlayer();
    LoadMap();
}

void StartTestSceneA()
{
    StartPlayer();
    StartMap();
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
    UpdateCamera();
}

void DrawTestSceneA()
{
    CameraData cam = GetCamera();   //追加

    if (g_TestHandle != -1)
    {
        DrawGraph(
            (int)-cam.posX,         // カメラを引く
            (int)-cam.posY,
            g_TestHandle,
            TRUE
        );
    }

    DrawMap();      // 背景（ブロック）
    DrawPlayer();   // プレイヤー
    DrawCamera();   // デバッグ
}
void FinTestSceneA()
{
    if (g_TestHandle != -1)
    {
        DeleteGraph(g_TestHandle);
        g_TestHandle = -1;
    }

    FinPlayer();
    FinMap();
}