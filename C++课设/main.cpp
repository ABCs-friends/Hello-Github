#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#include <easyx.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "Game.h"
#include "Block.h"
#include "Operate.h"
#include "Menu.h"


int main() {
	
	//创建游戏窗口
	initgraph(550, 680);

	while (true) {

		//游戏菜单
		Menu* menu = new Menu;
		menu->drawMenu();

		//菜单界面循环
		while (true) {

			menu->getKey();
			menu->drawCursor();

			//进入、退出游戏
			if (menu->confirm() == 0) {
				break;
			}
			else if (menu->confirm() == 2) {
				return 0;
			}

			//进入历史记录界面
			else if (menu->confirm() == 1) {

				cleardevice();
				menu->drawHistory();
				menu->drawData();

				//历史记录界面循环
				while (true) {

					//退出历史记录界面
					menu->getKey();
					if (menu->back() == true) {
						menu->drawMenu();
						break;
					}

					Sleep(20);
				}
			}

			Sleep(20);
		}

		//进入游戏
		delete menu;
		Sleep(200);
		cleardevice();

		//创建对象、生成种子
		Operate* operate = new Operate;
		srand((unsigned)time(nullptr));
		operate->start();

		//游戏一帧循环
		while (true) {

			//读键盘
			operate->getKey();

			//执行操作
			operate->moveLeft();
			operate->moveRight();
			operate->moveDown();
			operate->hold();
			operate->turn();

			//游戏暂停
			if (operate->pause() == true) {
				delete operate;
				Sleep(200);
				break;
			}

			//硬降
			if (operate->launch() == true) {
				operate->writeMap();
				operate->clearLine();
				operate->creatBag();
				operate->creatBlock();
			}

			else if (operate->launch() == false) {

				//检查方块落地
				if (operate->checkDrop() == true) {
					if (operate->landLock() == true) {

						operate->writeMap();
						operate->clearLine();
						operate->creatBag();
						operate->creatBlock();
					}
				}
			}

			//检查游戏结束
			if (operate->checkLose() == true) {
				delete operate;
				break;
			}

			//更新状态
			operate->updateTime();
			operate->ghostBlock();
			operate->dropBlock();

			//绘制游戏
			operate->drawGame();
			operate->drawBlock();

			Sleep(20);
		}
	}

	return 0;
}