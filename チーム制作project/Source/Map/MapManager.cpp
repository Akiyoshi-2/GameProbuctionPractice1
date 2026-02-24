#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
//ここにマップの当たり判定書くってよ

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