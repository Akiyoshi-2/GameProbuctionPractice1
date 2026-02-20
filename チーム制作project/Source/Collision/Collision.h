#pragma once

void CheckCollision();

struct BoxCollision
{
	float posX;
	float posY;
	float width;
	float height;
};

bool CheckSquarePoint(float squarePosX, float squarePosY, float squareHeight, float pointX, float pointY);

bool CheckSquarePoint(float squarePosX, float squarePosY, float squareWidth, float squareHeight, float pointX, float pointY);

bool CheckSquareSquare(float squareA_PosX, float squareA_PosY, float squareA_Width, float squareA_Height, float squareB_PosX, float square_PosY, float squareB_Width, float squareB_Height);

bool CheckCirclePoint(float circlePosX, float circlePosY, float circleRadius, float pointX, float pointY);

bool CheckCircleCircle(float circleA_PosX, float circleA_PosY, float circleA_Radius, float circleB_PosX, float circleB_PosY, float circleB_Radius);

//#pragma once
//
//void CheckCollision();
//
//struct BoxCollision
//{
//	float posX;
//	float posY;
//	float width;
//	float height;
//};

