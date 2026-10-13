#pragma once


class Menu {

private:

	bool DOWN;     //下移
	bool UP;       //上移
	bool zKey;     //确定
	bool xKey;     //返回

	bool now[4] = { 0 };       //存储这一帧的键盘状态
	bool last[4] = { 0 };      //存储上一帧的键盘状态

	int cursor;    //存储光标状态

public:

	Menu() :DOWN(0), UP(0), zKey(0), xKey(0), cursor(0) {}

	void drawMenu();
	void drawCursor();

	int confirm();
	bool back();

	bool getKey();

	void drawHistory();
	void drawData();
};