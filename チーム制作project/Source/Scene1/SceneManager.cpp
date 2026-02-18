#include "SceneManager.h"
#include "TitleScene/TitleScene.h"
#include "PlayScene/PlayScene.h"
#include "../Scene/TitleScene/Select/Select.h"
#include "../Scene/TitleScene/Menu/Menu.h"

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

	case SCENE_PLAY:	// プレイ
		InitPlayScene();
		break;

	case SCENE_MENU:	//メニュー
		InitMenuScene();
		break;

	case SCENE_SELECT://セレクト
			InitSelectScene();
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

	case SCENE_PLAY:	// プレイ
		LoadPlayScene();
		break;

	case SCENE_MENU:	//メニュー
		LoadMenuScene();
		break;

	case SCENE_SELECT:	//セレクト
		LoadSelectScene();
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

	case SCENE_PLAY:	// プレイ
		StartPlayScene();
		break;

	case SCENE_MENU:	//メニュー
		StartMenuScene();
		break;

	case SCENE_SELECT:	//セレクト
		StartSelectScene();
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

	case SCENE_PLAY:	// プレイ
		StepPlayScene();
		break;

	case SCENE_MENU:	//メニュー
		StepMenuScene();
		break;

	case SCENE_SELECT:	//セレクト
		StepSelectScene();
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

	case SCENE_PLAY:	// プレイ
		UpdatePlayScene();
		break;

	case SCENE_MENU:	//メニュー
		UpdateMenuScene();
		break;

	case SCENE_SELECT:	//セレクト
		UpdateSelectScene();
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

	case SCENE_PLAY:	// プレイ
		DrawPlayScene();
		break;

	case SCENE_MENU:	//メニュー
		DrawMenuScene();
		break;

	case SCENE_SELECT:	//セレクト
		DrawSelectScene();
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

	case SCENE_PLAY:	// プレイ
		FinPlayScene();
		break;

	case SCENE_MENU:	//メニュー
		FinMenuScene();
		break;

	case SCENE_SELECT:	//セレクト
		FinSelectScene();
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