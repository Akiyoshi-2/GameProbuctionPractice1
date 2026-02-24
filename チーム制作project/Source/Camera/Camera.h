#pragma once

void UpdateCamera();

struct CameraData
{
	float posX;
	float posY;
};

CameraData GetCamera();