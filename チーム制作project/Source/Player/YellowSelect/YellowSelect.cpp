#include "YellowSelect.h"
#include "DxLib.h"
#include "../Player.h"
#include "../../Camera/Camera.h"
#include "../../Input/Input.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Effect/Effect.h"
#include "../YellowStock/YellowStock.h"

// 内部状態
static bool selecting = false;
static int cursor = 0;

static int yellowPlayerHandle = -1;
static int g_YellowSelectFont = -1;
bool g_IsYellowSelecting = false;

// 保存用運動量
static float savedMoveX = 0.0f;
static float savedMoveY = 0.0f;

// 初期化
void InitYellowSelect()
{
    selecting = false;
    cursor = 0;

    if (yellowPlayerHandle == -1)
        yellowPlayerHandle = LoadGraph("Data/animation/YellowPlayer/黄player1.png");

    if (g_YellowSelectFont == -1)
        g_YellowSelectFont = CreateFontToHandle("Agency FB", 50, 5);
}

// 選択画面中か
bool IsSelectingYellow()
{
    return selecting;
}

// 選択画面処理
bool StepYellowSelect()
{
    PlayerData* player = GetPlayer();

    // Qでいつでも開く
    if (!selecting && IsTriggerKey(KEY_Q))
    {
        selecting = true;
        cursor = 0;
        player->selectingYellow = true;

        //運動量保存
        savedMoveX = player->move.x;
        savedMoveY = player->move.y;

        //停止
        player->move.x = 0.0f;
        player->move.y = 0.0f;

        StartYellowSelect();
    }

    if (!selecting) return false;

    player->move.x = 0.0f;
    player->move.y = 0.0f;

    int stock = GetYellowStock();

    // カーソル移動（ストックあるときだけ）
    if (stock > 0)
    {
        if (IsTriggerKey(KEY_LEFT)) cursor = 0;
        if (IsTriggerKey(KEY_RIGHT)) cursor = 1;
    }

    // 決定
    if (IsTriggerKey(KEY_F))
    {
        if (stock > 0 && cursor == 0)
        {
            if (!UseYellowStock()) return true;

            if (player->type != TYPE_YELLOW)
            {
                player->prevType = player->type;
            }

            player->type = TYPE_YELLOW;
            player->yellowRemainTime = PLAYER_YELLOW_TIME;

            StartEffect(
                PLAYER_CHANGE_YELLOW,
                player->pos.x,
                player->pos.y,
                YELLOW_EFFECT_INTERVAL
            );

            StartPlayerAnimation(YELLOW_PLAYER_ANIM_IDLE);
        }

        selecting = false;
        player->selectingYellow = false;

        //保存していた運動量を戻す
        player->move.x = savedMoveX;
        player->move.y = savedMoveY;

        EndYellowSelect();

        return true;
    }

    return true;
}

// 描画
void DrawYellowSelect()
{
    if (!selecting) return;

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    int stock = GetYellowStock();

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

    // プレイヤー画像
    DrawExtendGraph(
        playerX - size,
        playerY - size,
        playerX + size,
        playerY + size,
        yellowPlayerHandle,
        TRUE
    );

    // ストック表示
    DrawFormatStringToHandle(
        playerX + size - 30,
        playerY + size - 65,
        GetColor(255, 255, 0),
        g_YellowSelectFont,
        "x%d",
        stock
    );

    int y = centerY + 150;

    // ストックあり
    if (stock > 0)
    {
        int yesX = centerX - 100;
        int noX = centerX + 40;

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
    else
    {
        // ストック0 → Noのみ中央
        DrawStringToHandle(
            centerX - 30,
            y,
            "No",
            GetColor(255, 255, 255),
            g_YellowSelectFont
        );
    }
}

void StartYellowSelect() { g_IsYellowSelecting = true; }
void EndYellowSelect() { g_IsYellowSelecting = false; }