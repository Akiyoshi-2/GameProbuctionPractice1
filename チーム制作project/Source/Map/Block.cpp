#include "Block.h"
#include "../Camera/Camera.h"

BlockData g_Blocks[BLOCK_MAX] = { 0 };
int g_BlockHandle[BLOCK_TYPE_MAX] = { 0 };

void InitBlock()
{
	BlockData* block = g_Blocks;
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		block->active = false;
		block->handle = 0;
		block->pos = VGet(0.0f, 0.0f, 0.0f);
		block->type = MAP_CHIP_NONE;
	}
}

void LoadBlock()
{
	g_BlockHandle[NORMAL_BLOCK] = LoadGraph("Data/MapData/Block.png");
	g_BlockHandle[THORN_BLOCK] = LoadGraph("Data/MapData/ThornBlock.png");
}

void StartBlock()
{
}

void StepBlock()
{
}

void UpdateBlock()
{

}

void DrawBlock()
{
	CameraData cam = GetCamera();

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (!g_Blocks[i].active) continue;

		DrawGraph(
			(int)(g_Blocks[i].pos.x - cam.posX),
			(int)(g_Blocks[i].pos.y - cam.posY),
			g_Blocks[i].handle,
			TRUE
		);
	}
}


void FinBlock()
{
	for (int i = 0; i < BLOCK_TYPE_MAX; i++)
	{
		DeleteGraph(g_BlockHandle[i]);
	}
}

BlockData* CreateBlock(MapChipType type, VECTOR pos)
{
	BlockData* block = g_Blocks;
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (!block->active)
		{
			block->active = true;
			block->handle = g_BlockHandle[type];
			block->pos = pos;
			block->type = type;
			return block;
		}
	}

	return nullptr;
}

BlockData* GetBlock()
{
	return g_Blocks;
}