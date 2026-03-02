#include "EnemyManager.h"
#include "NormalEnemy/NormalEnemy.h"
#include "HelmetEnemy/HelmetEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "YellowEnemy/YellowEnemy.h"
#include "FullarmorEnemy/FullarmorEnemy.h"



void InitEnemy()
{
	// 各エネミー初期化
	InitNormalEnemy();
	InitHelmetEnemy();
	InitShieldEnemy();
	InitYellowEnemy();
	InitFullArmorEnemy();
}

void LoadEnemy()
{
	// 各エネミーロード
	LoadNormalEnemy();
	LoadHelmetEnemy();
	LoadShieldEnemy();
	LoadYellowEnemy();
	LoadFullArmorEnemy();
}

void StepEnemy()
{
	// 各エネミーステップ
	StepNormalEnemy();
	StepHelmetEnemy();
	StepShieldEnemy();
	StepYellowEnemy();
	StepFullArmorEnemy();
}

void UpdateEnemy()
{
	// 各エネミー更新
	UpdateNormalEnemy();
	UpdateHelmetEnemy();
	UpdateShieldEnemy();
	UpdateYellowEnemy();
	UpdateFullArmorEnemy();
}
void DrawEnemy()
{
	// 各エネミー描画
	DrawNormalEnemy();
	DrawHelmetEnemy();
	DrawShieldEnemy();
	DrawYellowEnemy();
	DrawFullArmorEnemy();
}

void FinEnemy()
{
	// 各エネミー終了
	FinNormalEnemy();
	FinHelmetEnemy();
	FinShieldEnemy();
	FinYellowEnemy();
	FinFullArmorEnemy();
}

void SpawnEnemy(const EnemySpawnData* spawnData)
{
	switch (spawnData->type)
	{
	case NORMAL_ENEMY:
		CreateNomalEnemy(spawnData->posX, spawnData->posY, spawnData->param);
		break;
	case HELMET_ENEMY:
		CreateHelmetEnemy(spawnData->posX, spawnData->posY, spawnData->param);
		break;
	case SHIELD_ENEMY:
		CreateShieldEnemy(spawnData->posX, spawnData->posY, spawnData->param);
		break;
	case YELLOW_ENEMY:
		CreateYellowEnemy(spawnData->posX, spawnData->posY, spawnData->param);
		break;
	case FULLARMOR_ENEMY:
		CreateFullArmorEnemy(spawnData->posX, spawnData->posY, spawnData->param);
		break;
	}
}

// enemy配置
void StepEnemySpawnSystem(int stage)
{
	int spawnDataNum = sizeof(ENEMY_SPAWN_DATA) / sizeof(EnemySpawnData);

	const EnemySpawnData* spawn = ENEMY_SPAWN_DATA;
	for (int i = 0; i < spawnDataNum; i++, spawn++)
	{
		SpawnEnemy(spawn);
	}
}