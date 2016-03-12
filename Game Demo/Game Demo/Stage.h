/*
	Date:2016.03.03
	Link
	好吧时间有点仓促……就先暂时用这个作为我的正式游戏吧
*/
#ifndef STAGE_H
#define STAGE_h

#include"MyDirect.h"
#include"Button.h"
#include"Barrier.h"
#include"Stuff.h"

enum STAGE_STATE{
	STAGE_INIT = 0,
	STAGE_LOAD = 1,
	STAGE_RUNING = 2,
	STAGE_OVER = 3
};

class Stage{
protected:
	int score;//本关分数
	LPDIRECT3DSURFACE9 background;//背景
	SPRITE Character;//游戏角色
	int starttime, startframe, endframe,direction,delay;//游戏角色的必须的可动的变量（是不是考虑要加一个类？）
	Barrier *barrier;//该关卡上的其他元素（比如什么障碍物啊之类的）
	Stuff *tool;//游戏中可以拾取的道具（或者不出现在大地图上的）

	string *dialog;//对话框中的内容
	int string_index;//该关卡上的第几段对话

	LPD3DXFONT font_arial;//字体的类型
	bool spaceButton;//表示这个space已经按下了

	Button *button;//在该关卡上拥有的按钮

	LPDIRECT3DSURFACE9 menu;//菜单
	STAGE_STATE  game_state;//游戏的状态
	float X, Y;//该场景的相对坐标
	//music 该关卡中的音乐

public:
	//Stage();
	//~Stage();
	//接下来是比较重要的几个函数
	//No.1  该关卡的初始化
	virtual bool Stage_Init(HWND hwnd) = 0;

	//No.2  该关卡的运行时的更新
	virtual void Stage_Run() = 0;

	//No.2.5 该关卡的图片更新
	virtual void Stage_Draw() = 0;

	//No.3  该关卡的结束(返回值确定此时是否完全的退出了游戏）
	virtual bool Stage_End() = 0;

	//No.4  绘制对话框
	virtual void Dialog_Draw() = 0;

	//No.5  创建基本菜单
	bool CreateMenu(){
		menu = LoadSurface("Menu.png");
		if (!menu){
			MessageBox(NULL, "Error Read menu,png", "Error", MB_OK);
			return false;
		}
	}
	//No.6  绘制基本菜单
	void DrawMenu(){
		DrawSurface(menu, 20, 20, backbuffer);
	}

	//No.7 返回当前的关卡状体
	STAGE_STATE GetGameState(){
		return game_state;
	}
	/*
	//No.8 调查功能：传入角色和物品，在阻止角色前进的同时，对其发生调查事件，假如传入的参数即将发生调查的话，我们就
	//阻止角色的行动，并且开始显示对话框（假如这个调查事件正在发生，则不再进行检测是否发生了调查,而是只去检测是否按下
	//空格键
	bool reserchHere(SPRITE &Character, SPRITE object){

		/*
		首先的首先，如果此时已经发生了调查事件的话（返回值为true的时候，我们应该去检测是否按下了空格键来使对话
		继续发生，所以这里的返回值canMove应该有两层含义（所以我们在这里还要注意，canMove为false的时候，不但不能
		行动，而且不用再次进行调查以减少游戏负荷（？），并且同时只进行外部的对话框事件，当对话框结束的时候，将camMove
		调整为true，使游戏继续进行
		
		//碰撞测试，得到角色与哪个物品发生了碰撞
		int temp_collision;
		bool canMove = true;
		temp_collision = CollisionD(Character, object);
		if (temp_collision == LEFT_COLLISION)
			Character.velx = SPEED;
		else if (temp_collision == RIGHT_COLLISION)
			Character.velx = -SPEED;
		else if (temp_collision == UP_COLLISION)
			Character.vely = -SPEED;
		else if (temp_collision == BOTTOM_COLLISION)
			Character.vely = SPEED;

		//然后，判断是否会进行调查(注意，如果此时已经显示了对话框，则不能再次显示）
		float temp_destx = abs(Character.x + Character.width / 2 - (object.x + object.width / 2));
		float temp_desty = abs(Character.y + Character.height / 2 - (object.y + object.height / 2));

		//判断是否已经可以调查
		if (temp_destx < Character.width / 2 + object.width / 2 + 30 &&
			temp_desty < Character.height / 2 + object.height / 2 + 30){
			if (Key_Down(DIK_SPACE) && !object.dialog_show &&!object.dialogIsShowing){
				if (!spaceButton){
					object.dialog_show = true;
					spaceButton = true;
					canMove = false;
				}
			}
			//但是，如果对话框已经绘制出来，并且还是按下了按钮的话，就不在绘制
			else if (object.dialog_show&&Key_Down(DIK_SPACE) && object.dialogIsShowing){
				if (!spaceButton){
					object.dialog_show = false;
					spaceButton = true;
					object.dialogIsShowing = false;
					canMove = true;
				}
			}
			else
				spaceButton = false;
		}

		return canMove;
	}
	*/
};

#endif STAGE_H