//#include "Menu.h"
//#include "../../../Input/Input.h"
//#include "../../SceneManager.h"
//
//int g_MenuBGHandle = -1;
//MenuUIData g_MenuUIData[MENU_MAX] = {};
//
//const float MENU_SELECT_Y = 470.0f;
//const float MENU_TUTORIAL_Y = 610.0f;
//
//int g_MenuCursor = MENU_SELECT;
//
//void InitMenuScene()
//{
//    g_MenuCursor = MENU_SELECT;
//}
//
//void LoadMenuScene()
//{
//    g_MenuUIData[MENU_SELECT].handle = LoadGraph("Data/Title/Menu/StageSelect.png");
//    g_MenuUIData[MENU_TUTORIAL].handle = LoadGraph("Data/Title/Menu/Tutorial.png");
//    g_MenuUIData[MENU_ARROW].handle = LoadGraph("Data/Title/–îˆó.png");
//}
//
//void StartMenuScene()
//{
//    g_MenuUIData[MENU_SELECT].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
//    g_MenuUIData[MENU_TUTORIAL].pos = VGet(600.0f, MENU_TUTORIAL_Y, 0.0f);
//    g_MenuUIData[MENU_ARROW].pos = VGet(600.0f, MENU_SELECT_Y, 0.0f);
//}
//
//void StepMenuScene()
//{
//    if (IsTriggerKey(KEY_DOWN))
//    {
//        g_MenuCursor++;
//        if (g_MenuCursor > MENU_TUTORIAL)
//            g_MenuCursor = MENU_SELECT;
//    }
//
//    if (IsTriggerKey(KEY_UP))
//    {
//        g_MenuCursor--;
//        if (g_MenuCursor < MENU_SELECT)
//            g_MenuCursor = MENU_TUTORIAL;
//    }
//
//    if (g_MenuCursor == MENU_SELECT)
//        g_MenuUIData[MENU_ARROW].pos.y = MENU_SELECT_Y;
//    else
//        g_MenuUIData[MENU_ARROW].pos.y = MENU_TUTORIAL_Y;
//
//    if (IsTriggerKey(KEY_F))
//    {
//        if (g_MenuCursor == MENU_SELECT)
//        {
//            ChangeScene(SCENE_SELECT);
//        }
//    }
//}
//
//void DrawMenuScene()
//{
//    for (int i = 0; i < MENU_MAX; i++)
//    {
//        DrawGraph(
//            (int)g_MenuUIData[i].pos.x,
//            (int)g_MenuUIData[i].pos.y,
//            g_MenuUIData[i].handle,
//            TRUE
//        );
//    }
//}
//
//void FinMenuScene()
//{
//    for (int i = 0; i < MENU_MAX; i++)
//    {
//        DeleteGraph(g_MenuUIData[i].handle);
//    }
//}
