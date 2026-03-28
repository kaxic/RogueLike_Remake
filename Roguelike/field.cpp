#include "def.hpp"

AREA areas[AREA_MAX];
int areaCount = 0;
int field[FIELD_HEIGHT][FIELD_WIDTH];
int map_stay[FIELD_HEIGHT][FIELD_WIDTH];
int item_stay[FIELD_HEIGHT][FIELD_WIDTH];
int floor_number = 1;

void drawField()
{
	int x, y;
	int x2 = 0;
	int y2 = 0;
	char way[][3] = {
		"北",
		"南",
		"西",
		"東",
	};
	if (player.hp > player.hpMax)
		player.hp = player.hpMax;
	if (player.hung > player.hungMax)
		player.hung = player.hungMax;
	
	DrawBox(0, SCREEN_RANGE * 2 * PIXEL, SCREEN_RANGE * 2 * PIXEL, HEIGHT, BEIGE, TRUE);
	DrawBox(SCREEN_RANGE * 2 * PIXEL, 0, WIDTH, HEIGHT / 2, WHITE, TRUE);
	DrawBox(SCREEN_RANGE * 2 * PIXEL, HEIGHT / 2, WIDTH, HEIGHT, BLUE, TRUE);
	SetFontSize(20);
	DrawFormatString(SCREEN_RANGE * 2 * PIXEL, 0, BLACK, "地下%dＦ\n経過ターン%d\n"
		, floor_number, turn);
	DrawFormatString(SCREEN_RANGE * 2 * PIXEL, HEIGHT / 2, YELLOW,
		"%s　Lv%2d\nHP:%d/%d\n向き:%s\n武器:%s＋%d\n防具:%s＋%d\n状態:%s\nおなか:%d\n"
		, player.name, level, (player.hp > 0 ? player.hp : 0), player.hpMax, way[player.way]
		, weapon[player.weapon].name, weapon[player.weapon].skil, armor[player.armor].name, armor[player.armor].skil
		, ASname[player.state], player.hung);

	for (y = player.y - SCREEN_RANGE; y < player.y + SCREEN_RANGE; y++)
	{
		for (x = player.x - SCREEN_RANGE; x < player.x + SCREEN_RANGE; x++)
		{
			if ((x < 0) || (x >= FIELD_WIDTH) || (y < 0) || (y >= FIELD_HEIGHT))
				DrawExtendGraph(x2*PIXEL, y2*PIXEL, (x2 + 1)*PIXEL, (y2 + 1)*PIXEL, cellAA[CELL_TYPE_WALL], TRUE);
			else
			{
				DrawExtendGraph(x2*PIXEL, y2*PIXEL, (x2 + 1)*PIXEL, (y2 + 1)*PIXEL, cellAA[field[y][x]], TRUE);
				if (item_stay[y][x] != CELL_TYPE_NONE)
					DrawExtendGraph(x2*PIXEL, y2*PIXEL, (x2 + 1)*PIXEL, (y2 + 1)*PIXEL, cellAA[item_stay[y][x]], TRUE);
				if (map_stay[y][x] != CELL_TYPE_NONE)
					DrawExtendGraph(x2*PIXEL, y2*PIXEL, (x2 + 1)*PIXEL, (y2 + 1)*PIXEL, cellAA[map_stay[y][x]], TRUE);
			}
			x2++;
		}
		y2++;
		x2 = 0;
	}


	
}

void spritArea(int oldAreaIndex)
{
	int newAreaIndex = areaCount;
	int w = areas[oldAreaIndex].w;
	int h = areas[oldAreaIndex].h;

	if (GetRand(1))
	{
		areas[oldAreaIndex].w /= 2;

		areas[newAreaIndex].x = areas[oldAreaIndex].x + areas[oldAreaIndex].w;
		areas[newAreaIndex].y = areas[oldAreaIndex].y;
		areas[newAreaIndex].w = w - areas[oldAreaIndex].w;
		areas[newAreaIndex].h = areas[oldAreaIndex].h;
	}
	else
	{
		areas[oldAreaIndex].h /= 2;

		areas[newAreaIndex].x = areas[oldAreaIndex].x;
		areas[newAreaIndex].y = areas[oldAreaIndex].y + areas[oldAreaIndex].h;
		areas[newAreaIndex].w = areas[oldAreaIndex].w;
		areas[newAreaIndex].h = h - areas[oldAreaIndex].h;
	}
	//分割後の二つのエリアがエリアの条件を満たしているか
	if ((areas[oldAreaIndex].w < AREA_SIZE_MIN) || (areas[oldAreaIndex].h < AREA_SIZE_MIN)
		|| (areas[newAreaIndex].w < AREA_SIZE_MIN) || (areas[newAreaIndex].h < AREA_SIZE_MIN))
	{
		areas[oldAreaIndex].w = w;
		areas[oldAreaIndex].h = h;
		return;
	}

	areaCount++;


	spritArea(oldAreaIndex);
	spritArea(newAreaIndex);
}

void setRandPosition(int& pX,int& pY)
{
	int areaIndex = GetRand(areaCount - 1);
	do
	{
		pX = areas[areaIndex].room.x + GetRand(areas[areaIndex].room.w - 1);
		pY = areas[areaIndex].room.y + GetRand(areas[areaIndex].room.h - 1);
	} while (map_stay[pY][pX] != CELL_TYPE_NONE);
}

void generateField()
{
	int x, x2, y, y2, i;
	areaCount = 0;
	areas[0].x = 0;
	areas[0].y = 0;
	areas[0].w = FIELD_WIDTH;
	areas[0].h = FIELD_HEIGHT;
	areaCount++;
	spritArea(0);

	for (y = 0; y < FIELD_HEIGHT; y++)
		for (x = 0; x < FIELD_WIDTH; x++)
			field[y][x] = CELL_TYPE_WALL;

	for (i = 0; i < areaCount; i++)
	{
		areas[i].room.x = areas[i].x + 2;
		areas[i].room.y = areas[i].y + 2;
		areas[i].room.w = areas[i].w - 4;
		areas[i].room.h = areas[i].h - 4;

		for(y= areas[i].room.y;y< areas[i].room.y+ areas[i].room.h;y++)
			for(x = areas[i].room.x; x < areas[i].room.x + areas[i].room.w; x++)
				field[y][x] = CELL_TYPE_NONE;

		for (x = areas[i].x; x < areas[i].x + areas[i].w; x++)
			field[areas[i].y + areas[i].h - 1][x] = CELL_TYPE_NONE;

		for (y = areas[i].y; y < areas[i].y + areas[i].h; y++)
			field[y][areas[i].x + areas[i].w - 1] = CELL_TYPE_NONE;

		for (y2 = areas[i].y; y2 < areas[i].room.y; y2++)
			field[y2][areas[i].x + areas[i].room.w / 2] = CELL_TYPE_NONE;

		for (y2 = areas[i].room.y + areas[i].room.h; y2 < areas[i].y + areas[i].h; y2++)
			field[y2][areas[i].x + areas[i].room.w / 2] = CELL_TYPE_NONE;

		for (x2 = areas[i].x; x2 < areas[i].room.x; x2++)
			field[areas[i].y + areas[i].room.h / 2][x2] = CELL_TYPE_NONE;

		for (x2 = areas[i].room.x + areas[i].room.w; x2 < areas[i].x + areas[i].w; x2++)
			field[areas[i].y + areas[i].room.h / 2][x2] = CELL_TYPE_NONE;
	}

	for (y = 0; y < FIELD_HEIGHT; y++)
		field[y][FIELD_WIDTH - 1] = CELL_TYPE_WALL;

	for (x = 0; x < FIELD_WIDTH; x++)
		field[FIELD_HEIGHT - 1][x] = CELL_TYPE_WALL;

	while (1)
	{
		bool filled = false;
		for (y = 0; y < FIELD_HEIGHT; y++)
			for (x = 0; x < FIELD_WIDTH; x++)
			{
				if (field[y][x] == CELL_TYPE_WALL)
					continue;

				int v[4][2] = {
					{0,-1},
					{-1,0},
					{0,1},
					{1,0},
				};
				int n = 0;
				for (i = 0; i < 4; i++)
				{
					x2 = x + v[i][0];
					y2 = y + v[i][1];
					if ((x2 < 0) || (x2 >= FIELD_WIDTH) || (y2 < 0) || (y2 >= FIELD_HEIGHT))
						n++;
					else if (field[y2][x2] == CELL_TYPE_WALL)
						n++;
				}
				if (n >= 3)
				{
					field[y][x] = CELL_TYPE_WALL;
					filled = true;
				}
			}
		if (!filled)
			break;
	}

	CharaInit();
	if (floor_number != GOAL_FLOOR)
	{
		enemyInit();
		itemInit();
	}
}

int getRoom(int _x, int _y)
{
	int i;

	for (i = 0; i < areaCount; i++)
		if ((_x >= areas[i].room.x) && (_x < areas[i].room.x + areas[i].room.w)
			&& (_y >= areas[i].room.y) && (_y < areas[i].room.y + areas[i].room.h))
			return i;
	return -1;
}