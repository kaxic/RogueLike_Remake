#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>	
#include "DxLib.h"
#include "def.hpp"

int story(char buf[])
{
	static MENU menu = MENU_NONE;

	if (buf[KEY_INPUT_ESCAPE] == 1)
		return 9;
	drawField();
	if (player.stay==CELL_TYPE_STAIRS)
	{
		stair(buf);
		return 1;
	}
	if ((buf[KEY_INPUT_LSHIFT] == 1) || (buf[KEY_INPUT_RSHIFT] == 1))
		menu = MENU_BASIC;
	if (menu != MENU_NONE)
	{
		command(menu, buf);
		WaitTimer(100);
		return 1;
	}
	if (player.state == AS_SLEEP)
		enemy_act();
	else
		player_act(buf);
	if (player.hp <= 0)
	{
		player.hp = 0;
		player.life = false;
		map_stay[player.y][player.x] = CELL_TYPE_DEAD;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%s‚Í—Ís‚«‚½...\n", player.name);
		WaitTimer(MES_TIME);
		gameover();
		return 0;
	}
	if (boss.hp <= 0)
	{
		boss.life = false;
		map_stay[boss.y][boss.x] = CELL_TYPE_NONE;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%s‚Í“|‚ê‚½\n", boss.name);
		WaitTimer(MES_TIME);
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "’n‰º–À‹{‚ðŠ®‘S§”e‚µ‚½\n");
		WaitTimer(MES_TIME);
		gameclear();
		return 0;
	}
	
	WaitTimer(100);

	return 1;
}
