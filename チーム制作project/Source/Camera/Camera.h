#pragma once

void UpdateCamera();
void DrawCamera();

struct CameraData
{
	float posX;
	float posY;
};

CameraData GetCamera();