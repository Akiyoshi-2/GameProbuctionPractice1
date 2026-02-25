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
//ここにマップの当たり判定書くってよ
void CheckMapPlayerCpllision()
{
	PlayerData* player = GetPlayer();
	int playerx = (int)(player->posX / MAP_CHIP_WIDTH);
	int playery = (int)((player->posY + PLAYER_HEIGHT) / MAP_CHIP_HEIGHT);

	int left = playerx - CHECK_ROUND_NUM;
	int top = playery - CHECK_ROUND_NUM;

	int right = playerx + CHECK_ROUND_NUM;
	int bottom = playery + CHECK_ROUND_NUM;

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

// void CheckMapEnemyCollision()
//{
// 	EnemyData* enemy = GetEnemy();
//	for ()int i = 0; i < ENEMY_MAX; i++, enemy++
//	{
//		int EnemyX = (int)(enemy->posX / MAP_CHIP_WIDTH);
//		int EnemyY = (int)((enemy->posY + ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);
//
//		int left = EnemyX - CHECK_ROUND_NUM;
//		int top = EnemyY - CHECK_ROUND_NUM;
//
//		int right = EnemyX + CHECK_ROUND_NUM;
//		int bottom = EnemyY + CHECK_ROUND_NUM;
//
//		if (!enemy->active) continue;
//
//		for (int y = top; y <= bottom; y++)
//		[
//			if (y < || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			[
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData MapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//					case NORMAL_BLOCK;
//						EnemyHITBlockX(MapChipData, i);
//					break;
//				}
//			]
//		]
//
//		for (int y = top; y <= bottm; y++)
//		{
//			if (y < 0 || y >= MAP_CHIP_X_NUM) continue;
//
//			MapChipData mapChipData = GetMapChipData(x, y);
//			switch (mapChipData.mapChip)
//			{
//				case NORMAL_BLOCK;
//					EnemyHitBlock(mapChipData, i);
//				break;
//			}
//		}
//	}
//}

//void CheckMapHelmetEnemyCollision()
//{
//	HelmetEnemyData* HelmetEnemy = GetHelmetEnemy();
//	for (int i = 0; i < HELMET_ENEMY_MAX; i++, HelmetEnemy++)
//	{
//		int EnemyX = (int)(HelmetEnemy->posX / MAP_CHIP_WIDTH);
//		int EnemyY = (int)((HelmetEnemy->posY + HELMET_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);
//
//		int left = EnemyX - CHECK_ROUND_NUM;
//		int top = EnemyY - CHECK_ROUND_NUM;
//
//		int right = EnemyX + CHECK_ROUND_NUM;
//		int bottom = EnemyY + CHECK_ROUND_NUM;
//
//		for (int y = top; y <= bottom; y++)
//		{
//			if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			{
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData mapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//				case NORMAL_BLOCK:
//					HelmetEnemyHitBlockX(mapChipData, i);
//					break;
//				}
//			}
//		}
//
//		for (int y = top; y <= bottom; y++)
//		{
//			if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			{
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData mapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//				case NORMAL_BLOCK:
//					HelmetEnemyHitBlockY(mapChipData, i);
//					break;
//				}
//			}
//		}
//	}
//}

//void CheckMapNormalEnemyCollision()
//{
//	EnemyNormalData* enemyNormal = GetNormal();
//	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemyNormalEnemy++)
//	{
//		int NormalEnemyX = (int)(enemyNormal->posX / MAP_CHIP_WIDTH);
//		int NormalEnemyY = (int)((enemyNormal->posY + ENEMY_NORMAL_HEIGHT) / MAP_CHIP_HEIGHT);
//
//		int left = NormalEnemyX = CHECK_ROUND_NUM;
//		int top = NormalEnemyY - CHECK_ROUND_NUM;
//
//		int right = NormalEnemyX + CHECK_ROUND_NUM;
//		int bottom = NormalEnemyY - CHECK_ROUND_NUM;
//
//		for (int y = top; y <= bottom; y++)
//		{
//			if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			{
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData mapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//				case NORMAL_BLOCK:
//					NormalEnemyHitBlockX(mapChipData, i);
//					break;
//				}
//			}
//		}
//	}
//}
//

//void CheckMapShieldEnemyCollision()
//{
//	ShieldEnemyData* ShieldEnemy = GetShieldEnemy();
//
//	for (int i = 0; i < SHIELD_ENEMY_MAX; i++, ShieldEnemy + )
//	{
//		int ShieldEnemyX = (int)(ShieldEnemy->posX / MAP_CHIP_WIDTH);
//		int ShieldEnemyY = (int)((ShieldEnemy->posY + SHIELD_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);
//
//		int left = ShieldEnemyY - CHECK_ROUND_NUM;
//		int top = ShieldEnemyY - CHECK_ROUND_NUM;
//
//		int right = ShieldEnemyX + CHECK_ROUND_NUM;
//		int bottom = ShieldEnemyY + CHECK_ROUND_NUM;
//
//		for (int y = top; y <= bottom; y++)
//		{
//			if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			{
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData mapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//				case NORMAL_BLOCK:
//					ShieldEnemyHitBlockX(mapChipData, i9;)
//					break;
//				}
//			}
//
//			for (int y = top; y <= bottom; y++)
//			{
//				if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//				for (int x = left; x <= right; x++)
//				{
//					if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//					MapChipData mapChipData = GetMapChipData(x, y);
//					switch (mapChipData.mapChip)
//					{
//					case NORMAL_BLOCK:
//						ShieldEnemyHitBlockY(mapChipData, i);
//						break;
//					}
//				}
//			}
//		}
//	}
//}

//void CheckMapYellowEnemyCollision()
//{
//	EnemyYellowEnemyData* enemyYellowEnemy = GetYellowEnemy();
//	for (int i = 0; i < ENEMY_YELLOW_ENEMY_MAX; i++, enemyYellowEnemy++)
//	{
//		int YellowEnemyX = (int)(enemyYellowEnemy->posX / MAP_CHIP_WIDTH);
//		int YellowEnemyY = (int)((enemyYellowEnemy->posY + ENEMY_YELLOW_ENEMY_HEIGHT) / MAP_CHIP_HEIGHT);
//
//		int left = YellowEnemyX - CHECK_ROUND_NUM;
//		int top = YellowEnemyY - CHECK_ROUND_NUM;
//
//		int right = YellowEnemyX + CHECK_ROUND_NUM;
//		int bottom = YellowEnemyY + CHECK_ROUND_NUM;
//
//		for (int y = top; y <= bottom; y ++)
//		{
//			if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			{
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData mapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//				case NORMAL_BLOCK:
//					YellowEnemyHitBlockX(mapChipData, i);
//					break;
//				}
//			}
//		}
//
//		for (int y = top; y <= bottom; y++)
//		{
//			if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//			for (int x = left; x <= right; x++)
//			{
//				if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//				MapChipData mapChipData = GetMapChipData(x, y);
//				switch (mapChipData.mapChip)
//				{
//				case NORMAL_BLOCK:YellowEnemyHitBlockY(mapChipData, i);
//					break;
//				}
//			}
//		}
//	}
//}

