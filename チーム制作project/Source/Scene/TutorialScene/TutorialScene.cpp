#include "TutorialScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"
#include "../../Map/Block.h"
#include "../../Map/Tutorial/TutorialMap.h"

void InitTutorialScene()
{
	InitBlock();
}

void LoadTutorialScene()
{
	LoadBlock();
	LoadTutorialMap();
}

void StartTutorialScene()
{
	StartTutorialMap();
}

void StepTutorialScene()
{
	
}

void UpdateTutorialScene()
{

}

void DrawTutorialScene()
{
	DrawBlock();
}

void FinTutorialScene()
{

}
