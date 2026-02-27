// Camera.h
#pragma once

struct CameraData
{
    float posX;
    float posY;
};

void UpdateCamera();
void DrawCamera();
CameraData GetCamera();

// ’Ç‰Á
void ResetCamera();
void SetCameraStage(int stage);