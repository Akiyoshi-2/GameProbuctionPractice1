#include "DxLib.h"
#include "GameSetting/GameSetting.h"
#include "Input/Input.h"
#include "Scene/SceneManager.h"


// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// ウィンドウモードON
	ChangeWindowMode(TRUE);

	// 画像解像度の設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH);

	if (DxLib_Init() == -1)		// DXライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ウィンドウサイズ設定
	SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// 透過色の設定
	SetTransColor(TRANS_COLOR_R, TRANS_COLOR_G, TRANS_COLOR_B);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 入力システムの初期化
	InitInput();

	// ゲームのメインループ
	while (ProcessMessage() >= 0)
	{
		// 1ミリ止めると安定する
		Sleep(1);

		// 画像をクリア
		ClearDrawScreen();

		// 入力システムの更新
		UpdateInput();

		// シーン管理の更新
		SceneManagerUpdate();

		// 入力システムの描画
		DrawInput();

		// エスケープキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// 画像フリップ
		ScreenFlip();
	}

	// 入力システムの終了
	FinInput();

	DxLib_End();		// DXライブラリ使用の終了処理

	return 0;		// ソフトの終了
}