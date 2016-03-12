/*
	时间：2016.2.27
	作者：Link
	简单游戏引擎的基本头文件
*/
#pragma once
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
//在这里定义一些基本的宏
#define LEFT_COLLISION 101
#define UP_COLLISION 102
#define RIGHT_COLLISION 103
#define BOTTOM_COLLISION 104
//鼠标的一些状态
#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_ROLL 2
//各种头文件
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<ctime>
using namespace std;
//------------------------------------------------------
//基本的要包含的库文件
//最重要的direct3d的总句柄
#pragma comment(lib,"d3d9.lib")
//很多基本的函数，比如massagebox（消息盒子）要从这里调用
#pragma comment(lib,"user32.lib")
//direct3d不能处理的，绘制图片的库(比如精灵啊纹理啊）
#pragma comment(lib,"d3dx9.lib")
//最最基本的库，和windows相关的编程都要包含这个
#pragma comment(lib,"winmm.lib")
//对于使用direct来说最基本的库
#pragma comment(lib,"dxguid.lib")
//使用directinput，也就是操控器
#pragma comment(lib,"dinput8.lib")
//绘图的关键库，这里面有很多Create函数，包括文字部分
#pragma comment(lib,"gdi32.lib")

//------------------------------------------------------
//然后是基本的变量定义
//首先定义一些到处都用得到的变量————屏幕的大小
const int SCREENH = 768;
const int SCREENW = 1024;
const string APPTITLE = "Game Demo";
//这一段是direct3d使用的基本句柄，只有利用下面的两个变量才能对direct进行操作
//这个device就是具体的操作函数,注意由于这两个变量在其他的文件中也要出现所以要声明为extern
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern bool gameover;//决定游戏的循环是否会进行
//然后是相关的函数
//初始化Direct,可以决定屏幕的大小
bool Direct_Init(HWND hwnd, int width, int height);
//销毁Direct
void Direct_ShutDown();

//这一段是directinput的相关定义，只有利用下面的几个变量才能对directinput操作
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern DIMOUSESTATE2 mouse_state;
//然后是相关的初始化函数，还有一些辅助功能比如“键盘的按钮”，“ 鼠标的位置”
bool DirectInput_Init(HWND hwnd);
//更新键盘操作
void DirectInput_Update();
void DirectInput_ShutDown();
//观察鼠标是否按下
int Mouse_Button(int);
//返回鼠标的位置
int Mouse_X();
int Mouse_Y();
int Mouse_PosX(HWND);
int Mouse_PosY(HWND);
//返回键盘对应的位置是否被按下
bool Key_Down(int);
//加强版的按钮：判断对应位置是否在被按下后弹起来
//bool Key_Up(int);

//接下来是关键的图片相关绘制,这两个也会在不同的文件使用（在Game循环中要反复使用），所以也声明为extern
//以下的两个变量是可以在DirectInit中初始化的
extern LPDIRECT3DSURFACE9 backbuffer;			//最关键的部分，这是一个“缓冲屏幕”，所有的操作都要优先提交到这里
extern LPD3DXSPRITE spriteobj;					//操控精灵的关键类

//这是相关的函数————关键的图片/精灵读取与处理
//读取普通的图片（作为背景)
LPDIRECT3DSURFACE9 LoadSurface(string filename);
//读取作为精灵对象的图片（纹理）
LPDIRECT3DTEXTURE9 LoadTexture(string filename ,D3DCOLOR transcolor = D3DCOLOR_XRGB(255,255,255));
//将资源图片（文件中读取的）绘制到目标图片对象（处理后的）
void DrawSurface(LPDIRECT3DSURFACE9 surface, float x, float y, LPDIRECT3DSURFACE9 &dest,int width = 0,int height = 0,int dest_x = 0,int dest_y = 0);
//将精灵文件(可以运动的或者是单帧的纹理）绘制到屏幕上
void Draw_Sprite_Texture(LPDIRECT3DTEXTURE9 teture,
	float x, float y,											//将要绘制的坐标
	int frame = 0,											//绘制的该帧，默认为0
	int framew = 64,										//该帧每一幅的宽度
	int frameh = 64,										//该帧每一幅的高度
	int columns	= 1,										//这个精灵有多少列
	D3DCOLOR tanscolor = D3DCOLOR_XRGB(255, 255, 255));		//透明的颜色
//按照一定的时间顺序播放精灵的动画
void Sprite_Animate(int &frame, 
	int &starttime,								//动画的开始时间
	int columns,								//精灵有多少列
	int startframe,								//开始的帧号
	int endframe,								//结束的帧号
	int direction,								//每次跨越多少帧
	int delay);									//延迟就是说过多久进入下一帧

void Transform_Sprite(LPDIRECT3DTEXTURE9 teture,
	float x, float y,										//将要绘制的坐标
	int frame = 0,											//绘制的该帧，默认为0
	int framew = 64,										//该帧每一幅的宽度
	int frameh = 64,										//该帧每一幅的高度
	int columns = 1,										//这个精灵有多少列
	float rotation = 0.0f,									//旋转的角度
	float scaling = 1.0f,									//缩放的程度
	D3DCOLOR tanscolor = D3DCOLOR_XRGB(255,255,255));		//透明的颜色


//===================================================================
//一下是保留部分，即是说可以更改的函数和结构体/类
//由于我的想法是为了增加调用过程中的简便性，我使用了结构体，因为不太需要精灵本身的函数，也不太需要精灵能够继承什么的
//然而我觉得的确是需要用类作为调用的方法的，因为我们还有按钮对象，这个按钮是很有必要作为精灵存在的，因为我需要这个
//按钮能够感觉到鼠标的点击之类的
//所以现在考虑与结构体相同的类的名字来命名
/*
typedef struct SPRITE{
	float x, y;//坐标
	int width, height;//大小
	int framenum;//精灵目前所在的帧的编号
	int startframe, endframe;//这个精灵的开始和结束帧
	int direction, delay;//精灵每次前进的帧的数量和延迟
	int columns;//总共有多少行
	float scaling, rotation;//缩放比例和旋转的情况
	LPDIRECT3DTEXTURE9 image;//读入的图片
	D3DCOLOR color;//设为背景的透明颜色
	float velx, vely;//在横方向上的速度和纵方向上的速度
	SPRITE(){
		x = y = 0;
		width = height = 64;
		framenum = 0;
		startframe = endframe = 0;
		direction = 1;
		delay = 30;
		columns = 0;
		scaling = 1.0f;
		rotation = 0.0f;
		image = NULL;
		color = D3DCOLOR_XRGB(255, 255, 255);
		velx = vely = 0;
	}
}SPRITE;
*/
class SPRITE {
public:
	float x, y;//坐标
	int width, height;//大小
	int framenum;//精灵目前所在的帧的编号
	int startframe, endframe;//这个精灵的开始和结束帧
	int direction, delay;//精灵每次前进的帧的数量和延迟
	int columns;//总共有多少行
	float scaling, rotation;//缩放比例和旋转的情况
	LPDIRECT3DTEXTURE9 image;//读入的图片
	D3DCOLOR color;//设为背景的透明颜色
	float velx, vely;//在横方向上的速度和纵方向上的速度

	bool dialog_show;				//展示对话框
	bool dialogIsShowing;			//表示当前对话框正在显示
	int dialog_num;				//目前为止在将第几段话
	SPRITE(){
		x = y = 0;
		width = height = 64;
		framenum = 0;
		startframe = endframe = 0;
		direction = 1;
		delay = 30;
		columns = 1;
		scaling = 1.0f;
		rotation = 0.0f;
		image = NULL;
		color = D3DCOLOR_XRGB(255, 255, 255);
		velx = vely = 0;

		dialog_show = false;
		dialogIsShowing = false;
		dialog_num = 1;
	}
	~SPRITE(){}
	//新增加一个函数，表示“选取的位置”
	bool In(int pos_x, int pos_y){
		return (pos_x > x&&pos_x< x + width &&pos_y>y&&pos_y < y + height);
	}
};
//碰撞测试，这里用int作为返回来确定发生碰撞的位置
int CollisionRect(SPRITE sprite1,SPRITE block);				//这两个精灵类我们在后面定义
//碰撞测试，这里用int作为返回来确定发生碰撞的位置
int CollisionD(SPRITE sprite1,SPRITE block);

//游戏的初始化
bool Game_Init(HWND hwnd);
//游戏运行(MSG 为黑科技）
void Game_Run(HWND window,MSG messsage);
//游戏结束
void Game_End();

//游戏背景画面的创立(由于这个背景变量作为了全局变量，所以无需传入参数）
bool CreateBackgroud(string filename, int width, int height);

//---------------------
//对话框功能
//---------------------
//LPDIRECT3DSURFACE9 normal_dialog;
//对话框的建立（感觉好多地方都要用到。。。发现不小心和FirstStage的对话框割裂了…………如何是好…………）
//这个的功能只是创建那个图标而已啦……具体内容的绘制还是要在各个地方完成的
LPDIRECT3DTEXTURE9 CreateMyDialog(string filaname);
//绘制对话框
bool DrawMyDialog(LPDIRECT3DTEXTURE9 dialog, float x, float y);

//---------------------
//创建文字的函数
//---------------------
LPD3DXFONT MakeFont(string fontname,int size);
//将汉字绘制到屏幕上
void PrintFont(LPD3DXFONT font, float x, float y, string text, 
	D3DCOLOR color = D3DCOLOR_XRGB(255,255,255),bool setFixed = false);

//新学会的技巧：找到帧率
extern DWORD FrameCnt;				 // 发生了（渲染了的）帧数
extern float TimeElapsed;			// 已经经过的时间
extern float FPS;					// （平均）帧率
void CalcFPS(HWND,float timeDelta);//计算发生的帧数，注意这里的timeDelta为两帧之间的时间差