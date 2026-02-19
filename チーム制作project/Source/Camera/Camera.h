#pragma once

struct CameraData
{
	float posX;
	float posY;
};

void InitCamera();
void StepCamera();
void DrawCamera();

CameraData GetCamera();



