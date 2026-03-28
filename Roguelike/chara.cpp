#include "def.hpp"

CHARACTER player;
char ASname[AS_KIND][50]=
{
{"普通"},
{"眠り"},
{"毒"},
{"感電"},
{"混乱"},
{"呪い"}
};
CHARACTER enemy[ENEMY_KIND][ENEMY_NUM];
CHARACTER boss;
DEF_E enemy_def[ENEMY_KIND] =
{
{"バット",                25,  9, 3,  5},
{"ウルフ",                40, 18, 4,  8},
{"火の玉",                60, 26, 6, 15},
{"電気クラゲ",            80, 38, 8, 26},
{"プチデーモン",         100, 50,10, 38},
{"ケサランパサラン",     120, 71,11, 52},
{"ベビーゴルゴン",       140, 85,13, 68},
{"ダークエルフ",         150,101,16, 85},
{"グリフォン",           180,122,17, 99},
{"グレイシャードラゴン", 200,150,20,130}
};

int stairX, stairY;


void CharaInit()
{
	int x, y;
	int i, j;

	for (y = 0; y < FIELD_HEIGHT; y++)
		for (x = 0; x < FIELD_WIDTH; x++)
			item_stay[y][x] = map_stay[y][x] = CELL_TYPE_NONE;

	for (i = 0; i < ENEMY_KIND; i++)
		for (j = 0; j < ENEMY_NUM; j++)
			enemy[i][j].life = false;

	for (i = 0; i < ITEM_KIND; i++)
		for (j = 0; j < ITEM_NUM + HAVE_NUM_MAX; j++)
			item[i][j].stay = false;

	setRandPosition(player.x, player.y);
	map_stay[player.y][player.x] = CELL_TYPE_PLAYER;
	
	setRandPosition(x, y);
	if (floor_number == GOAL_FLOOR)
		bossInit();
	else
	{
		field[y][x] = CELL_TYPE_STAIRS;
		stairX = x;
		stairY = y;
	}
}

void enemyInit()
{
	int i, j;
	int enemyCount = 0;

	for (i = 0; i < ENEMY_KIND; i++)
	{
		if (i + ENEMY_NUM_MAX/ENEMY_NUM < floor_number)
			continue;
		if (i > floor_number)
			break;
		for (j = 0; j < ENEMY_NUM; j++)
		{
			enemyCount++;
			if (enemyCount > ENEMY_NUM_MAX)
				return;
			setRandPosition(enemy[i][j].x, enemy[i][j].y);
			map_stay[enemy[i][j].y][enemy[i][j].x] = CELL_TYPE_BAT + i;
			strcpy_s(enemy[i][j].name, 50, enemy_def[i].name);
			enemy[i][j].hp = enemy[i][j].hpMax = enemy_def[i].hpMax;
			enemy[i][j].atk = enemy_def[i].atk;
			enemy[i][j].def = enemy_def[i].def;
			enemy[i][j].life = true;
			enemy[i][j].exp = enemy_def[i].exp;
		}
	}
}

void bossInit()
{
	setRandPosition(boss.x, boss.y);
	map_stay[boss.y][boss.x] = CELL_TYPE_BOSS;
	return;
}