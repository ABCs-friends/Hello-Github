#include <easyx.h>
#include <fstream>
#include <string>
#include "Menu.h"
#include "Game.h"


void Menu::drawMenu() {

    cleardevice();
    setbkcolor(BLACK);
    cleardevice();

    settextstyle(50, 0, "Press Start 2P");
    setbkmode(TRANSPARENT);

    COLORREF colors[6] = {
        RGB(0,   0,   240),   // J
        RGB(240, 160, 0),   // L
        RGB(240, 240, 0),   // O
        RGB(0,   240, 0),   // S
        RGB(160, 0,   240),   // T
        RGB(240, 0,   0)    // Z
    };

    const char* title = "TETRIS";
    int titleY = 180;
    int titleSpacing = 45;
    int totalWidth = 6 * titleSpacing;
    int startX = (550 - totalWidth) / 2 + 10;

    for (int i = 0; i < 6; i++) {
        settextcolor(colors[i]);
        char ch[2] = { title[i], '\0' };
        outtextxy(startX + i * titleSpacing, titleY, ch);
    }

    settextstyle(24, 0, "Press Start 2P");
    int optionY = titleY + 100;
    int optionSpacing = 60;

    const char* options[] = { "Start", "History", "Exit" };
    for (int i = 0; i < 3; i++) {
        int textW = textwidth(options[i]);
        int x = (550 - textW) / 2;
        settextcolor(WHITE);
        outtextxy(x, optionY + i * optionSpacing, options[i]);
    }
}


void Menu::drawCursor() {
    
    //更新光标状态
    if (UP == 1) {
        if (cursor == 0) {
            cursor = 2;
        }
        else {
            cursor--;
        }
    }
    if (DOWN == 1) {
        if (cursor == 2) {
            cursor = 0;
        }
        else {
            cursor++;
        }
    }

    //清除光标
    if (UP == 1 || DOWN == 1) {
        const char* items[] = { "Start", "History", "Exit" };
        int ys[] = { 280, 340, 400 };
        setfillcolor(BLACK);
        setlinecolor(BLACK);
        for (int i = 0; i < 3; i++) {
            int w = textwidth(items[i]);
            int h = textheight(items[i]);
            int x = (550 - w) / 2;
            int y = ys[i];
            int tipX = x - 10;
            int tipY = y + h / 2;
            POINT tri[3] = {
                {tipX, tipY},
                {tipX - 12, tipY - 8},
                {tipX - 12, tipY + 8}
            };
            solidpolygon(tri, 3);
        }
    }

    //绘制光标
    if (cursor == 0) {
        int w = textwidth("Start");
        int h = textheight("Start");
        int x = (550 - w) / 2;
        int y = 280;
        int tipX = x - 10;
        int tipY = y + h / 2;
        setfillcolor(WHITE);
        setlinecolor(WHITE);
        POINT tri[3] = {
            {tipX, tipY},
            {tipX - 12, tipY - 8},
            {tipX - 12, tipY + 8}
        };
        solidpolygon(tri, 3);
    }
    else if (cursor == 1) {
        int w = textwidth("History");
        int h = textheight("History");
        int x = (550 - w) / 2;
        int y = 340;
        int tipX = x - 10;
        int tipY = y + h / 2;
        setfillcolor(WHITE);
        setlinecolor(WHITE);
        POINT tri[3] = {
            {tipX, tipY},
            {tipX - 12, tipY - 8},
            {tipX - 12, tipY + 8}
        };
        solidpolygon(tri, 3);
    }
    else if (cursor == 2) {
        int w = textwidth("Exit");
        int h = textheight("Exit");
        int x = (550 - w) / 2;
        int y = 400;
        int tipX = x - 10;
        int tipY = y + h / 2;
        setfillcolor(WHITE);
        setlinecolor(WHITE);
        POINT tri[3] = {
            {tipX, tipY},
            {tipX - 12, tipY - 8},
            {tipX - 12, tipY + 8}
        };
        solidpolygon(tri, 3);
    }
}


bool Menu::getKey() {

    //重置键盘状态
    DOWN = 0;
    UP = 0;
    zKey = 0;
    xKey = 0;

    //重置这一帧
    for (int i = 0;i < 4;i++) {
        now[i] = 0;
    }

    //检测键盘状态
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        // 下键按下
        now[0] = 1;
    }
    else if (GetAsyncKeyState(VK_UP) & 0x8000) {
        // 上键按下
        now[1] = 1;
    }
    else if (GetAsyncKeyState('Z') & 0x8000) {
        // z 键按下
        now[2] = 1;
    }
    else if (GetAsyncKeyState('X') & 0x8000) {
        // x 键按下
        now[3] = 1;
    }

    //对比上一帧是否有更新
    bool check = 0;
    for (int i = 0;i < 4;i++) {
        if (now[i] == last[i]) {
            continue;
        }
        else {
            check = 1;
        }
    }

    //更新键盘状态
    if (check == 1) {
        DOWN = now[0];
        UP = now[1];
        zKey = now[2];
        xKey = now[3];
    }

    //更新上一帧
    for (int i = 0;i < 4;i++) {
        last[i] = now[i];
    }

    return check;
}


int Menu::confirm() {

    if (zKey == 1) {

        if (cursor == 0) {
            return 0;
        }
        else if (cursor == 2) {
            return 2;
        }
        else if (cursor == 1) {
            return 1;
        }
    }

    else {
        return -1;
    }
}


bool Menu::back() {

    if (xKey == 1) {
        return true;
    }
    else {
        return false;
    }
}


void Menu::drawHistory() {

    //标题
    settextstyle(36, 0, "Press Start 2P");
    setbkmode(TRANSPARENT);
    const char* title = "HISTORY";
    int titleW = textwidth(title);
    outtextxy((550 - titleW) / 2, 30, title);

    //大边框
    rectangle(30, 70, 520, 640);

    //表格参数
    const int X0 = 50;
    const int Y0 = 100;
    const int RowH = 30;
    const int ColW[3] = { 100, 100, 240 };
    int TotalRows = 16;

    // 计算每列的起始 X 坐标（累加）
    int ColX[3];
    ColX[0] = X0;
    for (int i = 1; i < 3; i++) {
        ColX[i] = ColX[i - 1] + ColW[i - 1];
    }

    settextstyle(18, 0, "Press Start 2P");
    setbkmode(TRANSPARENT);

    //表头（白色）
    const char* headers[3] = { "Score", "Time", "Date" };
    for (int c = 0; c < 3; c++) {
        rectangle(ColX[c], Y0, ColX[c] + ColW[c], Y0 + RowH);
        outtextxy(ColX[c] + 4, Y0 + 5, headers[c]);
    }

    //15 行空数据格子
    settextcolor(WHITE);
    for (int r = 1; r < TotalRows; r++) {
        int y = Y0 + r * RowH;
        for (int c = 0; c < 3; c++) {
            rectangle(ColX[c], y, ColX[c] + ColW[c], y + RowH);
        }
    }
}


void Menu::drawData() {

    //读取文件并创建链表
    char exePath[200] = { '\0' };
    GetModuleFileName(NULL, exePath, 200);
    std::string dir(exePath);
    size_t pos = dir.find_last_of("\\/");
    dir = dir.substr(0, pos + 1);
    std::string savePath = dir + "save.dat";
    std::fstream file;
    file.open(savePath, std::ios::in);

    if (!file.fail()) {

        Data* head = NULL;
        Data* tail = NULL;
        std::string line;

        while (true) {

            if (file.eof()) {
                break;
            }

            if (!std::getline(file, line)) break;
            int score = std::stoi(line);
            if (!std::getline(file, line)) break;
            int time = std::stoi(line);
            if (!std::getline(file, line)) break;
            std::string date = line;

            Data* newnode = new Data;
            newnode->score = score;
            newnode->time = time;
            newnode->date = date;
            if (head == NULL) {
                head = newnode;
                tail = newnode;
            }
            else {
                tail->next = newnode;
                tail = newnode;
            }
        }

        //将链表按分数排序
        if (head != NULL) {
            bool swapped;
            Data* ptr;
            Data* last = NULL;
            do {
                swapped = false;
                ptr = head;
                while (ptr->next != last) {
                    if (ptr->score < ptr->next->score) {
                        int tmpScore = ptr->score;
                        ptr->score = ptr->next->score;
                        ptr->next->score = tmpScore;

                        int tmpTime = ptr->time;
                        ptr->time = ptr->next->time;
                        ptr->next->time = tmpTime;

                        std::string tmpDate = ptr->date;
                        ptr->date = ptr->next->date;
                        ptr->next->date = tmpDate;

                        swapped = true;
                    }
                    ptr = ptr->next;
                }
                last = ptr;
            } while (swapped);
        }

        //绘制前十五个数据
        Data* ptr = NULL;
        ptr = head;
        settextstyle(18, 0, "Press Start 2P");
        for (int i = 1;i <= 15;i++) {

            if (ptr == NULL) {
                break;
            }

            std::string strScore = std::to_string(ptr->score);
            outtextxy(54, 105 + i * 30, strScore.c_str());
            std::string strTime = std::to_string(ptr->time);
            strTime += "s";
            outtextxy(154, 105 + i * 30, strTime.c_str());
            outtextxy(254, 105 + i * 30, ptr->date.c_str());

            ptr = ptr->next;
        }

        //释放链表
        ptr = head;
        while (ptr != NULL) {
            Data* temp = ptr->next;
            delete ptr;
            ptr = temp;
        }
    }
}