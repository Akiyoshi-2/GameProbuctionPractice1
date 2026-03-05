#pragma once
#include "DxLib.h"

#define BLOCK_MAX	2000

#define MAP_CHIP_WIDTH (50.0f)
#define MAP_CHIP_HEIGHT (50.0f)

// マップチップ最大値
#define MAP_CHIP_X_NUM_MAX (305)
#define MAP_CHIP_Y_NUM_MAX (133)

// チュートリアルシーン
#define MAP_CHIP_X_NUM (111)
#define MAP_CHIP_Y_NUM (20)
// ステージ1
#define STAGE1_MAP_CHIP_X_NUM	(305)
#define STAGE1_MAP_CHIP_Y_NUM	(30)
// ステージ2
#define STAGE2_MAP_CHIP_X_NUM	(80)
#define STAGE2_MAP_CHIP_Y_NUM	(133)
// ステージ3
#define STAGE3_MAP_CHIP_X_NUM	(277)
#define STAGE3_MAP_CHIP_Y_NUM	(34)

enum MapChipType
{
	MAP_CHIP_NONE,
	NORMAL_BLOCK,
	THORN_BLOCK,
	GOAL_BLOCK,
	BLOCK_TYPE_MAX,
};

struct BlockData
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
};

struct MapChipData
{
	int mapChip;
	BlockData* data;
};