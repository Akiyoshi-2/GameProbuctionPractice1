#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"

void CheckCollision()
{
	BoxCollision playerBox;

	PlayerData player = GetPlayer();

	playerBox.posX = player.posX;
	playerBox.posY = player.posY;
	playerBox.width = player.width;
	playerBox.height = player.height;
}