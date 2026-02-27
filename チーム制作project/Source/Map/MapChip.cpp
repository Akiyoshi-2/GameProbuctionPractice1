#include "DxLib.h"
#include "MapChip.h"
#include "Block.h"
#include "MapParameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Scene/TitleScene/TitleScene.h"

// マップチップ最大値
MapChipData g_MapChip[MAP_CHIP_Y_NUM_MAX][MAP_CHIP_X_NUM_MAX] = { 0 };

TitleUIData g_TitleData;

void LoadMapChipData(int stage)
{
	// チュートリアルシーン
	if (stage == 1)
	{
		FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage0.bin", "rb") != 0) return;

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 111; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
	// ステージ1
	else if(stage == 0)
	{
			FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage1Map.bin", "rb") != 0) return;

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 305; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
	// ステージ2
	else if (stage == 2)
	{
		FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage2.bin", "rb") != 0) return;

		for (int i = 0; i < 133; i++)
		{
			for (int j = 0; j < 80; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
	// ステージ3
	else if (stage == 3)
	{
		FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage3.bin", "rb") != 0) return;

		for (int i = 0; i < 34; i++)
		{
			for (int j = 0; j < 277; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
}

void CreateMap(int stage)
{
	// チュートリアルシーン
	if (stage == 0)
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 111; j++)
			{
				MapChipType type = (MapChipType)g_MapChip[i][j].mapChip;

				if (type == MAP_CHIP_NONE) continue;

				VECTOR pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);
				g_MapChip[i][j].data = CreateBlock(type, pos);
			}
		}
	}
	// ステージ1
	else if (stage == 1)
	{
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 305; j++)
			{
				MapChipType type = (MapChipType)g_MapChip[i][j].mapChip;

				if (type == MAP_CHIP_NONE) continue;

				VECTOR pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);
				g_MapChip[i][j].data = CreateBlock(type, pos);
			}
		}
	}
	// ステージ2
	else if (stage == 2)
	{
		for (int i = 0; i < 133; i++)
		{
			for (int j = 0; j < 80; j++)
			{
				MapChipType type = (MapChipType)g_MapChip[i][j].mapChip;

				if (type == MAP_CHIP_NONE) continue;

				VECTOR pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);
				g_MapChip[i][j].data = CreateBlock(type, pos);
			}
		}
	}
	// ステージ3
	else if (stage == 3)
	{
		for (int i = 0; i < 34; i++)
		{
			for (int j = 0; j < 277; j++)
			{
				MapChipType type = (MapChipType)g_MapChip[i][j].mapChip;

				if (type == MAP_CHIP_NONE) continue;

				VECTOR pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);
				g_MapChip[i][j].data = CreateBlock(type, pos);
			}
		}
	}

}

MapChipData GetMapChipData(int x, int y)
{
	return g_MapChip[y][x];
}