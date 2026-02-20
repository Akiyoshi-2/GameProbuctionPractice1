#include "TestSceneA.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

//====================================
// グローバル変数
//====================================
int g_TestHandle = -1;

//====================================
// 初期化
//====================================
void InitTestSceneA()
{
    g_TestHandle = -1;
}

//====================================
// ロード
//====================================
void LoadTestSceneA()
{
    // ★ 問題の行（ここで必ずロード）
    g_TestHandle = LoadGraph("Data/Title/test.png");
}

//====================================
// 開始
//====================================
void StartTestSceneA()
{
    // 特に初期化が必要なければ空でOK
}

//====================================
// ステップ（入力・ロジック）
//====================================
void StepTestSceneA()
{
    // ESCでタイトルに戻る（デバッグ用）
    if (IsTriggerKey(KEY_P))
    {
        ChangeScene(SCENE_TITLE);
    }
}

//====================================
// 更新
//====================================
void UpdateTestSceneA()
{
    // 今回は処理なし
}

//====================================
// 描画
//====================================
void DrawTestSceneA()
{
    // ★ ここで必ず描画される
    if (g_TestHandle != -1)
    {
        DrawGraph(0, 0, g_TestHandle, TRUE);
    }
}

//====================================
// 終了
//====================================
void FinTestSceneA()
{
    if (g_TestHandle != -1)
    {
        DeleteGraph(g_TestHandle);
        g_TestHandle = -1;
    }
}