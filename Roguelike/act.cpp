#include "def.hpp"

int turn;
int level;
int have_num;
ITEM have_item[HAVE_NUM_MAX];

void player_act(char buf[])
{
	int x = player.x;
	int y = player.y;

	if (buf[KEY_INPUT_UP] == 1)
	{
		y--;
		player.way = NORTH;
	}
	if (buf[KEY_INPUT_DOWN] == 1)
	{
		y++;
		player.way = SOUTH;
	}
	if (buf[KEY_INPUT_LEFT] == 1)
	{
		x--;
		player.way = WEST;
	}
	if (buf[KEY_INPUT_RIGHT] == 1)
	{
		x++;
		player.way = EAST;
	}
	if ((player.state == AS_CONFUSION)&&((x!=player.x)||(y!=player.y)))
	{
		x = player.x;
		y = player.y;
		player.way = GetRand(EAST);
		switch (player.way)
		{
		case NORTH:
			y--;
			break;
		case SOUTH:
			y++;
			break;
		case WEST:
			x--;
			break;
		case EAST:
			x++;
			break;
		}
	}
	if (map_stay[y][x] < CELL_TYPE_PLAYER)   //衝突防止
	{
		switch (field[y][x])
		{
		case CELL_TYPE_WALL:
			break;
		case CELL_TYPE_STAIRS:
		case CELL_TYPE_NONE:
			if (player.state == AS_ELEC)
			{
				if (GetRand(7) == 0)
				{
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは体が痺れて動けない\n"
						, player.name);
					WaitTimer(MES_TIME);
					enemy_act();
					return;
				}
			}
			map_stay[player.y][player.x] = CELL_TYPE_NONE;
			player.x = x;
			player.y = y;
			map_stay[player.y][player.x] = CELL_TYPE_PLAYER;
			player.stay = field[player.y][player.x];
			if (item_stay[player.y][player.x]!=CELL_TYPE_NONE)
				pickup();
			enemy_act();
			break;
		}
	}

	//攻撃
	if (buf[KEY_INPUT_SPACE] == 1)
	{
		if (player.state == AS_ELEC)
		{
			if (GetRand(7) == 0)
			{
				drawField();
				DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは体が痺れて動けない\n"
					, player.name);
				WaitTimer(MES_TIME);
				enemy_act();
				return;
			}
		}
		if (player.state == AS_CONFUSION)
		{
			player.way = GetRand(EAST);
		}
		switch (player.way)
		{
		case NORTH:
			attack(player.y - 1, player.x);
			break;
		case SOUTH:
			attack(player.y + 1, player.x);
			break;
		case WEST:
			attack(player.y, player.x - 1);
			break;
		case EAST:
			attack(player.y, player.x + 1);
			break;
		}
	}
}

void enemy_act()
{
	int i, j;
	int enemyCount = 0;

	if (floor_number == GOAL_FLOOR)
	{
		boss_act();
		Turn();
		return;
	}
	
	for (i = 0; i < ENEMY_KIND; i++)
	{
		if (i + ENEMY_NUM_MAX / ENEMY_NUM < floor_number)
			continue;
		if (i > floor_number)
			break;
		for (j = 0; j < ENEMY_NUM; j++)
		{
			enemyCount++;
			if (enemyCount > ENEMY_NUM_MAX)
			{
				i = ENEMY_KIND;
				j = ENEMY_NUM;
				continue;
			}
			if (!enemy[i][j].life)
				continue;
			int x = enemy[i][j].x;
			int y = enemy[i][j].y;

			int room = getRoom(enemy[i][j].x, enemy[i][j].y);
			int distance = abs(player.x - enemy[i][j].x) + abs(player.y - enemy[i][j].y);
			if (enemy[i][j].hp > enemy[i][j].hpMax)
				enemy[i][j].hp = enemy[i][j].hpMax;
			if (distance == 1)
			{
				int damage;
				drawField();
				if ((i >= ENEMY_FIRE) && (GetRand(5) == 0))
					spec(i, j);
				else
				{
					damage = Damage(enemy[i][j].atk, player.def + armor[player.armor].skil);
					player.hp -= damage;
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sの攻撃\n%sは%dのダメージを受けた\n"
						, enemy[i][j].name, player.name, damage);
					WaitTimer(MES_TIME);
				}
				continue;
			}
			else if (((room >= 0) && (room == getRoom(player.x, player.y))) || (distance == 2))
			{
				map_stay[enemy[i][j].y][enemy[i][j].x] = CELL_TYPE_NONE;
				if (x < player.x)
					x++;
				if (x > player.x)
					x--;
				if (y < player.y)
					y++;
				if (y > player.y)
					y--;
				if ((x != enemy[i][j].x) && (y != enemy[i][j].y))       //引っ掛かり防止
				{
					if (field[enemy[i][j].y][x] == CELL_TYPE_WALL)
						x = enemy[i][j].x;
					else if (field[y][enemy[i][j].x] == CELL_TYPE_WALL)
						y = enemy[i][j].y;
					else
					{
						if (GetRand(1))
							x = enemy[i][j].x;
						else
							y = enemy[i][j].y;
					}
				}

				if (map_stay[y][x] >= CELL_TYPE_BAT)
				{

				}
				else if (field[y][x] != CELL_TYPE_WALL)
				{
					enemy[i][j].x = x;
					enemy[i][j].y = y;
				}
			}
			else
			{
				map_stay[enemy[i][j].y][enemy[i][j].x] = CELL_TYPE_NONE;
				while (1)
				{
					switch (GetRand(3))
					{
					case 0:
						x++;
						break;
					case 1:
						x--;
						break;
					case 2:
						y++;
						break;
					case 3:
						y--;
						break;
					}
					if ((field[y][x] != CELL_TYPE_WALL) && (map_stay[y][x] < CELL_TYPE_BAT))
					{
						enemy[i][j].x = x;
						enemy[i][j].y = y;
						break;
					}
					x = enemy[i][j].x;
					y = enemy[i][j].y;
				}
			}
			map_stay[enemy[i][j].y][enemy[i][j].x] = CELL_TYPE_BAT + i;
		}
	}
	Turn();
}

int Damage(int atk,int def) 
{
	return 1 + (atk * 5.36 + GetRand(atk)) / (double)def;
}

void attack(int y,int x)
{
	if (map_stay[y][x] < CELL_TYPE_BAT)
	{
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは空振りをした\n", player.name);
		WaitTimer(MES_TIME);
	}
	else
	{
		int i, j;
		int damage;
		i = map_stay[y][x] - CELL_TYPE_BAT;

		if (i == BOSS)
		{
			damage = Damage(player.atk + weapon[player.weapon].skil, boss.def);
			boss.hp -= damage;
			DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sの攻撃\n%sに%dのダメージを与えた\n"
				, player.name, boss.name, damage);
			WaitTimer(MES_TIME);
			boss_act();
			return;
		}

		for (j = 0; j < ENEMY_NUM; j++)
		{
			if (enemy[i][j].life == false)
				continue;
			if ((x == enemy[i][j].x) && (y == enemy[i][j].y))
			{
				damage = Damage(player.atk + weapon[player.weapon].skil, enemy[i][j].def);
				enemy[i][j].hp -= damage;
				DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sの攻撃\n%sに%dのダメージを与えた\n"
					, player.name, enemy[i][j].name, damage);
				WaitTimer(MES_TIME);
				if (enemy[i][j].hp <= 0)
				{
					enemy[i][j].life = false;
					map_stay[enemy[i][j].y][enemy[i][j].x] = CELL_TYPE_NONE;
					player.exp += enemy[i][j].exp;
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは倒れた\n経験値を%d手に入れた"
						, enemy[i][j].name, enemy[i][j].exp);
					WaitTimer(MES_TIME);
					if (player.exp >= 10 * ((Index(1.8, level) - 1) / 0.8))
					{
						level++;
						player.hp += 4 + level / 3 + (level / 5) * 10;
						player.hpMax += 4 + level / 3 + (level / 5) * 10;
						player.atk += 2 + level;
						player.def += 2 + level / 3;
						drawField();
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sはLv%dにレベルアップした\n", player.name, level);
						WaitTimer(MES_TIME);
					}
				}
				break;
			}
		}
		
	}
	enemy_act();
	
}

void boss_act()
{
	int x = boss.x;
	int y = boss.y;

	int room = getRoom(boss.x, boss.y);
	int distance = abs(player.x - boss.x) + abs(player.y - boss.y);
	static bool injure = false;

	if (boss.hp != boss.hpMax)
		injure = true;
	if (boss.hp > boss.hpMax)
		boss.hp = boss.hpMax;
	if (injure == false)
		return;
	if (distance == 1)
	{
		int damage;
		drawField();
		damage = Damage(boss.atk, player.def + armor[player.armor].skil);
		player.hp -= damage;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sの攻撃\n%sは%dのダメージを受けた\n"
			, boss.name, player.name, damage);
		WaitTimer(MES_TIME);
	}
	else if (((room >= 0) && (room == getRoom(player.x, player.y))) || (distance == 2))
	{
		map_stay[boss.y][boss.x] = CELL_TYPE_NONE;
		if (x < player.x)
			x++;
		if (x > player.x)
			x--;
		if (y < player.y)
			y++;
		if (y > player.y)
			y--;
		if ((x != boss.x) && (y != boss.y))       //引っ掛かり防止
		{
			if (field[boss.y][x] == CELL_TYPE_WALL)
				x = boss.x;
			else if (field[y][boss.x] == CELL_TYPE_WALL)
				y = boss.y;
			else
			{
				if (GetRand(1))
					x = boss.x;
				else
					y = boss.y;
			}
		}
		else if (field[y][x] != CELL_TYPE_WALL)
		{
			boss.x = x;
			boss.y = y;
		}
		map_stay[boss.y][boss.x] = CELL_TYPE_BOSS;
	}
}

void Turn()
{
	static int countdown = 10;

	turn++;
	if (player.hung != 0)
	{
		player.hung--;
		if (player.state == AS_NONE)
			player.hp++;
	}
	if (player.hung == 0)
	{
		player.hp--;
	}
	if ((player.state == AS_POISON) && (turn % 5 == 0))
	{
		player.hp -= player.hpMax / 16;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは毒のダメージを受けた\n", player.name);
		WaitTimer(MES_TIME);
	}
	if (player.state == AS_SLEEP)
	{
		static int sleep = 0;
		sleep++;
		if ((sleep == 5) || (GetRand(4) == 0))
		{
			player.state = AS_NONE;
			drawField();
			DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは目覚めた\n", player.name);
			WaitTimer(MES_TIME);
			sleep = 0;
			return;
		}
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは眠っている\n", player.name);
		WaitTimer(MES_TIME);
	}
	if (player.state == AS_CURSE)
	{
		if (countdown == 0)
		{
			drawField();
			DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sの寿命が尽きた\n", player.name);
			WaitTimer(MES_TIME);
			player.hp = 0;
		}
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "死まであと%dターン\n", countdown);
		WaitTimer(MES_TIME);
		countdown--;
	}
	else
	{
		countdown = 10;
	}
	if (player.state == AS_CONFUSION)
	{
		static int confu = 0;
		confu++;
		if ((confu == 10) || (GetRand(19) == 0))
		{
			player.state = AS_NONE;
			drawField();
			DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは正気に戻った\n", player.name);
			WaitTimer(MES_TIME);
			confu = 0;
			return;
		}
	}
}

void spec(int i,int j)
{
	int damage;

	switch (i)
	{
	case ENEMY_FIRE:
		damage = Damage(60, player.def + armor[player.armor].skil);
		player.hp -= damage;
		enemy[i][j].life = false;
		map_stay[enemy[i][j].y][enemy[i][j].x] = CELL_TYPE_NONE;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは自爆した\n%sは%dのダメージを受けた\n"
			, enemy[i][j].name, player.name, damage);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_JELLYFISH:
		player.state = AS_ELEC;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは電流を流してきた\n%sは感電した\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_DEMON:
		player.state = AS_POISON;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは毒の息を吐いた\n%sは毒を浴びた\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_KESAPASA:
		player.state = AS_SLEEP;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは謎の呪文を唱えた\n%sは眠ってしまった\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_GORGON:
		player.state = AS_CURSE;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは石化の呪いをかけてきた\n%sは呪われた\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_ELF:
		player.state = AS_CONFUSION;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは幻影を見せた\n%sは混乱した\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_GRIFFON:
		player.atk -= 5;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは恐ろしい鳴き声を発した\n%sは恐怖で力が入らなくなった\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	case ENEMY_DRAGON:
		player.def -= 2;
		drawField();
		DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sは吹雪を起こした\n%sは寒さで守りが弱くなった\n"
			, enemy[i][j].name, player.name);
		WaitTimer(MES_TIME);
		break;
	}
}