#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include "DxLib.h"
using namespace std;

#define RED GetColor(255,0,0)
#define GREEN GetColor(0,255,0)
#define BLUE GetColor(0,0,255)
#define YELLOW GetColor(255,255,0)
#define BLACK GetColor(0,0,0)
#define WHITE GetColor(255,255,255)
#define BEIGE GetColor(255,204,153)

#define WIDTH 700        //横幅
#define HEIGHT 600       //縦幅
#define MODE_N 4         //モード数
#define MES_TIME 2000    //メッセージの表示時間
#define GOAL_FLOOR 10    //ゴールのフロア
#define KEY 29           //キー
#define PIXEL 60         //ピクセル
#define SCREEN_RANGE 4   //画面に表示するマップの大きさ
#define FIELD_WIDTH 40                    //マップの横幅
#define FIELD_HEIGHT (FIELD_WIDTH/2)      //マップの縦幅
#define AREA_MAX 32                       //エリアの最大個数
#define AREA_SIZE_MIN (FIELD_HEIGHT/2)    //エリアの最小サイズ

#define ENEMY_NUM_MAX 10                  //モンスターの１フロアあたりの最大数
#define ENEMY_NUM 3                       //モンスター１種類の１フロアあたりの最大数
#define EQUIP_KIND 10                     //武器と防具の種類
#define ITEM_NUM_MAX 10                   //アイテムの１フロアあたりの最大数
#define ITEM_NUM_MIN 6                    //アイテムの１フロアあたりの最小数
#define ITEM_NUM 3                        //アイテム１種類の１フロアあたりの最大数
#define HAVE_NUM_MAX 20                   //所持できるアイテムの最大数

typedef enum {
	CELL_TYPE_NONE,
	CELL_TYPE_WALL,
	CELL_TYPE_STAIRS,
	CELL_TYPE_WEAPON,
	CELL_TYPE_ARMOR,
	CELL_TYPE_HEAL,
	CELL_TYPE_MUSH,
	CELL_TYPE_BOOK,
	CELL_TYPE_PLAYER,
	CELL_TYPE_DEAD,
	CELL_TYPE_BAT,
	CELL_TYPE_WOLF,
	CELL_TYPE_FIRE,
	CELL_TYPE_JELLYFISH,
	CELL_TYPE_DEMON,
	CELL_TYPE_KESAPASA,
	CELL_TYPE_GORGON,
	CELL_TYPE_ELF,
	CELL_TYPE_GRIFFON,
	CELL_TYPE_DRAGON,
	CELL_TYPE_BOSS,
	END_CELL
}CELL;
typedef enum {
	ENEMY_BAT,
	ENEMY_WOLF,
	ENEMY_FIRE,
	ENEMY_JELLYFISH,
	ENEMY_DEMON,
	ENEMY_KESAPASA,
	ENEMY_GORGON,
	ENEMY_ELF,
	ENEMY_GRIFFON,
	ENEMY_DRAGON,
	BOSS,
	ENEMY_KIND
}E_ENEMY;
typedef enum {
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_HEAL,
	ITEM_MUSH,
	ITEM_BOOK,
	ITEM_KIND
}E_ITEM;
typedef enum {
	NORTH,
	SOUTH,
	WEST,
	EAST,
}WAY;
typedef enum {
	MENU_NONE,
	MENU_BASIC,
	MENU_HAVE,
	MENU_ITEM,
}MENU;
typedef enum {
	AS_NONE,
	AS_SLEEP,
	AS_POISON,
	AS_ELEC,
	AS_CONFUSION,
	AS_CURSE,
	AS_KIND
}AS;

typedef struct{
	int x;
	int y;
}Pos;
typedef struct {
	int x;
	int y;
	int w;
	int h;
}ROOM;
typedef struct {
	int x;
	int y;
	int w;
	int h;
	ROOM room;
}AREA;
typedef struct {
	char name[50];
	int skil;
}DEF_I;
typedef struct {
	char name[50];
	int hpMax;
	int atk;
	int def;
	int exp;
}DEF_E;
typedef struct {
	char name[50];
	int x;
	int y;
	bool stay;
	int item;
	int kind;
	int pow;
}ITEM;
typedef struct {
	char name[50];
	int x;
	int y;
	int hp;
	int hpMax;
	int hung;
	int hungMax;
	int atk;
	int def;
	int way;
	int exp;
	bool life;
	AS state;
	int stay;
	int weapon;
	int armor;
}CHARACTER;
class Chara
{
public:
	char name[50];
	int x, y;
	int hp, hpMax;
	int hung, hungMax;
	int atk, def;
	int way;
	int exp;
	bool life;
	AS state;
	int stay;
	int weapon, armor;

	//Chara();
	//~Chara();
	//void Act();
};
class Player :public Chara
{

};

extern AREA areas[AREA_MAX];
extern DEF_I weapon[EQUIP_KIND];
extern DEF_I armor[EQUIP_KIND];
extern vector<DEF_I> heal;
extern vector<DEF_I> mush;
extern vector<DEF_I> book;
extern ITEM item[ITEM_KIND][ITEM_NUM + HAVE_NUM_MAX];
extern ITEM have_item[HAVE_NUM_MAX];
extern DEF_E enemy_def[ENEMY_KIND];
extern CHARACTER player;
extern CHARACTER enemy[ENEMY_KIND][ENEMY_NUM];
extern CHARACTER boss;
extern int areaCount;
extern int field[FIELD_HEIGHT][FIELD_WIDTH];
extern int map_stay[FIELD_HEIGHT][FIELD_WIDTH];
extern int item_stay[FIELD_HEIGHT][FIELD_WIDTH];
extern int item_num[ITEM_KIND];
extern int floor_number;
extern int turn;                          //
extern int level;                         //
extern int have_num;                      //   
extern int stairX;                        //
extern int stairY;                        //
extern int cellAA[END_CELL];
extern char ASname[AS_KIND][50];

extern int titleHandle;
extern int keyHandle;
extern int staffHandle;
extern int helpHandle[2];

void LoadData();
void gameInit();
void gameover();
void gameclear();
void CharaInit();
void drawField();
void spritArea(int oldAreaIndex);
void generateField();
void setRandPosition(int& pX, int& pY);
void player_act(char buf[]);
void enemy_act();
void boss_act();
void enemyInit();
void itemInit();
void bossInit();
void attack(int y,int x);
void stair(char buf[]);
void command(MENU& menu, char buf[]);
void pickup();
void Turn();
void spec(int i, int j);

int title(char buf[]);
int story(char buf[]);
int staffed_roll(char buf[]);
int option(char buf[]);
int help(char buf[]);
int getRoom(int _x, int _y);
int Damage(int atk, int def);

double Index(double x, int n);
void swap(ITEM& a, ITEM& b);