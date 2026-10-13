#pragma once


#include <string>
#include <fstream>


class Game {

private:

	int gameTime;   //游戏开始秒数
	int score;      //本次游戏成绩
	int record;     //游戏最高记录
	unsigned long startTick;  //游戏开始时间
	unsigned long lastTick;   //drop()要用

	int locationG[2][4] = { 0 };  //幽灵方块坐标

protected:

	unsigned long gameTick;       //系统运行时间

	int bag[7] = { 0,1,2,3,4,5,6 };
	int bagLast;                  //记录袋中最后一个方块
	int bagNum;                   //下一方块序号
	int type;                     //方块种类
	int state;                    //方块状态

	bool holdBlock[4][4] = { 0 }; //记录hold方块
	int holdType = 0;             //hold形状
	int holdColor = 0;            //hold颜色

	bool map[24][10] = { 0 };     //记录地图
	int location[2][4] = { 0 };   //当前方块坐标 (location[0]为X,location[1]为Y)
	int locBlock[2] = { 0 };      //记录SHAPE左上角的坐标

public:

	Game() :gameTime(0), score(0), bagNum(0), lastTick(0), state(0), locBlock(0), bagLast(0), type(0) {
		startTick = GetTickCount();
		record = 0;
	}

	void start();           //循环前初始化

	bool checkDrop();       //检查下落

	//无法下落时调用
	void writeMap();        //写入地图
	void clearLine();       //检查行、消行、加分
	bool checkLose();       //检查游戏是否结束
	void creatBag();        //更新要生成的方块
	void creatBlock();      //生成方块

	//游戏循环结束时调用
	void updateTime();      //更新游戏时间
	void ghostBlock();      //更新幽灵方块
	void dropBlock();       //方块下落

	void drawGame();        //绘制游戏界面
	void drawBlock();       //绘制方块
};


struct Data {

	int score = 0;
	int time = 0;
	std::string date;

	Data* next = NULL;
};