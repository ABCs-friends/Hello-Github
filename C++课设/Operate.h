#pragma once


#include "Game.h"


class Operate:public Game {

private:

	bool LEFT;     //左移
	bool RIGHT;    //右移
	bool DOWN;     //软降
	bool UP;       //转向
	bool zKey;    //硬降
	bool xKey;     //hold
	bool ESC;

	bool now[7] = { 0 };       //存储这一帧的键盘状态
	bool last[7] = { 0 };      //存储上一帧的键盘状态

	unsigned long last_tick;   //getKey()要用

public:

	Operate() :LEFT(0), RIGHT(0), UP(0), DOWN(0), zKey(0), xKey(0), ESC(0) {
		last_tick = GetTickCount();
	};

	//读取键盘输入
	bool getKey();

	//方块操作
	void moveLeft();
	void moveRight();
	void moveDown();
	void turn();
	bool launch();
	void hold();

	//落地锁定
	bool landLock();

	//暂停
	bool pause();
};