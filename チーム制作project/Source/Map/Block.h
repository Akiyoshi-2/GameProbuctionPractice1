#pragma once
#include "MapParameter.h"

void InitBlock();
void LoadBlock();
void StartBlock();
void StepBlock();
void UpdateBlock();
void DrawBlock();
void FinBlock();

BlockData* CreateBlock(MapChipType type, VECTOR pos);
BlockData* GetBlock();