#include "DxLib.h"
#include "Crush.h"
#include "../Player.h"
#include "../../Enemy/NormalEnemy/NormalEnemy.h"

void UpdateCrush()
{
    PlayerData* player = GetPlayer();

    // Âó‘ÔˆÈŠO‚Í“¥‚ß‚È‚¢
    if (player->type != TYPE_BLUE) return;

    // ‹ó’†‚¶‚á‚È‚¢‚Æ“¥‚ß‚È‚¢
    if (!player->isAir) return;

    NormalEnemyData* enemy = GetNormalEnemy();

    for (int i = 0; i < NORMAL_ENEMY_MAX; i++)
    {
        if (!enemy[i].active) continue;

        float px = player->pos.x + 10;
        float py = player->pos.y + 40;
        float pw = 30;
        float ph = 10;

        float ex = enemy[i].pos.x;
        float ey = enemy[i].pos.y;
        float ew = 38;
        float eh = 38;

        if (CheckSquareSquare(px, py, pw, ph, ex, ey, ew, eh))
        {
            // “G‚ð“|‚·
            PlayerKillNormalEnemy(i);

            // ƒvƒŒƒCƒ„[‚ð­‚µ’µ‚Ë‚³‚¹‚é
            player->move.y = -8.0f;

            return;
        }
    }
}