/*
	Date:2016.03.02
	Link
	这个定义为第一关，目标为“从一个大的滚轴屏幕上收集通关的元素，在合适的地方使用后完成关卡”
*/

#ifndef FIRSTSTAGE_H
#define FIRSTSTAGE_H
#define SPEED 0.4
#define BUFFERW SCREENW*2
/*
	Author：Link
	Date:2016.03.08
	第一关：绿色精灵的诞生
	通过拾取道具和
*/

#define BUFFERH SCREENH*2

#include"Stage.h"

class FirstStage :public Stage{

	//LPDIRECT3DSURFACE9 background;//背景
	LPDIRECT3DTEXTURE9 staff;				//道具
	D3DXVECTOR2 menu_pos;					//菜单栏的位置
	SPRITE stone;							//石头
	float stage_valx, stage_valy;			//地图移动的速度
	int backgroundWidth, backgroundHeight;	//地图的长度和宽度
	int numOfBarrier;						//	物品的数量
	int numOfTool;							//工具的数量
	bool nextDialog;						//在对话功能上的“空格是否按下的判断”
	bool DialogEvent;				//对话发生事件，可能是解决方案
	bool canMove;
	/*
	对于对话框的处理：
	首先的首先，如果此时已经发生了调查事件的话（返回值为true的时候，我们应该去检测是否按下了空格键来使对话
	继续发生，所以这里的返回值canMove应该有两层含义（所以我们在这里还要注意，canMove为false的时候，不但不能
	行动，而且不用再次进行调查以减少游戏负荷（？），并且同时只进行外部的对话框事件，当对话框结束的时候，将camMove
	调整为true，使游戏继续进行
	*/
public:
	FirstStage();
	~FirstStage();

	//然后第一关要重载的函数
	//No.1  该关卡的初始化
	bool Stage_Init(HWND hwnd);

	//No.2  该关卡的运行画面
	void Stage_Run();

	//No。2.5 该关卡的绘制
	void Stage_Draw();

	//No.3  该关卡的结束
	bool Stage_End();

	//No 3.5  绘制结束画面

	bool Stage_End_Draw();

	//No.4  绘制对话框
	void Dialog_Draw();

	//No.5  创建基本菜单(好像不粗要重载把……）
	//bool CreateMenu();

	//No.8 调查功能：传入角色和物品，在阻止角色前进的同时，对其发生调查事件，假如传入的参数即将发生调查的话，我们就
	//阻止角色的行动，并且开始显示对话框（假如这个调查事件正在发生，则不再进行检测是否发生了调查,而是只去检测是否按下
	//空格键
	bool reserchHere(SPRITE &Character, SPRITE &object,bool canMove);
};

#endif FIRSTSTAGE_H