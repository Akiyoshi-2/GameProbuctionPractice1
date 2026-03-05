#include "DxLib.h"
#include "Goal.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"


struct GoalAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};

const GoalAnimationParam GOAL_ANIM_PARAM[GOAL_ANIM_MAX] =
{
	5, 4, 50, 50,
};

#define GOAL_BOX_COLLISION_WIDTH	(50)
#define GOAL_BOX_COLLISION_HEIGHT	(50)

GoalData g_GoalData[GOAL_MAX] = { 0 };


void StartGoalAnimation(GoalAnimationType type, int index);
void UpdateGoalAnimation(int index);

void InitGoal()
{
	GoalData* goal = g_GoalData;
	for (int i = 0; i < GOAL_MAX; i++, goal++)
	{
		goal->pos.x = 0;
		goal->pos.y = 0;
		goal->active = false;
		goal->playAnim = GOAL_ANIM_NONE;
		goal->boxCollision.width = GOAL_BOX_COLLISION_WIDTH;
		goal->boxCollision.height = GOAL_BOX_COLLISION_HEIGHT;

		for (int j = 0; j < GOAL_ANIM_MAX; j++)
		{
			InitAnimation(&goal->animation[j]);
		}

		memset(&goal[i].boxCollision, 0, sizeof(goal[i].boxCollision));
	}
}

void LoadGoal()
{
	int goalHandle = LoadGraph("Data/Akiyosi/GoalAnimation.png");

	for (int i = 0; i < GOAL_MAX; i++)
	{
		g_GoalData[i].animation[GOAL_ANIM_IDLE].handle = goalHandle;
	}
}

void StepGoal()
{

}

void UpdateGoal()
{

}

void DrawGoal()
{

}

void FinGoal()
{

}
