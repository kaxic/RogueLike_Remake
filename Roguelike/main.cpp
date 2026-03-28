////////////////////////////////////////////////
//
//  j1806 柿田浩佑
//  
//  参考文献
//  https://youtu.be/xEA-K5HSw08
//  
////////////////////////////////////////////////

#include "def.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char buf[256];
	char gamemode = 0;

	SetMainWindowText("柿田の春休み");
	ChangeWindowMode(TRUE);
	SetGraphMode(WIDTH, HEIGHT, 32);

	if (DxLib_Init() == -1) { return -1; }			// ＤＸライブラリ初期化処理

	gameInit();
	LoadData();

	while (ProcessMessage() == 0 && gamemode != 9)
	{
		DrawBox(0, 0, WIDTH, HEIGHT, BLACK, TRUE);
		GetHitKeyStateAll(buf);

		switch (gamemode)
		{
		case 0:
			gamemode = title(buf);  //タイトル画面
			break;
		case 1:
			gamemode = story(buf);  //ゲーム
			break;
		case 2:
			gamemode = staffed_roll(buf);  //スタッフロール
			break;
		case 3:
			gamemode = option(buf);  //オプション
			break;
		case 4:
			gamemode = help(buf);  //ヘルプ
			break;
		}
		ScreenFlip();
	}

	
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了
}