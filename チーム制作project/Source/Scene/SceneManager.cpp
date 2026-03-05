#include "SceneManager.h"
#include "TitleScene/TitleScene.h"
#include "PlayScene/PlayScene.h"
#include "../Scene/TitleScene/Select/Select.h"
#include "../Scene/TitleScene/Menu/Menu.h"
#include "TutorialScene/TutorialScene.h"
#include "TestScene/TestSceneA.h"
#include "../Scene/ClearScene/ClearScene.h"

// 現在のシーン
// 初期値は最初に開かれるシーン
Scene g_NowScene = SCENE_TITLE;

// 次に遷移するシーン
Scene g_NextScene = SCENE_TITLE;

// シーンの状態
SceneState g_SceneState = SCENE_STATE_INIT;

// ループを終了するか
bool g_IsLoopEnd = false;

void SceneManagerUpdate()
{
	// シーンの状態ごとに各シーンの処理を呼ぶ
	switch (g_SceneState)
	{
	case SCENE_STATE_INIT:	// 初期化
		InitScene();

		// ロードへ
		g_SceneState = SCENE_STATE_LOAD;
		break;

	case SCENE_STATE_LOAD:	// ロード
		LoadScene();

		// 開始へ
		g_SceneState = SCENE_STATE_START;
		break;

	case SCENE_STATE_START:	// 開始
		StartScene();

		// ループ終了フラグを折っておく
		g_IsLoopEnd = false;

		// ループへ
		g_SceneState = SCENE_STATE_LOOP;
		break;

	case SCENE_STATE_LOOP:	// ループ(ステップ→更新→描画)
		StepScene();
		UpdateScene();
		DrawScene();

		// ループ終了フラグが立ったら終了処理へ
		if (g_IsLoopEnd)
		{
			g_SceneState = SCENE_STATE_FIN;
		}
		break;

	case SCENE_STATE_FIN:	// 終了
		FinScene();

		// 次のシーンに切り替える
		g_NowScene = g_NextScene;

		// シーンは最初から
		g_SceneState = SCENE_STATE_INIT;
		break;
	}
}

void InitScene()
{
	// 開いているシーンの初期化処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		InitTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		InitTutorialScene();
		break;

	case SCENE_STAGE_1:	// ステージ1
		InitPlayScene();
		break;

	case SCENE_STAGE_2:	// ステージ2
		InitPlayScene();
		break;

	case SCENE_STAGE_3:	// ステージ3
		InitPlayScene();
		break;

	case TEST_SCENE_A:	// テスト
		InitTestSceneA();
		break;

	case SCENE_CLEAR:
		InitClearScene();
		break;


	}
}

void LoadScene()
{
	// 開いているシーンのロード処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		LoadTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		LoadTutorialScene(0);
		break;

	case SCENE_STAGE_1:	// ステージ1
		LoadPlayScene(1);
		break;

	case SCENE_STAGE_2:	// ステージ2
		LoadPlayScene(2);
		break;

	case SCENE_STAGE_3:	// ステージ3
		LoadPlayScene(3);
		break;

	case TEST_SCENE_A:	// テスト
		LoadTestSceneA();
		break;

	case SCENE_CLEAR:
		LoadClearScene();
		break;

	}
}

void StartScene()
{
	// 開いているシーンの開始処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		StartTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		StartTutorialScene(0);
		break;

	case SCENE_STAGE_1:	// ステージ1
		StartPlayScene(1);
		break;

	case SCENE_STAGE_2:	// ステージ2
		StartPlayScene(2);
		break;

	case SCENE_STAGE_3:	// ステージ3
		StartPlayScene(3);
		break;

	case TEST_SCENE_A:	// テスト
		StartTestSceneA();
		break;
	
	case SCENE_CLEAR:
		StartClearScene();
		break;
	}
}

void StepScene()
{
	// 開いているシーンのステップ処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		StepTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		StepTutorialScene(0);
		break;

	case SCENE_STAGE_1:	// ステージ1
		StepPlayScene();
		break;

	case SCENE_STAGE_2:	// ステージ2
		StepPlayScene();
		break;

	case SCENE_STAGE_3:	// ステージ3
		StepPlayScene();
		break;

	case TEST_SCENE_A:	// テスト
		StepTestSceneA();
		break;

	case SCENE_CLEAR:
		StepClearScene();
		break;

	}
}

void UpdateScene()
{
	// 開いているシーンの更新処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		UpdateTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		UpdateTutorialScene();
		break;

	case SCENE_STAGE_1:	// ステージ1
		UpdatePlayScene();
		break;

	case SCENE_STAGE_2:	// ステージ2
		UpdatePlayScene();
		break;

	case SCENE_STAGE_3:	// ステージ3
		UpdatePlayScene();
		break;

	case TEST_SCENE_A:	// テスト
		UpdateTestSceneA();
		break;

	case SCENE_CLEAR:
		UpdateClearScene();
		break;
	}
}

void DrawScene()
{
	// 開いているシーンの描画処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		DrawTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		DrawTutorialScene();
		break;

	case SCENE_STAGE_1:	// ステージ1
		DrawPlayScene();
		break;

	case SCENE_STAGE_2:	// ステージ2
		DrawPlayScene();
		break;

	case SCENE_STAGE_3:	// ステージ3
		DrawPlayScene();
		break;

	case TEST_SCENE_A:	// テスト
		DrawTestSceneA();
		break;

	case SCENE_CLEAR:
		DrawClearScene();
		break;

	}
}

void FinScene()
{
	// 開いているシーンのロード処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル
		FinTitleScene();
		break;

	case SCENE_TUTORIAL:	// チュートリアル
		FinTutorialScene();
		break;

	case SCENE_STAGE_1:	// ステージ1
		FinPlayScene();
		break;

	case SCENE_STAGE_2:	// ステージ2
		FinPlayScene();
		break;

	case SCENE_STAGE_3:	// ステージ3
		FinPlayScene();
		break;

	case TEST_SCENE_A:	// テスト
		FinTestSceneA();
		break;

	case SCENE_CLEAR:
		FinClearScene();
		break;
	}
}

void ChangeScene(Scene scene)
{
	// 次に遷移するシーンを設定
	g_NextScene = scene;

	// シーンのループを終了させる
	g_IsLoopEnd = true;
}