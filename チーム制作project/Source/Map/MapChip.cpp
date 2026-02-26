#include "DxLib.h"
#include "MapChip.h"
#include "Block.h"
#include "MapParameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Scene/TitleScene/TitleScene.h"

// チュートリアルシーン用
MapChipData g_MapChip[MAP_CHIP_Y_NUM][MAP_CHIP_X_NUM] = { 0 };
// ステージ1用
MapChipData g_Stage1MapChip[STAGE1_MAP_CHIP_Y_NUM][STAGE1_MAP_CHIP_X_NUM] = { 0 };
// ステージ2用
MapChipData g_Stage2MapChip[STAGE2_MAP_CHIP_Y_NUM][STAGE2_MAP_CHIP_X_NUM] = { 0 };
// ステージ3用
MapChipData g_Stage3MapChip[STAGE3_MAP_CHIP_Y_NUM][STAGE3_MAP_CHIP_X_NUM] = { 0 };

TitleUIData g_TitleData;

void LoadMapChipData()
{
	// チュートリアルシーン
	if (g_TitleData.stage == 0)
	{
		FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Tutorial.bin", "rb") != 0) return;

		for (int i = 0; i < MAP_CHIP_Y_NUM; i++)
		{
			for (int j = 0; j < MAP_CHIP_X_NUM; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
	// ステージ1
	else if(g_TitleData.stage == 1)
	{
			FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage1Map.bin", "rb") != 0) return;

		for (int i = 0; i < STAGE1_MAP_CHIP_Y_NUM; i++)
		{
			for (int j = 0; j < STAGE1_MAP_CHIP_X_NUM; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
	// ステージ2
	else if (g_TitleData.stage == 2)
	{
		FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage2Map.bin", "rb") != 0) return;

		for (int i = 0; i < STAGE2_MAP_CHIP_Y_NUM; i++)
		{
			for (int j = 0; j < STAGE2_MAP_CHIP_X_NUM; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
	// ステージ3
	else if (g_TitleData.stage == 3)
	{
		FILE* fp;
		if (fopen_s(&fp, "Data/MapData/Stage3Map.bin", "rb") != 0) return;

		for (int i = 0; i < STAGE3_MAP_CHIP_Y_NUM; i++)
		{
			for (int j = 0; j < STAGE3_MAP_CHIP_X_NUM; j++)
			{
				int map = fgetc(fp);
				g_MapChip[i][j].mapChip = map;
			}
		}
	}
}

void CreateMap()
{
	if (g_TitleData.stage == 0)
	{
		for (int i = 0; i < MAP_CHIP_Y_NUM; i++)
		{
			for (int j = 0; j < MAP_CHIP_X_NUM; j++)
			{
				MapChipType type = (MapChipType)g_MapChip[i][j].mapChip;

				if (type == MAP_CHIP_NONE) continue;

				VECTOR pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);
				g_MapChip[i][j].data = CreateBlock(type, pos);
			}
		}
	}

	else if (g_TitleData.stage == 1)
	{
		for (int i = 0; i < STAGE1_MAP_CHIP_Y_NUM; i++)
		{
			for (int j = 0; j < STAGE1_MAP_CHIP_X_NUM; j++)
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