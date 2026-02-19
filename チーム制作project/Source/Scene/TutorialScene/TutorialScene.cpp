#include "TutorialScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"
#include "../../Player/Player.h"

void InitTutorialScene()
{
	InitPlayer();
}

void LoadTutorialScene()
{
	LoadPlayer();
}

void StartTutorialScene()
{
	StartPlayer();
}

void StepTutorialScene()
{
	StepPlayer();
}

void UpdateTutorialScene()
{
	UpdatePlayer();
}

void DrawTutorialScene()
{
	DrawPlayer();
}

void FinTutorialScene()
{
	FinPlayer();
}
