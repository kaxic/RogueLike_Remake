#include "def.hpp"

void stair(char buf[])
{
	static int key_pos = 0;
	DrawBox(0, 0, 120, KEY * 2, GREEN, TRUE);
	DrawGraph(0, key_pos, keyHandle, TRUE);
	DrawFormatString(KEY, 0, YELLOW, "ç~ÇËÇÈ\n");
	DrawFormatString(KEY, KEY, YELLOW, "ç~ÇËÇ»Ç¢\n");
	if ((buf[KEY_INPUT_DOWN] == 1) || (buf[KEY_INPUT_UP] == 1))
	{
		key_pos += KEY;
		key_pos = key_pos % (KEY * 2);
		WaitTimer(200);
	}
	else if (buf[KEY_INPUT_RETURN] == 1)
	{
		if (key_pos == 0)
		{
			floor_number++;
			Turn();
			ClearDrawScreen();
			DrawBox(0, 0, WIDTH, HEIGHT, BLACK, TRUE);
			SetFontSize(50);
			DrawFormatString(250, 250, WHITE, "ínâ∫%dÇe", floor_number);
			WaitTimer(1500);
			generateField();
		}
		key_pos = 0;
		player.stay = CELL_TYPE_NONE;
	}
	
}

void command(MENU& menu, char buf[])
{
	static int key_pos = 0;
	static int have_pos;
	static ITEM select_item = { 0 };
	select_item.x = player.x;
	select_item.y = player.y;
	
	switch (menu)
	{
	case MENU_BASIC:
		if (buf[KEY_INPUT_BACK] == 1)
		{
			menu = MENU_NONE;
			key_pos = 0;
			return;
		}
		DrawBox(0, 0, 120, KEY * 2, GREEN, TRUE);
		DrawGraph(0, key_pos, keyHandle, TRUE);
		DrawFormatString(KEY, 0, YELLOW, "çUåÇ\n");
		DrawFormatString(KEY, KEY, YELLOW, "éùÇøï®\n");
		//DrawFormatString(KEY, KEY * 2, YELLOW, "ãZ\n");
		if (buf[KEY_INPUT_DOWN] == 1)
		{
			key_pos += KEY;
			key_pos = key_pos % (KEY * 2);
			WaitTimer(200);
		}
		else if (buf[KEY_INPUT_UP] == 1)
		{
			key_pos += KEY;
			key_pos = key_pos % (KEY * 2);
			WaitTimer(200);
		}
		else if (buf[KEY_INPUT_RETURN] == 1)
		{
			switch (key_pos / KEY)
			{
			case 0:
				if (player.state == AS_CONFUSION)
				{
					player.way = GetRand(EAST);
				}
				drawField();
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
				menu = MENU_NONE;
				break;
			case 1:
				menu = MENU_HAVE;
				break;
			case 2:
				menu = MENU_NONE;
				break;
			}
			key_pos = 0;
		}
		break;
	case MENU_HAVE:
		int i;
		if (buf[KEY_INPUT_BACK] == 1)
		{
			menu = MENU_BASIC;
			key_pos = 0;
			return;
		}
		DrawBox(0, 0, 400, KEY * (HAVE_NUM_MAX / 2), GREEN, TRUE);
		DrawGraph(200 * ((key_pos / KEY) / (HAVE_NUM_MAX / 2)), key_pos % (KEY*(HAVE_NUM_MAX / 2)), keyHandle, TRUE);
		DrawFormatString(KEY, 0, YELLOW, "%sÅö\n", weapon[player.weapon].name);
		DrawFormatString(KEY, KEY, YELLOW, "%sÅö\n", armor[player.armor].name);
		for (i = 2; i < have_num; i++)
		{
			DrawFormatString(KEY + 200 * (i / (HAVE_NUM_MAX / 2)), KEY * (i % (HAVE_NUM_MAX / 2)), YELLOW, "%s\n"
				, have_item[i].name);
		}

		if (buf[KEY_INPUT_DOWN] == 1)
		{
			key_pos += KEY;
			key_pos = key_pos % (KEY * have_num);
			WaitTimer(200);
		}
		else if (buf[KEY_INPUT_UP] == 1)
		{
			key_pos += KEY * (have_num - 1);
			key_pos = key_pos % (KEY * have_num);
			WaitTimer(200);
		}
		else if (buf[KEY_INPUT_RETURN] == 1)
		{
			if (key_pos / KEY <= 1)
			{
				break;
			}
			menu = MENU_ITEM;
			select_item = have_item[key_pos / KEY];
			have_pos = key_pos / KEY;
			key_pos = 0;
		}
		break;
	case MENU_ITEM:
		if (buf[KEY_INPUT_BACK] == 1)
		{
			menu = MENU_HAVE;
			key_pos = 0;
			return;
		}
		DrawBox(0, 0, 120, KEY * 3, GREEN, TRUE);
		DrawGraph(0, key_pos, keyHandle, TRUE);
		switch (select_item.item)
		{
		case ITEM_WEAPON:
		case ITEM_ARMOR:
			DrawFormatString(KEY, 0, YELLOW, "ëïîı\n");
			break;
		case ITEM_HEAL:
			DrawFormatString(KEY, 0, YELLOW, "égÇ§\n");
			break;
		case ITEM_MUSH:
			DrawFormatString(KEY, 0, YELLOW, "êHÇ◊ÇÈ\n");
			break;
		case ITEM_BOOK:
			DrawFormatString(KEY, 0, YELLOW, "ì«Çﬁ\n");
			break;
		}
		DrawFormatString(KEY, KEY, YELLOW, "íuÇ≠\n");
		DrawFormatString(KEY, KEY * 2, YELLOW, "ìäÇ∞ÇÈ\n");
		if (buf[KEY_INPUT_DOWN] == 1)
		{
			key_pos += KEY;
			key_pos = key_pos % (KEY * 3);
			WaitTimer(200);
		}
		else if (buf[KEY_INPUT_UP] == 1)
		{
			key_pos += KEY * 2;
			key_pos = key_pos % (KEY * 3);
			WaitTimer(200);
		}
		else if (buf[KEY_INPUT_RETURN] == 1)
		{
			menu = MENU_NONE;
			switch (key_pos / KEY)
			{
			case 0:
				int r;
				drawField();
				if (select_item.item > ITEM_ARMOR)
				{
					have_num--;
					for (i = have_pos; i < have_num; i++)
					{
						have_item[i] = have_item[i + 1];
					}
				}
				switch (select_item.item)
				{
				case ITEM_WEAPON:
					player.weapon = select_item.kind;
					break;
				case ITEM_ARMOR:
					player.armor = select_item.kind;
					break;
				case ITEM_HEAL:
					player.hp += heal[select_item.kind].skil;
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇégÇ¡ÇΩ\n", select_item.name);
					WaitTimer(MES_TIME);
					break;
				case ITEM_MUSH:
					r = GetRand(10);
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇêHÇ◊ÇΩ\n", select_item.name);
					WaitTimer(MES_TIME);
					if (select_item.kind == 2)
						switch (r)
						{
						case 0:
							player.hp = player.hpMax;
							break;
						case 1:
							player.hung = player.hungMax;
							break;
						case 2:
							player.hp = 1;
							break;
						case 3:
							player.hung = 0;
							break;
						case 4:
							player.state = AS_POISON;
							break;
						case 5:
							player.state = AS_SLEEP;
							break;
						default:
							break;
						}
					else
					    player.hung += mush[select_item.kind].skil;
					break;
				case ITEM_BOOK:
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇì«ÇÒÇæ\n", select_item.name);
					WaitTimer(MES_TIME);
					switch (select_item.kind)
					{
					case 0:
						player.hp = player.hpMax;
						break;
					case 1:
						player.hung = player.hungMax;
						break;
					case 2:
						map_stay[player.y][player.x] = CELL_TYPE_NONE;
						setRandPosition(player.x, player.y);
						map_stay[player.y][player.x] = CELL_TYPE_PLAYER;
						player.stay = field[player.y][player.x];
						break;
					case 3:
						player.state = AS_NONE;
						break;
					case 4:
						map_stay[player.y][player.x] = CELL_TYPE_NONE;
						player.x = stairX;
						player.y = stairY;
						map_stay[player.y][player.x] = CELL_TYPE_PLAYER;
						player.stay = field[player.y][player.x];
						break;
					case 5:
						player.atk += 5;
						break;
					case 6:
						player.def += 4;
						break;
					}
					break;
				}
				drawField();
				switch (select_item.item)
				{
				case ITEM_WEAPON:
				case ITEM_ARMOR:
					swap(have_item[select_item.item], have_item[have_pos]);
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇëïîıÇµÇΩ\n", select_item.name);
					WaitTimer(MES_TIME);
					break;
				case ITEM_HEAL:
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "ÇgÇoÇ™%dâÒïúÇµÇΩ\n"
						, heal[select_item.kind].skil);
					WaitTimer(MES_TIME);
					break;
				case ITEM_MUSH:
					if (select_item.kind == 2)
						switch (r)
						{
						case 0:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "ÇgÇoÇ™ëSâÒïúÇµÇΩ\n");
							WaitTimer(MES_TIME);
							break;
						case 1:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "Ç®Ç»Ç©Ç™ëSâÒïúÇµÇΩ\n");
							WaitTimer(MES_TIME);
							break;
						case 2:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "ÇgÇoÇ™ÇPÇ…Ç»Ç¡ÇΩ\n");
							WaitTimer(MES_TIME);
							break;
						case 3:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "Ç®Ç»Ç©Ç™ÇOÇ…Ç»Ç¡ÇΩ\n");
							WaitTimer(MES_TIME);
							break;
						case 4:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕì≈ÇóÅÇ—ÇΩ\n", player.name);
							WaitTimer(MES_TIME);
							break;
						case 5:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕñ∞Ç¡ÇƒÇµÇ‹Ç¡ÇΩ\n", player.name);
							WaitTimer(MES_TIME);
							break;
						default:
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "âΩÇ‡ãNÇ´Ç»Ç©Ç¡ÇΩ\n");
							WaitTimer(MES_TIME);
							break;
						}
					else
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "Ç®Ç»Ç©Ç™%dâÒïúÇµÇΩ\n"
							, mush[select_item.kind].skil);
					WaitTimer(MES_TIME);
					break;
				case ITEM_BOOK:
					switch (select_item.kind)
					{
					case 0:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "ÇgÇoÇ™ëSâÒïúÇµÇΩ\n");
						WaitTimer(MES_TIME);
						break;
					case 1:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "Ç®Ç»Ç©Ç™ëSâÒïúÇµÇΩ\n");
						WaitTimer(MES_TIME);
						break;
					case 2:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕÉèÅ[ÉvÇµÇΩ\n", player.name);
						WaitTimer(MES_TIME);
						break;
					case 3:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "èÛë‘àŸèÌÇ™âÒïúÇµÇΩ\n");
						WaitTimer(MES_TIME);
						break;
					case 4:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "äKíiÇå©Ç¬ÇØÇΩ\n");
						WaitTimer(MES_TIME);
						break;
					case 5:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "çUåÇóÕÇ™è„Ç™Ç¡ÇΩ\n");
						WaitTimer(MES_TIME);
						break;
					case 6:
						DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "ñhå‰óÕÇ™è„Ç™Ç¡ÇΩ\n");
						WaitTimer(MES_TIME);
						break;
					}
					break;
				}
				break;
			case 1:
				have_num--;
				for (i = have_pos; i < have_num; i++)
				{
					have_item[i] = have_item[i + 1];
				}
				if ((item_stay[player.y][player.x] != CELL_TYPE_NONE) || (field[player.y][player.x] == CELL_TYPE_STAIRS))
				{
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "Ç±Ç±Ç…ÇÕíuÇØÇ»Ç¢\n", player.name);
					WaitTimer(MES_TIME);
				}
				else
				{
					item_stay[player.y][player.x] = CELL_TYPE_WEAPON + select_item.item;
					item[select_item.item][item_num[select_item.item]] = select_item;
					item_num[select_item.item]++;
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇë´å≥Ç…íuÇ¢ÇΩ\n", select_item.name);
					WaitTimer(MES_TIME);
				}
				break;
			case 2:
				have_num--;
				for (i = have_pos; i < have_num; i++)
				{
					have_item[i] = have_item[i + 1];
				}
				int x = SCREEN_RANGE;
				int y = SCREEN_RANGE;
				while ((field[select_item.y][select_item.x] == CELL_TYPE_NONE) &&
					(map_stay[select_item.y][select_item.x] < CELL_TYPE_BAT))
				{
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕ%sÇìäÇ∞ÇΩ\n", player.name, select_item.name);
					switch (player.way)
					{
					case NORTH:
						select_item.y--;
						y--;
						break;
					case SOUTH:
						select_item.y++;
						y++;
						break;
					case WEST:
						select_item.x--;
						x--;
						break;
					case EAST:
						select_item.x++;
						x++;
						break;
					}
					DrawExtendGraph(x*PIXEL, y*PIXEL, (x + 1)*PIXEL, (y + 1)*PIXEL, cellAA[CELL_TYPE_WEAPON + select_item.item], TRUE);
					WaitTimer(200);
				}
				WaitTimer(MES_TIME);
				if (field[select_item.y][select_item.x] == CELL_TYPE_WALL)
				{
					drawField();
					DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕï«Ç…Ç‘Ç¬Ç©Ç¡Çƒè¡Ç¶ÇΩ\n"
						, select_item.name);
					WaitTimer(MES_TIME);
				}
				else
				{
					int a, b;
					a = map_stay[select_item.y][select_item.x] - CELL_TYPE_BAT;

					if (a == BOSS)
					{
						boss.hp -= select_item.pow;
						drawField();
						if (select_item.pow >= 0)
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK,
								"%sÇÕ%sÇ…Ç‘Ç¬Ç©Ç¡ÇΩ\n%sÇ…%dÇÃÉ_ÉÅÅ[ÉWÇó^Ç¶ÇΩ\n"
								, select_item.name, boss.name, boss.name, select_item.pow);
						else
							DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK,
								"%sÇÕ%sÇ…Ç‘Ç¬Ç©Ç¡ÇΩ\n%sÇÃÇgÇoÇ™%dâÒïúÇµÇΩ\n"
								, select_item.name, boss.name, boss.name, -select_item.pow);
						WaitTimer(MES_TIME);
						boss_act();
					}
					else
					{
						for (b = 0; b < ENEMY_NUM; b++)
						{
							if (enemy[a][b].life == false)
								continue;
							if ((select_item.x == enemy[a][b].x) && (select_item.y == enemy[a][b].y))
							{
								enemy[a][b].hp -= select_item.pow;
								drawField();
								if (select_item.pow >= 0)
									DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK,
										"%sÇÕ%sÇ…Ç‘Ç¬Ç©Ç¡ÇΩ\n%sÇ…%dÇÃÉ_ÉÅÅ[ÉWÇó^Ç¶ÇΩ\n"
										, select_item.name, enemy[a][b].name, enemy[a][b].name, select_item.pow);
								else
									DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK,
										"%sÇÕ%sÇ…Ç‘Ç¬Ç©Ç¡ÇΩ\n%sÇÃÇgÇoÇ™%dâÒïúÇµÇΩ\n"
										, select_item.name, enemy[a][b].name, enemy[a][b].name, -select_item.pow);
								WaitTimer(MES_TIME);
								if (enemy[a][b].hp <= 0)
								{
									enemy[a][b].life = false;
									map_stay[enemy[a][b].y][enemy[a][b].x] = CELL_TYPE_NONE;
									player.exp += enemy[a][b].exp;
									drawField();
									DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕì|ÇÍÇΩ\nåoå±ílÇ%déËÇ…ì¸ÇÍÇΩ"
										, enemy[a][b].name, enemy[a][b].exp);
									WaitTimer(MES_TIME);
									if (player.exp >= 10 * ((Index(1.8, level) - 1) / 0.8))
									{
										level++;
										player.hp += 5;
										player.hpMax += 5;
										player.atk += 4;
										player.def += 3;
										drawField();
										DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sÇÕLv%dÇ…ÉåÉxÉãÉAÉbÉvÇµÇΩ\n", player.name, level);
										WaitTimer(MES_TIME);
									}
								}
								break;
							}
						}
						enemy_act();
					}	
				}
				break;
			}
			key_pos = 0;
		}
		break;
	}
}