#pragma once

struct CameraData
{
	float posX;
	float posY;
};

void UpdateCamera();
void DrawCamera();
CameraData GetCamera();