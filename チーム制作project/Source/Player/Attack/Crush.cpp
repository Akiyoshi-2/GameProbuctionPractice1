#include "DxLib.h"
#include "Crush.h"
#include "../Player.h"
#include "../../Enemy/NormalEnemy/NormalEnemy.h"
#include "../../Sound/SoundManager.h"

extern PlayerData g_PrevPlayerData;

bool UpdateCrush(int enemyIndex)
{
    PlayerData* player = GetPlayer();
    NormalEnemyData* enemy = GetNormalEnemy();

    // 青状態のみ
    if (player->type != TYPE_BLUE) return false;

    // 落下中のみ
    if (player->move.y <= 0.0f) return false;

    NormalEnemyData* e = &enemy[enemyIndex];

    if (!e->active) return false;

    float playerBottomPrev = g_PrevPlayerData.pos.y + PLAYER_HEIGHT;
    float enemyTop = e->pos.y;

    // 前フレームで敵より上なら踏みつけ
    if (playerBottomPrev <= enemyTop)
    {
        PlayerKillNormalEnemy(enemyIndex);

        // SE
        PlaySE(SE_PLAYER_CRUSH);

        // バウンド
        player->move.y = -8.0f;

        return true;
    }

    return false;
}