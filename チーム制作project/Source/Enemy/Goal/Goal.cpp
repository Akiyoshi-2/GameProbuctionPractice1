#include "DxLib.h"
#include "Goal.h"
#include "../EnemyParameter.h"
#include "../../GameSetting/GameSetting.h"
#include "../../Scene/SceneManager.h"


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


void StartGoalAnimation(GoalAnimationType anim, int index);
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
	GoalData* goal = g_GoalData;
	for (int i = 0; i < GOAL_MAX; i++, goal++)
	{
		
		if (!goal->active)continue;

		UpdateGoalAnimation(i);
	}
}

void DrawGoal()
{
	GoalData* goal = g_GoalData;
	for (int i = 0; i < GOAL_MAX; i++, goal++)
	{
		if (!goal->active)continue;

		GoalAnimationType animType = goal->playAnim;
		AnimationData* animData = &goal->animation[animType];

		DrawAnimation(animData, goal->pos.x, goal->pos.y, FALSE, FALSE);
	}
	
}

void FinGoal()
{
	for (int i = 0; i < GOAL_ANIM_MAX; i++)
	{
		DeleteGraph(g_GoalData->animation[i].handle);
	}
}

GoalData* GetGoal()
{
	return g_GoalData;
}

void PlayerHitGoal()
{
	ChangeScene(SCENE_CLEAR);
}

void CreateGoal(float posX, float posY, const EnemyParameter* param)
{
	GoalData* goal = g_GoalData;
	for (int i = 0; i < GOAL_MAX; i++, goal++)
	{
		if (!goal->active)
		{
			goal->active = true;

			goal->animation->handle = g_GoalData->animation->handle;

			goal->pos.x = posX;
			goal->pos.y = posY;

			goal->param = param;

			break;
		}
	}
}

void StartGoalAnimation(GoalAnimationType anim, int index)
{
	GoalData* goal = &g_GoalData[index];

	if (anim == goal->playAnim)return;

	goal->playAnim = anim;

	AnimationData* animData = &goal->animation[anim];
	GoalAnimationParam animParam = GOAL_ANIM_PARAM[anim];

	StartAnimation(animData, goal->pos.x, goal->pos.y,
		animParam.interval, animParam.frameNum,
		animParam.width, animParam.height, true);
}

void UpdateGoalAnimation(int index)
{
	GoalData* goal = &g_GoalData[index];

	StartGoalAnimation(GOAL_ANIM_IDLE, index);

	AnimationData* animData = &goal->animation[goal->playAnim];
	UpdateAnimation(animData);
}
