#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/HelmetEnemy/HelmetEnemy.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"
#include "../Enemy/ShieldEnemy/ShieldEnemy.h"
#include "../Enemy/YellowEnemy/YellowEnemy.h"

// キャラの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

void InitMap()
{
	InitBlock();
}

void LoadMap()
{
	LoadMapChipData();
	LoadBlock();
}

void StartMap()
{
	CreateMap();
}

void DrawMap()
{
	DrawBlock();
}

void FinMap()
{
	FinBlock();
}

void CheckMapPlayerCpllision()
{
	PlayerData* player = GetPlayer();
	int playerX = (int)(player->pos.x / MAP_CHIP_WIDTH);
	int playerY = (int)((player->pos.y + PLAYER_HEIGHT) / MAP_CHIP_HEIGHT);

	int left = playerX - CHECK_ROUND_NUM;
	int top = playerY - CHECK_ROUND_NUM;

	int right = playerX + CHECK_ROUND_NUM;
	int bottom = playerY + CHECK_ROUND_NUM;

	for (int y = top; y <= bottom; y++)
	{
		if (y < 0 || y >= MAP_CHIP_Y_NUM)continue;

		for (int x = left; x <= right; x++)
		{
			if (x < 0 || x >= MAP_CHIP_X_NUM)continue;

			MapChipData mapchipdata = GetMapChipData(x, y);
			switch (mapchipdata.mapChip)
			{
			case NORMAL_BLOCK:
				PlayerHitNormalBlockX(mapchipdata);
				break;
			}
		}
	}
 
	for (int y = top; y <= bottom; y++)
	{
		// マップチップからはみ出したら処理しなくていい
		if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;

		for (int x = left; x <= right; x++)
		{
			// マップチップからはみ出したら処理しなくていい
			if (x < 0 || x >= MAP_CHIP_X_NUM) continue;

			// マップチップ番号を取得
			MapChipData mapChipData = GetMapChipData(x, y);
			switch (mapChipData.mapChip)
			{
			case NORMAL_BLOCK:
				PlayerHitNormalBlockY(mapChipData);
				break;
			}
		}
	}
}

