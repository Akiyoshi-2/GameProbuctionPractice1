#include "YellowStock.h"

static int g_YellowStock = 0;

void InitYellowStock()
{
    g_YellowStock = 0;
}

void AddYellowStock(int value)
{
    g_YellowStock += value;
}

bool UseYellowStock()
{
    if (g_YellowStock <= 0) return false;

    g_YellowStock--;
    return true;
}

int GetYellowStock()
{
    return g_YellowStock;
}

void SetYellowStock(int value)
{
    g_YellowStock = value;
}