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
#include "../Enemy/FullarmorEnemy/FullarmorEnemy.h"

// キャラの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

void InitMap()
{
	InitBlock();
}

void LoadMap(int stage)
{
	LoadMapChipData(stage);
	LoadBlock();
}

void StartMap(int stage)
{
	CreateMap(stage);
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
		if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

		for (int x = left; x <= right; x++)
		{
			if (x < 0 || x >= MAP_CHIP_X_NUM_MAX)continue;

			MapChipData mapchipdata = GetMapChipData(x, y);
			switch (mapchipdata.mapChip)
			{
			case NORMAL_BLOCK:
				PlayerHitNormalBlockX(mapchipdata);
				break;

			case THORN_BLOCK:
				PlayerHitThornBlockX(mapchipdata);
				break;
			}
		}
	}
 
	for (int y = top; y <= bottom; y++)
	{
		// マップチップからはみ出したら処理しなくていい
		if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX) continue;

		for (int x = left; x <= right; x++)
		{
			// マップチップからはみ出したら処理しなくていい
			if (x < 0 || x >= MAP_CHIP_X_NUM_MAX) continue;

			// マップチップ番号を取得
			MapChipData mapChipData = GetMapChipData(x, y);
			switch (mapChipData.mapChip)
			{
			case NORMAL_BLOCK:
				PlayerHitNormalBlockY(mapChipData);
				break;

			case THORN_BLOCK:
				PlayerHitThornBlockY(mapChipData);
				break;
			}
		}
	}
}

void CheckMapNormalEnemyCollision()
{
	NormalEnemyData* normal = GetNormalEnemy();
	for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normal++)
	{
		int normalX = (int)(normal->pos.x / MAP_CHIP_WIDTH);
		int normalY = (int)((normal->pos.y + NORMAL_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);

		int left = normalX - CHECK_ROUND_NUM;
		int top = normalY - CHECK_ROUND_NUM;
		int right = normalX + CHECK_ROUND_NUM;
		int bottom = normalY + CHECK_ROUND_NUM;

		if (!normal->active)continue;


		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					NormalEnemyHitBlockX(mapChipData, i);
					break;
				}
			}
		}

		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					NormalEnemyHitBlockY(mapChipData, i);
					break;
				}
			}
		}
	}
}

void CheckMapHelmetEnemyCollision()
{
	HelmetEnemyData* helmet = GetHelmetEnemy();
	for (int i = 0; i < HELMET_ENEMY_MAX; i++, helmet++)
	{
		int helmetX = (int)(helmet->pos.x / MAP_CHIP_WIDTH);
		int helmetY = (int)((helmet->pos.y + HELMET_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);

		int left = helmetX - CHECK_ROUND_NUM;
		int top = helmetY - CHECK_ROUND_NUM;
		int right = helmetX + CHECK_ROUND_NUM;
		int bottom = helmetY + CHECK_ROUND_NUM;

		if (!helmet->active)continue;


		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					HelmetEnemyHitBlockX(mapChipData, i);
					break;
				}
			}
		}

		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					HelmetEnemyHitBlockY(mapChipData, i);
					break;
				}
			}
		}
	}
}



void CheckMapShieldEnemyCollision()
{
	ShieldEnemyData* shield = GetShieldEnemy();
	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, shield++)
	{
		int shieldX = (int)(shield->pos.x / MAP_CHIP_WIDTH);
		int shieldY = (int)((shield->pos.y + SHIELD_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);

		int left = shieldX - CHECK_ROUND_NUM;
		int top = shieldY - CHECK_ROUND_NUM;
		int right = shieldX + CHECK_ROUND_NUM;
		int bottom = shieldY + CHECK_ROUND_NUM;

		if (!shield->active)continue;


		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					ShieldEnemyHitBlockX(mapChipData, i);
					break;
				}
			}
		}

		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					ShieldEnemyHitBlockY(mapChipData, i);
					break;
				}
			}
		}
	}
}

void CheckMapYellowEnemyCollision()
{
	YellowEnemyData* yellow = GetYellowEnemy();
	for (int i = 0; i < YELLOW_ENEMY_MAX; i++, yellow++)
	{
		int yellowX = (int)(yellow->pos.x / MAP_CHIP_WIDTH);
		int yellowY = (int)((yellow->pos.y + SHIELD_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);

		int left = yellowX - CHECK_ROUND_NUM;
		int top = yellowY - CHECK_ROUND_NUM;
		int right = yellowX + CHECK_ROUND_NUM;
		int bottom = yellowY + CHECK_ROUND_NUM;

		if (!yellow->active)continue;


		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					YellowEnemyHitBlockX(mapChipData, i);
					break;
				}
			}
		}

		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					YellowEnemyHitBlockY(mapChipData, i);
					break;
				}
			}
		}
	}
}

void CheckMapFullarmorEnemyCollision()
{
	FullArmEnemyData* fullArm = GetFullArmorEnemy();
	for (int i = 0; i < FULLARMOR_ENEMY_MAX; i++, fullArm++)
	{
		int fullArmX = (int)(fullArm->pos.x / MAP_CHIP_WIDTH);
		int fullArmY = (int)((fullArm->pos.y + SHIELD_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);

		int left = fullArmX - CHECK_ROUND_NUM;
		int top = fullArmY - CHECK_ROUND_NUM;
		int right = fullArmX + CHECK_ROUND_NUM;
		int bottom = fullArmY + CHECK_ROUND_NUM;

		if (!fullArm->active)continue;


		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					FullArmorEnemyHitBlockX(mapChipData, i);
					break;
				}
			}
		}

		for (int y = top; y <= bottom; y++)
		{
			if (y < 0 || y >= MAP_CHIP_Y_NUM_MAX)continue;

			for (int x = left; x <= right; x++)
			{
				if (x < 0 || x > MAP_CHIP_X_NUM_MAX)continue;

				MapChipData mapChipData = GetMapChipData(x, y);
				switch (mapChipData.mapChip)
				{
				case NORMAL_BLOCK:
					FullArmorEnemyHitBlockY(mapChipData, i);
					break;
				}
			}
		}
	}
}