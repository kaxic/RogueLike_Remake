#include "def.hpp"

int titleHandle;
int keyHandle;
int staffHandle;
int helpHandle[2];
int cellAA[END_CELL];

void LoadData()
{
	titleHandle = LoadGraph("Graphic/title.png");
	keyHandle = LoadGraph("Graphic/key.png");
	staffHandle = LoadGraph("Graphic/staff.png");
	helpHandle[0] = LoadGraph("Graphic/help1.png");
	helpHandle[1] = LoadGraph("Graphic/help2.png");
	cellAA[CELL_TYPE_NONE] = LoadGraph("Graphic/floor.png");
	cellAA[CELL_TYPE_WALL] = LoadGraph("Graphic/block.png");
	cellAA[CELL_TYPE_STAIRS] = LoadGraph("Graphic/stairs.png");
	cellAA[CELL_TYPE_WEAPON] = LoadGraph("Graphic/weapon.png");
	cellAA[CELL_TYPE_ARMOR] = LoadGraph("Graphic/armor.png");
	cellAA[CELL_TYPE_HEAL] = LoadGraph("Graphic/heal.png");
	cellAA[CELL_TYPE_MUSH] = LoadGraph("Graphic/mush.png");
	cellAA[CELL_TYPE_BOOK] = LoadGraph("Graphic/book.png");
	cellAA[CELL_TYPE_PLAYER] = LoadGraph("Graphic/player.png");
	cellAA[CELL_TYPE_DEAD] = LoadGraph("Graphic/player_dead.png");
	cellAA[CELL_TYPE_BAT] = LoadGraph("Graphic/bat.png");
	cellAA[CELL_TYPE_WOLF] = LoadGraph("Graphic/wolf.png");
	cellAA[CELL_TYPE_FIRE] = LoadGraph("Graphic/fire.png");
	cellAA[CELL_TYPE_JELLYFISH] = LoadGraph("Graphic/jellyfish.png");
	cellAA[CELL_TYPE_DEMON] = LoadGraph("Graphic/demon.png");
	cellAA[CELL_TYPE_KESAPASA] = LoadGraph("Graphic/kesaranpasarang.png");
	cellAA[CELL_TYPE_GORGON] = LoadGraph("Graphic/gorgon.png");
	cellAA[CELL_TYPE_ELF] = LoadGraph("Graphic/darkelf.png");
	cellAA[CELL_TYPE_GRIFFON] = LoadGraph("Graphic/griffon.png");
	cellAA[CELL_TYPE_DRAGON] = LoadGraph("Graphic/dragon.png");
	cellAA[CELL_TYPE_BOSS] = LoadGraph("Graphic/boss.png");
}