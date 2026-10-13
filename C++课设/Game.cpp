#include <easyx.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include "Game.h"
#include "Block.h"


void Game::drawGame() {

	//绘制界面
	rectangle(10, 10, 540, 670);    //大框框
	rectangle(39, 39, 341, 641);    //游戏框框
	rectangle(379, 89, 501, 211);   //提示框框
	rectangle(379, 429, 501, 551);  //hold框框

	//绘制文字
	settextstyle(32, 0, "Press Start 2P");
	outtextxy(410, 50, "NEXT");

	outtextxy(360, 240, "SCORE:");
	std::string strScore = std::to_string(score);
	outtextxy(460, 240, strScore.c_str());

	outtextxy(360, 300, "RECORD:");
	std::string strRecord = std::to_string(record);
	outtextxy(470, 300, strRecord.c_str());

	outtextxy(360, 360, "TIME:");
	std::string strTime = std::to_string(gameTime);
	strTime += "s";
	outtextxy(450, 360, strTime.c_str());

	outtextxy(410, 560, "HOLD");
}


void Game::drawBlock() {

	//绘制提示方块
	setfillcolor(Block::COLORS[bag[bagNum]]);
	for (int p = 0;p < 4;p++) {
		for (int q = 0;q < 4;q++) {
			if (Block::SHAPES[bag[bagNum]][0][p][q] == 1) {
				fillrectangle(380 + 30 * q, 90 + 30 * p, 410 + 30 * q, 120 + 30 * p);
			}
		}
	}

	//绘制hold方块
	setfillcolor(Block::COLORS[holdColor]);
	for (int p = 0;p < 4;p++) {
		for (int q = 0;q < 4;q++) {
			if (holdBlock[p][q] == 1) {
				fillrectangle(380 + 30 * q, 430 + 30 * p, 410 + 30 * q, 460 + 30 * p);
			}
		}
	}

	//绘制、清除游戏内方块
	setfillcolor(RGB(128, 128, 128));
	for (int i = 4;i < 24;i++) {
		for (int j = 0;j < 10;j++) {
			if (map[i][j] == true) {
				fillrectangle(40 + 30 * j, 40 + 30 * (i - 4), 70 + 30 * j, 70 + 30 * (i - 4));
			}
			if (map[i][j] == false) {
				clearrectangle(40 + 30 * j, 40 + 30 * (i - 4), 70 + 30 * j, 70 + 30 * (i - 4));
			}
		}
	}

	// 绘制幽灵方块轮廓

	for (int i = 0; i < 4; i++) {
		if (locationG[1][i] < 4) continue;

		int x = locationG[0][i];
		int y = locationG[1][i];

		// 当前格子的像素边界
		int left = 40 + 30 * x;
		int top = 40 + 30 * (y - 4);
		int right = 40 + 30 * x + 29;
		int bottom = 40 + 30 * (y - 4) + 29;

		// 检查四方邻居，无邻居则画边线
		auto isGhost = [&](int nx, int ny) {
			for (int j = 0; j < 4; j++)
				if (locationG[0][j] == nx && locationG[1][j] == ny)
					return true;
			return false;
			};

		if (!isGhost(x - 1, y)) line(left, top, left, bottom);      // 左边线
		if (!isGhost(x + 1, y)) line(right, top, right, bottom);    // 右边线
		if (!isGhost(x, y - 1)) line(left, top, right, top);        // 上边线
		if (!isGhost(x, y + 1)) line(left, bottom, right, bottom);  // 下边线
	}

	//绘制当前方块
	if (bagNum > 0) {
		setfillcolor(Block::COLORS[bag[bagNum - 1]]);
		for (int i = 0;i < 4;i++) {
			if (location[1][i] < 4) {
				continue;
			}
			fillrectangle(40 + 30 * location[0][i], 40 + 30 * (location[1][i] - 4), 70 + 30 * location[0][i], 70 + 30 * (location[1][i] - 4));
		}
	}
	else {
		setfillcolor(Block::COLORS[bagLast]);
		for (int i = 0;i < 4;i++) {
			if (location[1][i] < 4) {
				continue;
			}
			fillrectangle(40 + 30 * location[0][i], 40 + 30 * (location[1][i] - 4), 70 + 30 * location[0][i], 70 + 30 * (location[1][i] - 4));
		}
	}
}


void Game::start() {

	//游戏开始时随机生成bag，并让bagNum从1开始
	for (int i = 0;i < 7;i++) {
		int order = std::rand() % 7;
		int temp = bag[i];
		bag[i] = bag[order];
		bag[order] = temp;
	}
	bagNum = 1;

	this->drawGame();
	this->drawBlock();
	this->creatBlock();
}


void Game::updateTime() {

	int temp = gameTime;

	gameTime = (GetTickCount() - startTick) / 1000;
	gameTick = GetTickCount();

	if (gameTime - temp > 0) {
		clearrectangle(450, 360, 539, 392);
	}
}


void Game::dropBlock() {

	unsigned long gameTick = GetTickCount();

	//难度一
	if (score >= 0 && score < 100) {
		if (gameTick - lastTick >= 1000) {

			//更新坐标
			for (int i = 0;i < 4;i++) {
				location[1][i]++;
			}
			locBlock[1]++;

			lastTick = gameTick;
		}
	}

	//难度二
	else if (score >= 100 && score < 200) {
		if (gameTick - lastTick >= 700) {

			//更新坐标
			for (int i = 0;i < 4;i++) {
				location[1][i]++;
			}
			locBlock[1]++;

			lastTick = gameTick;
		}
	}

	//难度三
	else if (score >= 200 && score < 300) {
		if (gameTick - lastTick >= 500) {

			//更新坐标
			for (int i = 0;i < 4;i++) {
				location[1][i]++;
			}
			locBlock[1]++;

			lastTick = gameTick;
		}
	}

	//难度四
	else if (score >= 300 && score < 400) {
		if (gameTick - lastTick >= 300) {

			//更新坐标
			for (int i = 0;i < 4;i++) {
				location[1][i]++;
			}
			locBlock[1]++;

			lastTick = gameTick;
		}
	}

	//难度五
	else if (score >= 400 && score < 500) {
		if (gameTick - lastTick >= 200) {

			//更新坐标
			for (int i = 0;i < 4;i++) {
				location[1][i]++;
			}
			locBlock[1]++;

			lastTick = gameTick;
		}
	}

	//难度六
	else {
		if (gameTick - lastTick >= 100) {

			//更新坐标
			for (int i = 0;i < 4;i++) {
				location[1][i]++;
			}
			locBlock[1]++;

			lastTick = gameTick;
		}
	}
}


bool Game::checkDrop() {

	//检查是否到底
	for (int i = 0;i < 4;i++) {
		if (location[1][i] == 23) {
			return true;
		}
	}

	//检查下面是否有方块
	for (int i = 0;i < 4;i++) {
		if (map[location[1][i] + 1][location[0][i]] == 1) {
			return true;
		}
	}

	return false;
}


void Game::writeMap() {
	for (int j = 0;j < 4;j++) {
		map[location[1][j]][location[0][j]] = true;
	}
}


void Game::clearLine() {

	int count = 0;   //记录消除的行数

	for (int i = 23;i > 3;i--) {

		while (true) {

			//检查满行
			bool check = true;
			for (int j = 0;j < 10;j++) {
				if (map[i][j] == 0) {
					check = false;
				}
			}

			//未满行则推出循环
			if (check == false) {
				break;
			}

			//满行则消除并移动上面的所有行
			if (check == true) {
				//消除
				for (int j = 0;j < 10;j++) {
					map[i][j] = 0;
				}
				//移动行
				for (int n = i;n > 3;n--) {
					for (int j = 0;j < 10;j++) {
						map[n][j] = map[n - 1][j];
					}
				}

				count++;
			}
		}
	}

	//加分
	score = score + 10 * count;
	clearrectangle(460, 240, 539, 272);
}


bool Game::checkLose() {

	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 10;j++) {
			if (map[i][j] == true) {

				Sleep(200);
				
				//保存游戏记录
				Data* data = new Data;

				data->score = score;
				data->time = gameTime;

				time_t now = time(nullptr);
				struct tm local;
				localtime_s(&local, &now);
				char buf[64];
				strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local);
				data->date = buf;

				//输出游戏记录到存档
				char exePath[200] = { '\0' };
				GetModuleFileName(NULL, exePath, 200);
				std::string dir(exePath);
				size_t pos = dir.find_last_of("\\/");
				dir = dir.substr(0, pos + 1);
				std::string savePath = dir + "save.dat";

				std::fstream file;
				file.open(savePath, std::ios::out | std::ios::app);
				file << data->score << std::endl;
				file << data->time << std::endl;
				file << data->date << std::endl;
				file.close();
				delete data;
				
				//绘制结束界面
				cleardevice();
				settextstyle(50, 0, "Press Start 2P");
				outtextxy(160, 250, "GAME OVER");
				settextstyle(25, 0, "Press Start 2P");
				outtextxy(210, 330, "SCORE:");
				std::string strScore = std::to_string(score);
				outtextxy(300, 330, strScore.c_str());
				settextstyle(25, 0, "Press Start 2P");
				outtextxy(100, 380, "press any key to continue...");

				Sleep(200);
				
				//按任意键继续
				while (true) {
					bool check = false;
					for (int key = 8; key <= 255; key++) {
						if (GetAsyncKeyState(key) & 0x8000) {
							check = true;
							break;
						}
					}
					if (check == true) {
						break;
					}
					Sleep(20);
				}

				Sleep(200);

				return true;
			}
		}
	}

	return false;
}


void Game::creatBag() {

	//袋中方块耗尽后更新顺序
	if (bagNum >= 6) {

		bagLast = bag[bagNum];

		for (int i = 0;i < 7;i++) {
			bag[i] = i;
		}

		for (int i = 0;i < 7;i++) {
			int order = std::rand() % 7;
			int temp = bag[i];
			bag[i] = bag[order];
			bag[order] = temp;
		}
		bagNum = 0;
	}

	//取下一个方块
	else {
		bagNum++;
	}
}


void Game::creatBlock() {

	//创建方块时更新state
	state = 0;

	//清除提示框内方块
	clearrectangle(380, 90, 500, 210);

	//创建袋中的下一个方块
	if (bagNum >= 1) {
		int num = 0;
		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 4;j++) {
				if (Block::SHAPES[bag[bagNum - 1]][state][i][j] == 1) {
					location[0][num] = j + 3;
					location[1][num] = i;
					locBlock[0] = 3;
					locBlock[1] = 0;
					num++;
				}
			}
		}

		type = bag[bagNum - 1];
	}

	//创建上一袋中的最后一个方块
	else if (bagNum == 0) {
		int num = 0;
		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 4;j++) {
				if (Block::SHAPES[bagLast][state][i][j] == 1) {
					location[0][num] = j + 3;
					location[1][num] = i;
					locBlock[0] = 3;
					locBlock[1] = 0;
					num++;
				}
			}
		}

		type = bagLast;
	}
}


void Game::ghostBlock() {

	//初始化坐标
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 4;j++) {
			locationG[i][j] = location[i][j];
		}
	}

	//下落幽灵方块
	while (true) {

		//检查下边是否撞墙
		bool check = 0;
		for (int i = 0;i < 4;i++) {
			if (locationG[1][i] >= 23) {
				check = 1;
				break;
			}
		}
		//检查下边是否有方块
		for (int i = 0;i < 4;i++) {
			if (map[locationG[1][i] + 1][locationG[0][i]] == 1) {
				check = 1;
				break;
			}
		}

		//移动方块
		if (check == 0) {
			for (int i = 0;i < 4;i++) {
				locationG[1][i]++;
			}
		}

		else {
			break;
		}
	}
}