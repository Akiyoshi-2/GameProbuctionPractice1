#include "YellowSelect.h"
#include "DxLib.h"
#include "../Player.h"
#include "../../Camera/Camera.h"
#include "../../Input/Input.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Effect/Effect.h"

// 内部状態
static bool selecting = false;
static int cursor = 0; // 0 = Yes, 1 = No
static int yellowPlayerHandle = -1;
static int g_YellowSelectFont = -1;
bool g_IsYellowSelecting = false;

// 初期化
void InitYellowSelect()
{
    selecting = false;
    cursor = 0;

    // 画像ロード
    if (yellowPlayerHandle == -1)
        yellowPlayerHandle = LoadGraph("Data/animation/YellowPlayer/黄player1.png");

    // フォント作成（Agency FB）
    if (g_YellowSelectFont == -1)
        g_YellowSelectFont = CreateFontToHandle("Agency FB", 50, 5);//フォント名、サイズ、太さ
}

// 選択画面中か
bool IsSelectingYellow()
{
    return selecting;
}

// 選択画面処理（StepPlayer内で呼ぶ）
void StepYellowSelect()
{
    PlayerData* player = GetPlayer();

    // Q キーでいつでも選択画面に
    if (!selecting && IsTriggerKey(KEY_Q))
    {
        selecting = true;
        cursor = 0;
        player->selectingYellow = true;

        StartYellowSelect();
    }

    if (!selecting) return;

    // カーソル移動
    if (IsTriggerKey(KEY_LEFT))
    {
        cursor = 0;
    }

    if (IsTriggerKey(KEY_RIGHT))
    {
        cursor = 1;
    }

    // 決定（Fキー）
    if (IsTriggerKey(KEY_F))
    {
        if (cursor == 0) // Yes
        {
            // 黄色状態でなければ元のタイプ保存
            if (player->type != TYPE_YELLOW)
            {
                player->prevType = player->type;
            }

            player->type = TYPE_YELLOW;
            player->yellowRemainTime = PLAYER_YELLOW_TIME;

            // 変身エフェクト
            StartEffect(
                PLAYER_CHANGE_YELLOW,
                player->pos.x,
                player->pos.y,
                YELLOW_EFFECT_INTERVAL
            );

            StartPlayerAnimation(YELLOW_PLAYER_ANIM_IDLE);
        }
        // No の場合は何もしない

        selecting = false;
        player->selectingYellow = false;

        EndYellowSelect();
    }
}

// 選択画面描画（DrawPlayer内で呼ぶ）
void DrawYellowSelect()
{

    if (!selecting) return;

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // 暗転
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // タイトル
    DrawStringToHandle(
        centerX - 110,
        centerY - 250,
        "Change Yellow?",
        GetColor(255, 255, 0),
        g_YellowSelectFont
    );

    int playerX = centerX - 30;
    int playerY = centerY - 10;
    int size = 90;

    DrawExtendGraph(
        playerX - size,
        playerY - size,
        playerX + size,
        playerY + size,
        yellowPlayerHandle,
        TRUE
    );

    // Yes / No
    int yesX = centerX - 100;
    int noX = centerX + 40;
    int y = centerY + 150;

    DrawStringToHandle(
        yesX,
        y,
        "Yes",
        cursor == 0 ? GetColor(255, 255, 0) : GetColor(255, 255, 255),
        g_YellowSelectFont
    );

    DrawStringToHandle(
        noX,
        y,
        "No",
        cursor == 1 ? GetColor(255, 255, 0) : GetColor(255, 255, 255),
        g_YellowSelectFont
    );
}

void StartYellowSelect() { g_IsYellowSelecting = true; }
void EndYellowSelect() { g_IsYellowSelecting = false; }