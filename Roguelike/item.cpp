#include "def.hpp"

template<typename TYPE, int SIZE>
int array(const TYPE(&)[SIZE])
{
	return SIZE;
}

DEF_I weapon[EQUIP_KIND]=
{
{"ナイフ",          1},
{"サーベル",        3},
{"カットラス",      5},
{"日本刀",          8},
{"蜻蛉切",          10},
{"狼牙棒",          12},
{"トマホーク",      15},
{"レーバテイン",    17},
{"Ⅹカリバー",      20},
{"天叢雲剣",        30}
};
DEF_I armor[EQUIP_KIND]=
{
{"マント",          1},
{"トラの毛皮",      3},
{"防弾チョッキ",    5},
{"ブロンズの鎧",    6},
{"鉄の鎧",          8},
{"ダイヤの鎧",      10},
{"光のマント",      13},
{"天女の羽衣",      15},
{"龍の鱗",          18},
{"不死鳥の羽",      20}
};
vector<DEF_I> heal=
{
{"回復薬ＸＳ",      20},
{"回復薬Ｓ",        40},
{"回復薬Ｍ",        60},
{"回復薬Ｌ",        80},
{"回復薬ＸＬ",     100},
};
vector<DEF_I> mush=
{
{"キノコ",         200},
{"おおきなキノコ", 500},
{"不思議なキノコ", 100},
};

vector<DEF_I> book=
{
{"癒しの魔導書"},
{"満腹の魔導書"},
{"ワープの魔導書"},
{"浄化の魔導書"},
{"階段の魔導書"},
{"攻めの魔導書"},
{"守りの魔導書"}
};

ITEM item[ITEM_KIND][ITEM_NUM + HAVE_NUM_MAX];
int item_num[ITEM_KIND];
//int item_num_max[ITEM_KIND] = { 3,3,2,2 };

void itemInit()
{
	int i, j;
	int l;
	int item_total;
	int itemCount = 0;

	for (i = 0; i < ITEM_KIND; i++)
		item_num[i] = 0;
	item_total = GetRand(ITEM_NUM_MAX - ITEM_NUM_MIN) + ITEM_NUM_MIN;
	
	while (itemCount < item_total)
	{
		i = GetRand(ITEM_KIND - 1);
		j = item_num[i];
		if ((item[i][j].stay == true) || (item_num[i] == ITEM_NUM))
			continue;
		setRandPosition(item[i][j].x, item[i][j].y);
		if ((field[item[i][j].y][item[i][j].x] == CELL_TYPE_STAIRS))
		{
			j--;
			continue;
		}
		item_stay[item[i][j].y][item[i][j].x] = CELL_TYPE_WEAPON + i;
		item[i][j].stay = true;
		item[i][j].item = i;
		switch (i)
		{
		case ITEM_WEAPON:
		case ITEM_ARMOR:
			l = GetRand(GetRand(floor_number - 1) + GetRand((floor_number / 2)*GetRand(1)) + GetRand(GetRand(GetRand(1))))
				+ GetRand(floor_number / 2) + floor_number / 3;
			if (l >= EQUIP_KIND)
				l = EQUIP_KIND - 1;
			break;
		case ITEM_HEAL:
			l = GetRand(heal.size() - 1);
			break;
		case ITEM_MUSH:
			l = GetRand(mush.size() - 1);
			break;
		case ITEM_BOOK:
			l = GetRand(book.size() - 1);
			break;
		}
		item[i][j].kind = l;
		switch (i)
		{
		case ITEM_WEAPON:
			strcpy_s(item[i][j].name, 50, weapon[l].name);
			item[i][j].pow = 15 + weapon[l].skil*5.5;
			break;
		case ITEM_ARMOR:
			strcpy_s(item[i][j].name, 50, armor[l].name);
			item[i][j].pow = 10 + armor[l].skil * 4;
			break;
		case ITEM_HEAL:
			strcpy_s(item[i][j].name, 50, heal[l].name);
			item[i][j].pow = -heal[l].skil;
			break;
		case ITEM_MUSH:
			strcpy_s(item[i][j].name, 50, mush[l].name);
			item[i][j].pow = -(mush[l].skil / 100);
			break;
		case ITEM_BOOK:
			strcpy_s(item[i][j].name, 50, book[l].name);
			item[i][j].pow = 30;
			break;
		}
		item_num[i]++;
		itemCount++;
	}
}

void pickup()
{
	int i, j;
	i = item_stay[player.y][player.x] - CELL_TYPE_WEAPON;

	for (j = 0; j < item_num[i]; j++)
	{
		if ((player.x == item[i][j].x) && (player.y == item[i][j].y))
		{
			if (have_num == HAVE_NUM_MAX)
			{
				drawField();
				DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "これ以上は持ち歩けない\n");
				WaitTimer(MES_TIME);
			}
			else
			{
				have_item[have_num] = item[i][j];
				item_stay[player.y][player.x] = CELL_TYPE_NONE;
				drawField();
				DrawFormatString(0, SCREEN_RANGE * 2 * PIXEL, BLACK, "%sを拾った\n", item[i][j].name);
				WaitTimer(MES_TIME);
				item_num[i]--;
				for (int a = j; a < item_num[i]; a++)
				{
					item[i][a] = item[i][a + 1];
				}
				item[i][item_num[i]].stay = false;
				have_num++;
			}
			return;
		}
	}
}