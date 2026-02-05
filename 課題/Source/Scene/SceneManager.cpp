#include "SceneManager.h"
#include "TitleScene/TitleScene.h"
#include "PlayScene/PlayScene.h"
#include "OptionScene/OptionScene.h"

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
		
		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

		break;
	}
}

void LoadScene()
{
	// 開いているシーンのロード処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル

		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

		break;
	}
}

void StartScene()
{
	// 開いているシーンの開始処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル

		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

		break;
	}
}

void StepScene()
{
	// 開いているシーンのステップ処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル

		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

		break;
	}
}

void UpdateScene()
{
	// 開いているシーンの更新処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル

		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

		break;
	}
}

void DrawScene()
{
	// 開いているシーンの描画処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル

		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

		break;
	}
}

void FinScene()
{
	// 開いているシーンのロード処理を呼ぶ
	switch (g_NowScene)
	{
	case SCENE_TITLE:	// タイトル

		break;

	case SCENE_PLAY:	// プレイ

		break;

	case SCENE_OPTION:	// ゲームクリア

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