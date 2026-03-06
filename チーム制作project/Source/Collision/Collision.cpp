#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"
#include "../Map/MapManager.h"
#include "../Map/Block.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"
#include "../Enemy/HelmetEnemy/HelmetEnemy.h"
#include "../Enemy/ShieldEnemy/ShieldEnemy.h"
#include "../Enemy/YellowEnemy/YellowEnemy.h"
#include "../Enemy/FullarmorEnemy/FullarmorEnemy.h"
#include "../Enemy/Goal/Goal.h"
#include <math.h>

bool CheckSquarePoint(float squarePosX,  float squarePosY, float squareWidth, float squareHeight, float pointX, float pointY)
{
	if (pointX >= squarePosX && pointX <= (squarePosX + squareWidth))
	{
		if (pointY >= squarePosY && (pointY <= squarePosY + squareHeight))
		{
			return true;
		}
	}

	return false;
}


bool CheckSquarePoint(float squarePosX, float squarePosY, float squareHeight, float pointX, float pointY)
{
	if (pointX >= squarePosX && pointX <= (squarePosX - squareHeight))
	{
		if (pointY >= squarePosY && (pointX <= squarePosX + squareHeight))
		{
			return true;
		}
	}

	return false;
}

bool CheckSquareSquare(float squareA_PosX, float squareA_PosY, float squareA_Width, float squareA_Height, float squareB_PosX, float squareB_PosY, float squareB_Width, float squareB_Height)
{
	float leftA = squareA_PosX;
	float rightA = squareA_PosX + squareA_Width;
	float topA = squareA_PosY;
	float bottomA = squareA_PosY + squareA_Height;

	float leftB = squareB_PosX;
	float rightB = squareB_PosX + squareB_Width;
	float topB = squareB_PosY;
	float bottomB = squareB_PosY + squareB_Height;

	if (rightA >= leftB && leftA <= rightB)
	{
		if (bottomA >= topB && topA <= bottomB)
		{
			return true;
		}
	}

	return false;
}

bool CheckCirclePoint(float circlePosX, float circlePosY, float circleRadius, float pointX, float pointY)
{
	float distance = sqrtf((pointX - circlePosX) * (pointX - circlePosX) + (pointY - circlePosY) * (pointY - circlePosY));

	if (distance <= circleRadius)
	{
		return true;
	}

	return false;

}

bool CheckCircleCircle(float circleA_PosX, float circleA_PosY, float circleA_Radius, float circleB_PosX, float circleB_PosY, float circleB_Radius)
{
	float distance = sqrtf((circleB_PosX - circleA_PosX) * (circleB_PosX - circleA_PosX) + (circleB_PosY - circleA_PosY) * (circleB_PosY - circleA_PosY));

	if (distance <= (circleA_Radius + circleB_Radius))
	{
		return true;
	}

	return false;
}

void CheckPlayerEnemy()
{
	PlayerData* player = GetPlayer();
	NormalEnemyData* normal = GetNormalEnemy();
	HelmetEnemyData* helmet = GetHelmetEnemy();
	ShieldEnemyData* shield = GetShieldEnemy();
	YellowEnemyData* yellow = GetYellowEnemy();
	FullArmEnemyData* fullArm = GetFullArmorEnemy();

	if (player->active)
	{
		// プレイヤーの位置設定
		int playerX = player->pos.x;
		int playerY = player->pos.y;
		int playerW = PLAYER_WIDTH;
		int playerH = PLAYER_HEIGHT;

		for (int i = 0; i < NORMAL_ENEMY_MAX; i++, normal++)
		{
			if (!normal->active)continue;

			// NormalEnemyの位置設定
			int normalX = normal->pos.x;
			int normalY = normal->pos.y;
			int normalW = NORMAL_ENEMY_WIDTH;
			int normalH = NORMAL_ENEMY_HEIGHT;

			if (CheckSquareSquare(playerX, playerY, playerW, playerH, 
				normalX, normalY, normalW, normalH))
			{
				// NormalEnemyとPlayerの当たり判定
			}
		}

		for (int i = 0; i < NORMAL_ENEMY_MAX; i++, helmet++)
		{
			if (!helmet->active)continue;

			// HelmetEnemyの位置設定
			int helmetX = helmet->pos.x;
			int helmetY = helmet->pos.y;
			int helmetW = NORMAL_ENEMY_WIDTH;
			int helmetH = NORMAL_ENEMY_HEIGHT;

			if (CheckSquareSquare(playerX, playerY, playerW, playerH,
				helmetX, helmetY, helmetW, helmetH))
			{
				// HelmetEnemyとPlayerの当たり判定 
			}
		}

		for (int i = 0; i < NORMAL_ENEMY_MAX; i++, shield++)
		{
			if (!shield->active)continue;

			// ShieldEnemyの位置設定
			int shieldX = shield->pos.x;
			int shieldY = shield->pos.y;
			int shieldW = NORMAL_ENEMY_WIDTH;
			int shieldH = NORMAL_ENEMY_HEIGHT;

			if (CheckSquareSquare(playerX, playerY, playerW, playerH,
				shieldX, shieldY, shieldW, shieldH))
			{
				// ShieldEnemyとPlayerの当たり判定 
			}
		}

		for (int i = 0; i < NORMAL_ENEMY_MAX; i++, yellow++)
		{
			if (!yellow->active)continue;

			// YellowEnemyの位置設定
			int yellowX = yellow->pos.x;
			int yellowY = yellow->pos.y;
			int yellowW = NORMAL_ENEMY_WIDTH;
			int yellowH = NORMAL_ENEMY_HEIGHT;

			if (CheckSquareSquare(playerX, playerY, playerW, playerH,
				yellowX, yellowY, yellowW, yellowH))
			{
				// YellowEnemyとPlayerの当たり判定 
			}
		}

		for (int i = 0; i < NORMAL_ENEMY_MAX; i++, fullArm++)
		{
			if (!fullArm->active)continue;

			// FullArmEnemyの位置設定
			int fullArmX = fullArm->pos.x;
			int fullArmY = fullArm->pos.y;
			int fullArmW = NORMAL_ENEMY_WIDTH;
			int fullArmH = NORMAL_ENEMY_HEIGHT;

			if (CheckSquareSquare(playerX, playerY, playerW, playerH,
				fullArmX, fullArmY, fullArmW, fullArmH))
			{
				// FullArmEnemyとPlayerの当たり判定 
			}
		}
	}
	
		
	
}

void CheackPlayerGoal()
{
	PlayerData* player = GetPlayer();
	GoalData* goal = GetGoal();

	if (player->active)
	{
		float playerCenterX = player->pos.x + PLAYER_WIDTH / 2;
		float playerCenterY = player->pos.y + PLAYER_HEIGHT / 2;

		for (int i = 0; i < GOAL_MAX; i++, goal++)
		{
			if (!goal->active)continue;

			float goalCenterX = goal->pos.x + GOAL_WIDTH / 2;
			float goalCenterY = goal->pos.y + GOAL_HEIGHT / 2;

			if (CheckCircleCircle(playerCenterX, playerCenterY, PLAYER_RADIUS,
				goalCenterX, goalCenterY, GOAL_RADIUS))
			{
				PlayerHitGoal();
			}
		}
	}
}

// 各当たり判定を呼ぶ
void CheckCollision()
{
	// プレイヤーとマップの当たり判定
	CheckMapPlayerCpllision();

	// プレイヤーとゴール(エネミー)の当たり判定

	// エネミーとマップとの当たり判定
	CheckMapNormalEnemyCollision();
	CheckMapHelmetEnemyCollision();
	CheckMapShieldEnemyCollision();
	CheckMapYellowEnemyCollision();
	CheckMapFullarmorEnemyCollision();

}
