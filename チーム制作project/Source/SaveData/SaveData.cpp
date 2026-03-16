#include "SaveData.h"
#include <stdio.h>

#define SAVE_FILE "Data/SaveData/save.txt"

void SaveGameData(int life, int score)
{
    FILE* fp;

    fopen_s(&fp, SAVE_FILE, "w");

    if (fp == NULL) return;

    fprintf_s(fp, "Life:%d\n", life);
    fprintf_s(fp, "Score:%d\n", score);

    fclose(fp);
}

void LoadGameData(int& life, int& score)
{
    FILE* fp;

    fopen_s(&fp, SAVE_FILE, "r");

    if (fp == NULL)
    {
        life = 3;
        score = 0;
        return;
    }

    if (fscanf_s(fp, "Life:%d\nScore:%d", &life, &score) != 2)
    {
        life = 3;
        score = 0;
    }

    fclose(fp);
}