#include "def.hpp"

//初期化
void gameInit()
{
	turn = 0;
	level = 1;
	have_num = 2;
	for (int i = 0; i < 2; i++)
	{
		have_item[i].item = i;
		have_item[i].kind = 0;
		if (i == 0)
		{
			strcpy_s(have_item[i].name, 50, weapon[0].name);
			have_item[i].pow = 15 + weapon[0].skil*5.5;
		}
		else
		{
			strcpy_s(have_item[i].name, 50, armor[0].name);
			have_item[i].pow = 10 + armor[0].skil * 4;
		}
	}
	
	SRand((unsigned int)time(NULL));
	generateField();

	strcpy_s(player.name, 50, "勇者");
	player.hp = player.hpMax = 40;
	player.hung = player.hungMax = 1000;
	player.atk = 10;
	player.def = 5;
	player.way = SOUTH;
	player.life = true;
	player.state = AS_NONE;
	player.exp = 0;
	player.stay = CELL_TYPE_NONE;
	player.weapon = player.armor = 0;

	strcpy_s(boss.name, 50, "？？？");
	boss.hp = boss.hpMax = 500;
	boss.atk = 200;
	boss.def = 30;
	boss.life = true;
}

//タイトル画面
int title(char buf[])
{
	static int key_pos = 0;
	DrawGraph(0, 0, titleHandle, TRUE);
	DrawGraph(260, key_pos + 308, keyHandle, TRUE);
	
	if (buf[KEY_INPUT_DOWN] == 1)
	{
		key_pos += KEY;
		key_pos = key_pos % (KEY * MODE_N);
		WaitTimer(200);
	}
	else if (buf[KEY_INPUT_UP] == 1)
	{
		key_pos += KEY * (MODE_N - 1);
		key_pos = key_pos % (KEY * MODE_N);
		WaitTimer(200);
	}
	else if (buf[KEY_INPUT_RETURN] == 1)
		return key_pos / KEY + 1;
	return 0;
}

int staffed_roll(char buf[])
{
	static int mes_pos = HEIGHT;
	
	DrawBox(0, 0, WIDTH, HEIGHT, BLACK, TRUE);
	DrawGraph(250, mes_pos, staffHandle, FALSE);
	mes_pos--;
	WaitTimer(20);

	if (buf[KEY_INPUT_BACK] == 1 || mes_pos < -100)
	{
		mes_pos = HEIGHT;
		return 0;
	}
	if (buf[KEY_INPUT_ESCAPE] == 1)
		return 9;
	return 2;
}

int option(char buf[])
{
	SetFontSize(60);
	DrawFormatString(0, 0, WHITE, "オプションは\n作れませんでした");
	if (buf[KEY_INPUT_BACK] == 1)
		return 0;
	if (buf[KEY_INPUT_ESCAPE] == 1)
		return 9;
	return 3;
}

int help(char buf[])
{
	static int i = 0;
	if (i == 0)
	{
		DrawGraph(0, 0, helpHandle[0], TRUE);
		if (buf[KEY_INPUT_RIGHT] == 1)
			i++;
	}
	else
	{
		DrawGraph(0, 0, helpHandle[1], TRUE);
		if (buf[KEY_INPUT_LEFT] == 1)
			i--;
	}
	return 4;
}

void gameover()
{
	DrawBox(0, 0, WIDTH, HEIGHT, BLACK, TRUE);
	SetFontSize(55);
	DrawFormatString(100, 250, RED, "ＧＡＭＥ　ＯＶＥＲ");
	WaitTimer(3000);
	gameInit();
}

void gameclear()
{
	DrawBox(0, 0, WIDTH, HEIGHT, YELLOW, TRUE);
	SetFontSize(40);
	DrawFormatString(50, 250, RED, "Ｃｏｎｇｒａｔｕｌａｔｉｏｎ！！");
	WaitTimer(3000);
	gameInit();
}

double Index(double x,int n)
{
	if (n == 0)
		return 1;
	return x * Index(x, n - 1);
}

void swap(ITEM& a, ITEM& b)
{
	ITEM tmp;
	tmp = a;
	a = b;
	b = tmp;
	return;
}