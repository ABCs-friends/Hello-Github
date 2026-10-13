#include <windows.h>
#include <easyx.h>
#include "Operate.h"
#include "Block.h"
#include "Game.h"


bool Operate::getKey(){

    //重置键盘状态
    LEFT = 0;
    RIGHT = 0;
    DOWN = 0;
    UP = 0;
    zKey = 0;
    xKey = 0;
    ESC = 0;

    //重置这一帧
    for (int i = 0;i < 7;i++) {
        now[i] = 0;
    }

    //检测键盘状态
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        // 左键按下
        now[0] = 1;
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        // 右键按下
        now[1] = 1;
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        // 下键按下
        now[2] = 1;
    }
    else if (GetAsyncKeyState(VK_UP) & 0x8000) {
        // 上键按下
        now[3] = 1;
    }
    else if (GetAsyncKeyState('Z') & 0x8000) {
        // z 键按下
        now[4] = 1;
    }
    else if (GetAsyncKeyState('X') & 0x8000) {
        // x 键按下
        now[5] = 1;
    }
    else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        // ESC 键按下
        now[6] = 1;
    }

    //对比上一帧是否有更新
    bool check = 0;
    for (int i = 0;i < 7;i++) {
        if (now[i] == last[i]) {
            continue;
        }
        else {
            check = 1;
        }
    }

    //更新键盘状态
    if (check == 1) {
        LEFT = now[0];
        RIGHT = now[1];
        DOWN = now[2];
        UP = now[3];
        zKey = now[4];
        xKey = now[5];
        ESC = now[6];
    }

    //更新上一帧
    for (int i = 0;i < 7;i++) {
        last[i] = now[i];
    }

    //计时器超时last自动归零 (实现左、右、下移和旋转的连续操作)
    if (check == 0) {
        if (gameTick - last_tick > 100) {
            for (int i = 0;i < 4;i++) {
                last[i] = 0;
            }
            last_tick = gameTick;
        }
    }
    else {
        last_tick = gameTick;
    }

    return check;
}


void Operate::moveLeft() {

    if (LEFT == 1) {

        //检查左边是否撞墙
        bool check = 0;
        for (int i = 0;i < 4;i++) {
            if (location[0][i] <= 0) {
                check = 1;
                break;
            }
        }
        //检查左边是否有方块
        for (int i = 0;i < 4;i++) {
            if (map [location[1][i]] [location[0][i] - 1] == 1) {
                check = 1;
                break;
            }
        }

        //移动方块
        if (check == 0) {
            for (int i = 0;i < 4;i++) {
                location[0][i]--;
            }
            locBlock[0]--;
        }
    }
}


void Operate::moveRight() {

    if (RIGHT == 1) {

        //检查右边是否撞墙
        bool check = 0;
        for (int i = 0;i < 4;i++) {
            if (location[0][i] >= 9) {
                check = 1;
                break;
            }
        }
        //检查右边是否有方块
        for (int i = 0;i < 4;i++) {
            if (map [location[1][i]] [location[0][i] + 1] == 1) {
                check = 1;
                break;
            }
        }

        //移动方块
        if (check == 0) {
            for (int i = 0;i < 4;i++) {
                location[0][i]++;
            }
            locBlock[0]++;
        }
    }
}


void Operate::moveDown() {

    if (DOWN == 1) {

        //检查下边是否撞墙
        bool check = 0;
        for (int i = 0;i < 4;i++) {
            if (location[1][i] >= 23) {
                check = 1;
                break;
            }
        }
        //检查下边是否有方块
        for (int i = 0;i < 4;i++) {
            if (map [location[1][i] + 1] [location[0][i]] == 1) {
                check = 1;
                break;
            }
        }

        //移动方块
        if (check == 0) {
            for (int i = 0;i < 4;i++) {
                location[1][i]++;
            }
            locBlock[1]++;
        }
    }
}


bool Operate::launch() {

    if (zKey == 1) {

        while (true) {

            //检查下边是否撞墙
            bool check = 0;
            for (int i = 0;i < 4;i++) {
                if (location[1][i] >= 23) {
                    check = 1;
                    break;
                }
            }
            //检查下边是否有方块
            for (int i = 0;i < 4;i++) {
                if (map[location[1][i] + 1][location[0][i]] == 1) {
                    check = 1;
                    break;
                }
            }

            if (check == 0) {

                //移动方块
                for (int i = 0;i < 4;i++) {
                    location[1][i]++;
                }
                locBlock[1]++;
            }

            else {
                return true;
            }
        }
    }

    else {
        return false;
    }
}


void Operate::hold() {

    if (xKey == 1) {

        //检查是否是初次hold
        bool check = 1;
        for (int i = 0;i < 4;i++) {
            for (int j = 0;j < 4;j++) {
                if (holdBlock[i][j] == 1) {
                    check = 0;
                    break;
                }
            }
            if (check == 0) {
                break;
            }
        }

        //初次hold
        if (check == 1) {

            //记录当前方块的形状和颜色
            for (int i = 0;i < 4;i++) {
                for (int j = 0;j < 4;j++) {
                    holdBlock[i][j] = Block::SHAPES [type] [0] [i] [j];
                    holdType = type;
                }
            }
            holdColor = type;

            //记录当前方块的位置
            int tempLoc[2][4] = { 0 };
            int tempLocBlo[2] = { 0 };
            for (int i = 0;i < 2;i++) {
                for (int j = 0;j < 4;j++) {
                    tempLoc[i][j] = location[i][j];
                }
            }
            tempLocBlo[0] = locBlock[0];
            tempLocBlo[1] = locBlock[1];

            //跳到下一个方块
            this->creatBag();
            this->creatBlock();

            //将下一方块移动到当前位置
            int temp1 = locBlock[1];
            locBlock[0] = tempLocBlo[0];
            locBlock[1] = tempLocBlo[1];
            for (int i = 0;i < 4;i++) {
                location[1][i] += locBlock[1] - temp1;
            }
        }

        //非初次hold
        else {

            //检查能否替换hold方块
            bool _check = true;

            int temp_location[2][4] = { 0 };
            for (int n = 0;n < 4;n++) {
                for (int i = 0;i < 4;i++) {
                    for (int j = 0;j < 4;j++) {
                        if (holdBlock[i][j] == true) {
                            temp_location[0][n] = locBlock[0] + j;
                            temp_location[1][n] = locBlock[1] + i;
                        }
                    }
                }
            }

            //检查是否撞墙
            for (int i = 0;i < 4;i++) {
                if (temp_location[0][i] > 9 || temp_location[0][i] < 0 || temp_location[1][i]>23 || temp_location[1][i] < 0) {
                    _check = false;
                }
            }

            //检查是否撞方块
            for (int i = 0;i < 4;i++) {
                if (map[temp_location[1][i]][temp_location[0][i]] == true) {
                    _check = false;
                }
            }

            if (_check == true) {

                //清除hold框内方块
                clearrectangle(380, 430, 500, 550);

                //将hold方块与当前方块替换
                bool temp[4][4] = { 0 };
                for (int i = 0;i < 4;i++) {
                    for (int j = 0;j < 4;j++) {
                        temp[i][j] = holdBlock[i][j];
                        holdBlock[i][j] = 0;
                    }
                }

                for (int n = 0;n < 4;n++) {
                    int i = location[1][n] - locBlock[1];
                    int j = location[0][n] - locBlock[0];
                    holdBlock[i][j] = 1;
                }

                int n = 0;
                for (int i = 0;i < 4;i++) {
                    for (int j = 0;j < 4;j++) {
                        if (temp[i][j] == 1) {
                            location[1][n] = locBlock[1] + i;
                            location[0][n] = locBlock[0] + j;
                            n++;
                        }
                    }
                }

                //替换颜色和形状
                if (bagNum > 0) {
                    int _temp = bag[bagNum - 1];
                    bag[bagNum - 1] = holdType;
                    type = holdType;
                    holdType = _temp;
                    holdColor = _temp;
                }
                else {
                    int _temp = bagLast;
                    bagLast = holdType;
                    type = holdType;
                    holdType = _temp;
                    holdColor = _temp;
                }
            }
        }
    }
}


void Operate::turn() {
    
    if (UP == 1) {

        bool check = 0;

        //存储旋转后的方块状态
        int temp_state = 0;
        if (state == 3) {
            temp_state = 0;
        }
        else {
            temp_state = state + 1;
        }

        //存储旋转后的方块坐标
        int temp_location[2][4] = { 0 };
        int temp_locBlock[2] = { 0 };

        //通用方块J、L、S、Z、T的踢墙检测
        if (type != 0) {
            for (int n = 0;n < 5;n++) {

                check = 0;

                //移动顶点方块
                temp_locBlock[0] = locBlock[0] + Block::WALLKICK[0][state][n][0];
                temp_locBlock[1] = locBlock[1] + Block::WALLKICK[0][state][n][1];

                //记录方块旋转后的坐标
                int num = 0;
                for (int i = 0;i < 4;i++) {
                    for (int j = 0;j < 4;j++) {
                        if (Block::SHAPES [type] [temp_state] [i] [j] == 1) {
                            temp_location[0][num] = temp_locBlock[0] + j;
                            temp_location[1][num] = temp_locBlock[1] + i;
                            num++;
                        }
                    }
                }

                //检查是否撞墙
                for (int i = 0;i < 4;i++) {
                    if (temp_location[0][i] < 0 || temp_location[0][i] > 9 || temp_location[1][i] > 23 || temp_location[1][i] < 0) {
                        check = 1;
                        break;
                    }
                }
                //检查是否撞方块
                for (int i = 0;i < 4;i++) {
                    if (map [temp_location[1][i]] [temp_location[0][i]] == 1) {
                        check = 1;
                        break;
                    }
                }

                if (check == 0) {
                    break;
                }
            }
        }

        //I的踢墙检测
        else {
            for (int n = 0;n < 5;n++) {

                check = 0;

                //移动顶点方块
                temp_locBlock[0] = locBlock[0] + Block::WALLKICK[1][state][n][0];
                temp_locBlock[1] = locBlock[1] + Block::WALLKICK[1][state][n][1];

                //记录方块旋转后的坐标
                int num = 0;
                for (int i = 0;i < 4;i++) {
                    for (int j = 0;j < 4;j++) {
                        if (Block::SHAPES[type][temp_state][i][j] == 1) {
                            temp_location[0][num] = temp_locBlock[0] + j;
                            temp_location[1][num] = temp_locBlock[1] + i;
                            num++;
                        }
                    }
                }

                //检查是否撞墙
                for (int i = 0;i < 4;i++) {
                    if (temp_location[0][i] < 0 || temp_location[0][i] > 9 || temp_location[1][i] > 23 || temp_location[1][i] < 0) {
                        check = 1;
                        break;
                    }
                }
                //检查是否撞方块
                for (int i = 0;i < 4;i++) {
                    if (map[temp_location[1][i]][temp_location[0][i]] == 1) {
                        check = 1;
                        break;
                    }
                }

                if (check == 0) {
                    break;
                }
            }
        }

        //如果能够旋转
        if (check == 0) {

            //更新state
            state = temp_state;

            //更新顶点、方块坐标
            locBlock[0] = temp_locBlock[0];
            locBlock[1] = temp_locBlock[1];
            for (int i = 0; i < 4; i++) {
                location[0][i] = temp_location[0][i];
                location[1][i] = temp_location[1][i];
            }
        }
    }
}


bool Operate::landLock() {

    unsigned long lastTime = GetTickCount();

    while (true) {

        //读键盘
        this->getKey();

        //执行操作
        this->moveLeft();
        this->moveRight();
        this->moveDown();
        this->hold();
        this->turn();

        //绘制游戏
        this->updateTime();
        this->drawGame();
        this->drawBlock();

        //依然落地则继续循环，直到超过0.5秒
        if (this->checkDrop() == true) {

            unsigned long gameTime = GetTickCount();
            if (gameTime - lastTime > 500) {
                return true;
            }

            Sleep(20);
        }

        //不落地则直接返回false
        else {
            return false;
        }
    }
}


bool Operate::pause() {

    if (ESC == 1) {

        cleardevice();
        int cursor = 0;

        // 设置字体和背景模式
        settextstyle(30, 0, "Press Start 2P");
        setbkmode(TRANSPARENT);

        const char* line1 = "continue";
        const char* line2 = "return top";

        // 获取每行文字的宽高
        int w1 = textwidth(line1);
        int h1 = textheight(line1);
        int w2 = textwidth(line2);
        int h2 = textheight(line2);

        // 行间距
        int lineSpacing = 10;
        int totalHeight = h1 + lineSpacing + h2;

        // 计算起始 y 坐标（整体垂直居中）
        int startY = (680 - totalHeight) / 2;

        // 绘制第一行（水平居中）
        int x1 = (550 - w1) / 2;
        outtextxy(x1, startY, line1);

        // 绘制第二行（水平居中）
        int x2 = (550 - w2) / 2;
        outtextxy(x2, startY + h1 + lineSpacing, line2);

        while (true) {

            this->getKey();

            if (UP == 1) {
                if (cursor == 0) {
                    cursor = 1;
                }
                else {
                    cursor--;
                }
            }
            else if (DOWN == 1) {
                if (cursor == 1) {
                    cursor = 0;
                }
                else {
                    cursor++;
                }
            }
            else if (zKey == 1) {
                zKey = 0;
                cleardevice();
                if (cursor == 0) {
                    return false;
                }
                else if (cursor == 1) {
                    return true;
                }
            }
            else if (xKey == 1) {
                xKey = 0;
                cleardevice();
                return false;
            }

            //清除光标
            if (UP == 1 || DOWN == 1) {
                settextstyle(30, 0, "Press Start 2P");

                const char* lines[] = { "continue", "return top" };
                int h1 = textheight(lines[0]);
                int h2 = textheight(lines[1]);
                int lineSpacing = 10;
                int totalHeight = h1 + lineSpacing + h2;
                int startY = (680 - totalHeight) / 2;
                int ys[2] = { startY, startY + h1 + lineSpacing };

                setfillcolor(BLACK);
                setlinecolor(BLACK);

                for (int i = 0; i < 2; i++) {
                    int w = textwidth(lines[i]);
                    int h = textheight(lines[i]);
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
                settextstyle(30, 0, "Press Start 2P");

                const char* line1 = "continue";
                const char* line2 = "return top";
                int w1 = textwidth(line1);
                int h1 = textheight(line1);
                int w2 = textwidth(line2);
                int h2 = textheight(line2);
                int lineSpacing = 10;
                int totalHeight = h1 + lineSpacing + h2;
                int startY = (680 - totalHeight) / 2;

                int x1 = (550 - w1) / 2;
                int y1 = startY;
                int tipX = x1 - 10;
                int tipY = y1 + h1 / 2;

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
                settextstyle(30, 0, "Press Start 2P");

                const char* line1 = "continue";
                const char* line2 = "return top";
                int w1 = textwidth(line1);
                int h1 = textheight(line1);
                int w2 = textwidth(line2);
                int h2 = textheight(line2);
                int lineSpacing = 10;
                int totalHeight = h1 + lineSpacing + h2;
                int startY = (680 - totalHeight) / 2;

                int x2 = (550 - w2) / 2;
                int y2 = startY + h1 + lineSpacing;
                int tipX = x2 - 10;
                int tipY = y2 + h2 / 2;

                setfillcolor(WHITE);
                setlinecolor(WHITE);
                POINT tri[3] = {
                    {tipX, tipY},
                    {tipX - 12, tipY - 8},
                    {tipX - 12, tipY + 8}
                };
                solidpolygon(tri, 3);
            }

            Sleep(20);
        }
    }
}