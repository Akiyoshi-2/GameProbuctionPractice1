#include "EnemyManager.h"
#include "NormalEnemy/NormalEnemy.h"
#include "HelmetEnemy/HelmetEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "YellowEnemy/YellowEnemy.h"
#include "FullarmorEnemy/FullarmorEnemy.h"
#include "Goal/Goal.h"

int g_EnemySpawnTimer = 0;

void InitEnemy()
{
	g_EnemySpawnTimer = 0;

	// 各エネミー初期化
	InitNormalEnemy();
	InitHelmetEnemy();
	InitShieldEnemy();
	InitYellowEnemy();
	InitFullArmorEnemy();
	// ゴール
	InitGoal();
}

void LoadEnemy()
{
	// 各エネミーロード
	LoadNormalEnemy();
	LoadHelmetEnemy();
	LoadShieldEnemy();
	LoadYellowEnemy();
	LoadFullArmorEnemy();
	// ゴール
	LoadGoal();
}

void StepEnemy()
{
	// 各エネミーステップ
	StepNormalEnemy();
	StepHelmetEnemy();
	StepShieldEnemy();
	StepYellowEnemy();
	StepFullArmorEnemy();
	// ゴール
	StepGoal();
}

void UpdateEnemy()
{
	// 各エネミー更新
	UpdateNormalEnemy();
	UpdateHelmetEnemy();
	UpdateShieldEnemy();
	UpdateYellowEnemy();
	UpdateFullArmorEnemy();
	// ゴール
	UpdateGoal();
}
void DrawEnemy()
{
	// 各エネミー描画
	DrawNormalEnemy();
	DrawHelmetEnemy();
	DrawShieldEnemy();
	DrawYellowEnemy();
	DrawFullArmorEnemy();
	// ゴール
	DrawGoal();
}

void FinEnemy()
{
	// 各エネミー終了
	FinNormalEnemy();
	FinHelmetEnemy();
	FinShieldEnemy();
	FinYellowEnemy();
	FinFullArmorEnemy();
	// ゴール
	FinGoal();
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
	case GOAL:
		CreateGoal(spawnData->posX, spawnData->posY, spawnData->param);
		break;
	}
}

// enemy配置
void StepEnemySpawnSystem(int stage)
{
	if (stage == 0)
	{
		int spawnDataNum = sizeof(ENEMY_SPAWN_DATA_0) / sizeof(EnemySpawnData);

		const EnemySpawnData* spawn = ENEMY_SPAWN_DATA_0;
		for (int i = 0; i < spawnDataNum; i++, spawn++)
		{
			if (spawn->spawnTimer == g_EnemySpawnTimer)
			{
				SpawnEnemy(spawn);
			}
		}

		g_EnemySpawnTimer++;
	}
	/*
	else if (stage == 1)
	{
		int spawnDataNum = sizeof(ENEMY_SPAWN_DATA_1) / sizeof(EnemySpawnData);

		const EnemySpawnData* spawn = ENEMY_SPAWN_DATA_1;
		for (int i = 0; i < spawnDataNum; i++, spawn++)
		{
			SpawnEnemy(spawn);
		}
	}*/
	/*else if (stage == 2)
	{
		int spawnDataNum = sizeof(ENEMY_SPAWN_DATA_2) / sizeof(EnemySpawnData);

		const EnemySpawnData* spawn = ENEMY_SPAWN_DATA_2;
		for (int i = 0; i < spawnDataNum; i++, spawn++)
		{
			SpawnEnemy(spawn);
		}
	}
	else if (stage == 3)
	{
		int spawnDataNum = sizeof(ENEMY_SPAWN_DATA_3) / sizeof(EnemySpawnData);

		const EnemySpawnData* spawn = ENEMY_SPAWN_DATA_3;
		for (int i = 0; i < spawnDataNum; i++, spawn++)
		{
			SpawnEnemy(spawn);
		}
	}*/

	
}