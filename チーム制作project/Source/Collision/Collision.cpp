#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"
#include <math.h>

void CheckCollision()
{
	BoxCollision playerBox;

	PlayerData player = GetPlayer();

	playerBox.posX = player.posX;
	playerBox.posY = player.posY;
	playerBox.width = player.width;
	playerBox.height = player.height;
}

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



=======
//#include "DxLib.h"
//#include "Collision.h"
//#include "../Player/Player.h"
//
//void CheckCollision()
//{
//	BoxCollision playerBox;
//
//	PlayerData player = GetPlayer();
//
//	playerBox.posX = player.posX;
//	playerBox.posY = player.posY;
//	playerBox.width = player.width;
//	playerBox.height = player.height;
//}
>>>>>>> 4830d80920fd6fce726dd671cfdfe3393ada1543
