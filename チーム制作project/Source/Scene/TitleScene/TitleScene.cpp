#include "TitleScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

//画像
int g_TitleHandle = -1;
TitleUIData g_TitleUIData[TITLE_MUX] = {};

//MenuのUI座標
const float MENU_SELECT_Y = 470.0f;
const float MENU_TUTORIAL_Y = 610.0f;

//SelectのUI座標
const float SELECT_STAGE_Y = 470.0;
const float SELECT_STAGE1_X = 170.0f;
const float SELECT_STAGE2_X = 670.0f;
const float SELECT_STAGE3_X = 1170.0f;

const float SELECT_BACK_Y = 700.0f;
const float SELECT_BACK_X = 700.0f;

const float SELECT_ARROW_X = 3.0f;
const float SELECT_ARROW_Y = 460.0f;

//矢印
int g_MenuCursor = MENU_SELECT;
int g_SelectCursor = SELECT_STAGE1;

//SE関連
int g_SEHandle = -1;
int g_MoveSEHandle = -1;

//点滅関係
//AnyKey専用
int  g_BlinkTimer = 0;		//点滅用タイマー
bool g_DrawKeyUI = true;	//描画するかどうか
bool g_IsBlinking = true;   // 点滅しているか
//Scene切替専用
bool g_IsTutorialBlink = false;		//選択時の点滅中フラグ
bool g_DrawTutorialUI = true;		//描画するか
int  g_TutorialBlinkTimer = 0;		//点滅用タイマー
bool g_IsTutorialSceneWait = false;	//シーン遷移待ち中か
int  g_TutorialSceneTimer = 0;		//遷移待ち時間計測

// タイトル画面の状態フラグ
bool g_IsDecided = false; // AnyKey押されたか
bool g_IsShowMenu = false; // メニュー表示中か
bool g_IsStageSelectMode = false; //セレクト表示中か

//シーン切り替えまでの時間作成
bool g_IsSceneChangeWait = false;		//シーン切り替え待ち中か
int g_SceneChangeTimer = 0;				//待ち時間計測用タイマー
const int SCENE_CHANGE_WAIT_TIME = 60;	//フレーム

int g_SelectInputLockTimer = 0;
const int SELECT_INPUT_LOCK_TIME = 15;

// ステージ決定時 点滅用
bool g_IsStageBlink = false;
bool g_DrawStageUI = true;
int  g_StageBlinkTimer = 0;

bool g_IsStageSceneWait = false;
int  g_StageSceneTimer = 0;

int g_DecidedStage = 0;

//Scene帰宅
bool g_ReturnFromGame = false;

void InitTitleScene()
{
	Input_Reset();

	// AnyKey用
	g_BlinkTimer = 0;
	g_DrawKeyUI = true;
	g_IsBlinking = false;

	// Tutorial用
	g_IsTutorialBlink = false;
	g_DrawTutorialUI = true;
	g_TutorialBlinkTimer = 0;
	g_IsTutorialSceneWait = false;
	g_TutorialSceneTimer = 0;

	g_IsDecided = false;
	g_IsShowMenu = false;
	g_IsStageSelectMode = false;

	g_IsSceneChangeWait = false;
	g_SceneChangeTimer = 0;

	g_TitleUIData->stage = 0;

	g_MenuCursor = MENU_SELECT;
	g_SelectCursor = SELECT_STAGE1;

    // ステージ決定状態のリセット
    g_IsStageBlink = false;
    g_DrawStageUI = true;
    g_StageBlinkTimer = 0;

    g_IsStageSceneWait = false;
    g_StageSceneTimer = 0;
    g_DecidedStage = 0;

	// ゲームから戻ってきた時
	if (g_ReturnFromGame)
	{
		g_IsDecided = true;
		g_IsShowMenu = true;
		g_DrawKeyUI = false;
		g_ReturnFromGame = false;
	}
}

void LoadTitleScene()
{
	// 背景
	g_TitleHandle = LoadGraph("Data/Title/画像/Title.png");

	// UI
	g_TitleUIData[TITLE_KEYUI].handle = LoadGraph("Data/Title/画像/AnyKey Push.png");
	g_TitleUIData[MENU_SELECT].handle = LoadGraph("Data/Title/Menu/StageSelect.png");
	g_TitleUIData[MENU_TUTORIAL].handle = LoadGraph("Data/Title/Menu/Tutorial.png");
	g_TitleUIData[MENU_ARROW].handle = LoadGraph("Data/Title/Menu/矢印.png");
	g_TitleUIData[SELECT_STAGE1].handle = LoadGraph("Data/Title/Select/Stage1.png");
	g_TitleUIData[SELECT_STAGE2].handle = LoadGraph("Data/Title/Select/Stage2.png");
	g_TitleUIData[SELECT_STAGE3].handle = LoadGraph("Data/Title/Select/Stage3.png");
	g_TitleUIData[SELECT_ARROW].handle = LoadGraph("Data/Title/Menu/矢印.png");
	g_TitleUIData[SELECT_BACK].handle = LoadGraph("Data/Title/Select/Buck.png");

	// SE
	g_SEHandle = LoadSoundMem("Data/title/Sound/SE/決定(案1).ogg");
	g_MoveSEHandle = LoadSoundMem("Data/title/Sound/SE/カーソル移動案(1).ogg");
}

void StartTitleScene()
{
	g_TitleUIData[TITLE_KEYUI].pos = VGet(0.0f, 0.0f, 0.0f);
	g_TitleUIData[MENU_SELECT].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
	g_TitleUIData[MENU_TUTORIAL].pos = VGet(600.0f, MENU_TUTORIAL_Y, 0.0f);
	g_TitleUIData[MENU_ARROW].pos = VGet(420.0f, 458.0f, 0.0f);
	g_TitleUIData[SELECT_STAGE1].pos = VGet(SELECT_STAGE1_X, SELECT_STAGE_Y, 0.0f);
	g_TitleUIData[SELECT_STAGE2].pos = VGet(SELECT_STAGE2_X, SELECT_STAGE_Y, 0.0f);
	g_TitleUIData[SELECT_STAGE3].pos = VGet(SELECT_STAGE3_X, SELECT_STAGE_Y, 0.0f);
	g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X, SELECT_ARROW_Y, 0.0f);
	g_TitleUIData[SELECT_BACK].pos = VGet(SELECT_BACK_X, SELECT_BACK_Y, 0.0f);
}
void StepTitleScene()
{
    //========================================
    // AnyKey待ち
    //========================================
    if (!g_IsDecided)
    {
        if (Input_IsAnyKeyPush())
        {
            g_IsDecided = true;

            g_IsBlinking = true;
            g_DrawKeyUI = true;
            g_BlinkTimer = 0;

            g_IsSceneChangeWait = true;
            g_SceneChangeTimer = 0;

            PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
            Input_Reset();
        }
    }

    // AnyKey点滅
    if (g_IsBlinking)
    {
        g_BlinkTimer++;
        if (g_BlinkTimer >= 5)
        {
            g_BlinkTimer = 0;
            g_DrawKeyUI = !g_DrawKeyUI;
        }
    }

    // AnyKey → メニュー遷移
    if (g_IsSceneChangeWait)
    {
        g_SceneChangeTimer++;
        if (g_SceneChangeTimer >= SCENE_CHANGE_WAIT_TIME)
        {
            g_IsBlinking = false;
            g_DrawKeyUI = false;
            g_IsShowMenu = true;
            g_IsSceneChangeWait = false;
        }
    }

    //========================================
    // メニュー操作
    //========================================
    if (g_IsShowMenu)
    {
        if (IsTriggerKey(KEY_DOWN) || IsTriggerKey(KEY_UP))
        {
            g_MenuCursor =
                (g_MenuCursor == MENU_SELECT) ? MENU_TUTORIAL : MENU_SELECT;
            PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
        }

        g_TitleUIData[MENU_ARROW].pos.y =
            (g_MenuCursor == MENU_SELECT) ? 458.0f : 598.0f;

        if (IsTriggerKey(KEY_F) && !g_IsStageSceneWait)
        {
            if (g_MenuCursor == MENU_TUTORIAL)
            {
                g_IsTutorialBlink = true;
                g_DrawTutorialUI = true;
                g_TutorialBlinkTimer = 0;

                g_IsTutorialSceneWait = true;
                g_TutorialSceneTimer = 0;

                PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
                Input_Reset();
            }
            else
            {
                g_IsShowMenu = false;
                g_IsStageSelectMode = true;

                g_SelectCursor = SELECT_STAGE1;
                g_TitleUIData[SELECT_ARROW].pos =
                    VGet(SELECT_ARROW_X, SELECT_ARROW_Y, 0);

                g_SelectInputLockTimer = 1;
                Input_Reset();
                PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
            }
        }
    }

    // チュートリアル点滅
    if (g_IsTutorialBlink)
    {
        g_TutorialBlinkTimer++;
        if (g_TutorialBlinkTimer >= 5)
        {
            g_TutorialBlinkTimer = 0;
            g_DrawTutorialUI = !g_DrawTutorialUI;
        }
    }

    // チュートリアル遷移
    if (g_IsTutorialSceneWait)
    {
        g_TutorialSceneTimer++;
        if (g_TutorialSceneTimer >= SCENE_CHANGE_WAIT_TIME)
        {
            ChangeScene(SCENE_TUTORIAL);
        }
    }

    //========================================
    // ステージセレクト操作
    //========================================
    if (g_IsStageSelectMode)
    {
        if (g_SelectInputLockTimer > 0)
        {
            g_SelectInputLockTimer--;

            // 矢印位置だけは更新
            if (g_SelectCursor == SELECT_STAGE1)
                g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X, SELECT_ARROW_Y, 0);
            else if (g_SelectCursor == SELECT_STAGE2)
                g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X + 500, SELECT_ARROW_Y, 0);
            else if (g_SelectCursor == SELECT_STAGE3)
                g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X + 1000, SELECT_ARROW_Y, 0);
            else
                g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X + 520, SELECT_ARROW_Y + 210, 0);

            return; 
        }

        if (IsTriggerKey(KEY_RIGHT))
        {
            if (g_SelectCursor == SELECT_STAGE1) g_SelectCursor = SELECT_STAGE2;
            else if (g_SelectCursor == SELECT_STAGE2) g_SelectCursor = SELECT_STAGE3;
            else if (g_SelectCursor == SELECT_STAGE3) g_SelectCursor = SELECT_STAGE1;
            PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
        }

        if (IsTriggerKey(KEY_LEFT))
        {
            if (g_SelectCursor == SELECT_STAGE1) g_SelectCursor = SELECT_STAGE3;
            else if (g_SelectCursor == SELECT_STAGE2) g_SelectCursor = SELECT_STAGE1;
            else if (g_SelectCursor == SELECT_STAGE3) g_SelectCursor = SELECT_STAGE2;
            PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
        }

        if (IsTriggerKey(KEY_DOWN))
        {
            if (g_SelectCursor != SELECT_BACK)
            {
                g_SelectCursor = SELECT_BACK;
                PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
            }
        }

        if (IsTriggerKey(KEY_UP))
        {
            if (g_SelectCursor == SELECT_BACK)
            {
                g_SelectCursor = SELECT_STAGE1;
                PlaySoundMem(g_MoveSEHandle, DX_PLAYTYPE_BACK);
            }
        }

        // 矢印位置
        if (g_SelectCursor == SELECT_STAGE1)
            g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X, SELECT_ARROW_Y, 0);
        else if (g_SelectCursor == SELECT_STAGE2)
            g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X + 500, SELECT_ARROW_Y, 0);
        else if (g_SelectCursor == SELECT_STAGE3)
            g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X + 1000, SELECT_ARROW_Y, 0);
        else
            g_TitleUIData[SELECT_ARROW].pos = VGet(SELECT_ARROW_X + 520, SELECT_ARROW_Y + 210, 0);

        // 決定
        if (IsTriggerKey(KEY_F))
        {
            if (g_SelectCursor == SELECT_BACK)
            {
                g_IsStageSelectMode = false;
                g_IsShowMenu = true;
                g_SelectCursor = SELECT_STAGE1;
                PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
                Input_Reset();
                return;
            }

            g_DecidedStage =
                (g_SelectCursor == SELECT_STAGE1) ? 1 :
                (g_SelectCursor == SELECT_STAGE2) ? 2 : 3;

            g_IsStageBlink = true;
            g_DrawStageUI = true;
            g_StageBlinkTimer = 0;

            g_IsStageSceneWait = true;
            g_StageSceneTimer = 0;

            PlaySoundMem(g_SEHandle, DX_PLAYTYPE_BACK);
            Input_Reset();
        }
    }

    // ステージ点滅
    if (g_IsStageBlink)
    {
        g_StageBlinkTimer++;
        if (g_StageBlinkTimer >= 5)
        {
            g_StageBlinkTimer = 0;
            g_DrawStageUI = !g_DrawStageUI;
        }
    }

    // ステージ遷移
    if (g_IsStageSceneWait)
    {
        g_StageSceneTimer++;
        if (g_StageSceneTimer >= SCENE_CHANGE_WAIT_TIME)
        {
            if (g_DecidedStage == 1) ChangeScene(SCENE_STAGE_1);
            else if (g_DecidedStage == 2) ChangeScene(SCENE_STAGE_2);
            else if (g_DecidedStage == 3) ChangeScene(SCENE_STAGE_3);
        }
    }
}

void UpdateTitleScene()
{

}

void DrawTitleScene()
{
    DrawGraph(0, 0, g_TitleHandle, TRUE);

    if (g_DrawKeyUI)
    {
        DrawGraph(0, 0, g_TitleUIData[TITLE_KEYUI].handle, TRUE);
    }

    if (g_IsShowMenu)
    {
        DrawGraph(
            (int)g_TitleUIData[MENU_SELECT].pos.x,
            (int)g_TitleUIData[MENU_SELECT].pos.y,
            g_TitleUIData[MENU_SELECT].handle, TRUE);

        if (g_DrawTutorialUI)
        {
            DrawGraph(
                (int)g_TitleUIData[MENU_TUTORIAL].pos.x,
                (int)g_TitleUIData[MENU_TUTORIAL].pos.y,
                g_TitleUIData[MENU_TUTORIAL].handle, TRUE);
        }

        DrawGraph(
            (int)g_TitleUIData[MENU_ARROW].pos.x,
            (int)g_TitleUIData[MENU_ARROW].pos.y,
            g_TitleUIData[MENU_ARROW].handle, TRUE);
    }

    if (g_IsStageSelectMode)
    {
        // ===== ステージ1 =====
        if (g_DecidedStage == 1 && g_IsStageBlink)
        {
            if (g_DrawStageUI)
            {
                DrawGraph((int)g_TitleUIData[SELECT_STAGE1].pos.x,
                    (int)g_TitleUIData[SELECT_STAGE1].pos.y,
                    g_TitleUIData[SELECT_STAGE1].handle, TRUE);
            }
        }
        else
        {
            DrawGraph((int)g_TitleUIData[SELECT_STAGE1].pos.x,
                (int)g_TitleUIData[SELECT_STAGE1].pos.y,
                g_TitleUIData[SELECT_STAGE1].handle, TRUE);
        }

        // ===== ステージ2 =====
        if (g_DecidedStage == 2 && g_IsStageBlink)
        {
            if (g_DrawStageUI)
            {
                DrawGraph((int)g_TitleUIData[SELECT_STAGE2].pos.x,
                    (int)g_TitleUIData[SELECT_STAGE2].pos.y,
                    g_TitleUIData[SELECT_STAGE2].handle, TRUE);
            }
        }
        else
        {
            DrawGraph((int)g_TitleUIData[SELECT_STAGE2].pos.x,
                (int)g_TitleUIData[SELECT_STAGE2].pos.y,
                g_TitleUIData[SELECT_STAGE2].handle, TRUE);
        }

        // ===== ステージ3 =====
        if (g_DecidedStage == 3 && g_IsStageBlink)
        {
            if (g_DrawStageUI)
            {
                DrawGraph((int)g_TitleUIData[SELECT_STAGE3].pos.x,
                    (int)g_TitleUIData[SELECT_STAGE3].pos.y,
                    g_TitleUIData[SELECT_STAGE3].handle, TRUE);
            }
        }
        else
        {
            DrawGraph((int)g_TitleUIData[SELECT_STAGE3].pos.x,
                (int)g_TitleUIData[SELECT_STAGE3].pos.y,
                g_TitleUIData[SELECT_STAGE3].handle, TRUE);
        }

        // 矢印は常に表示
        DrawGraph((int)g_TitleUIData[SELECT_ARROW].pos.x,
            (int)g_TitleUIData[SELECT_ARROW].pos.y,
            g_TitleUIData[SELECT_ARROW].handle, TRUE);

        // BACK
        DrawGraph((int)g_TitleUIData[SELECT_BACK].pos.x,
            (int)g_TitleUIData[SELECT_BACK].pos.y,
            g_TitleUIData[SELECT_BACK].handle, TRUE);
    }
}

void FinTitleScene()
{
	DeleteGraph(g_TitleHandle);

	for (int i = 0; i < TITLE_MUX; i++)
	{
		DeleteGraph(g_TitleUIData[i].handle);
	}

	DeleteSoundMem(g_SEHandle);
	DeleteSoundMem(g_MoveSEHandle);
}