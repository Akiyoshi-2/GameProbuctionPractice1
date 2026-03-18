#include "DxLib.h"
#include "Life.h"
#include "../Player.h"

// 画像ハンドル
static int RedLifeHandle = -1;
static int BlueLifeHandle = -1;
static int YellowLifeHandle = -1;
static int g_Life = 5;

// フォント
static int g_LifeFontHandle = -1;

// 表示位置
#define LIFE_POS_X (0)
#define LIFE_POS_Y (0)

void InitLife()
{
    // ライフ初期化
    g_Life = 5;

    // Agency FB フォント作成
    g_LifeFontHandle = CreateFontToHandle("Agency FB", 40, 3);
}

void LoadLife()
{
    // ライフ画像
    RedLifeHandle = LoadGraph("Data/player/赤/赤player1.png");
    BlueLifeHandle = LoadGraph("Data/player/青/青player1.png");
    YellowLifeHandle = LoadGraph("Data/player/黄/黄player1.png");

}

void UpdateLife()
{

}

void DrawLife()
{
    PlayerData* player = GetPlayer();

    int handle = -1;

    // プレイヤー状態で画像変更
    if (player->type == TYPE_BLUE)
    {
        handle = BlueLifeHandle;
    }
    else if (player->type == TYPE_RED)
    {
        handle = RedLifeHandle;
    }
    else
    {
        handle = YellowLifeHandle;
    }

    // サイズ指定
    int size = 1; //倍率

    // 画像描画（拡大）
    DrawExtendGraph(
        LIFE_POS_X,
        LIFE_POS_Y,
        LIFE_POS_X + 80 * size,
        LIFE_POS_Y + 80 * size,
        handle,
        TRUE
    );

    // ライフ表示
    int life = player->life;

    if (life < 0)
    {
        life = 0;
    }

    DrawFormatStringToHandle(
        LIFE_POS_X + 80,
        LIFE_POS_Y + 20,
        GetColor(0, 0, 0),
        g_LifeFontHandle,
        "X%d",
        life
    );
}

void SetLife(int life)
{
    g_Life = life;
}

void FinLife()
{
    DeleteGraph(RedLifeHandle);
    DeleteGraph(BlueLifeHandle);
    DeleteFontToHandle(g_LifeFontHandle);
}