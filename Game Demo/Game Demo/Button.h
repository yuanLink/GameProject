/*
	Date:2016.03.01
	Link
	用来创建Button对象
*/
#include"MyDirect.h"

#ifndef BUTTON_H
#define BUTTON_H

enum BUTTON_STATE{
	BUTTON_NONE = 0,		//按钮完全没被理会……
	BUTTON_OVER = 1,		//在按钮的上方
	BUTTON_CLICKED = 2,		//按钮被按下
	BUTTON_RELEASED = 3		//按钮被松开
};
class Button :public SPRITE{
	//增加的为一个叫做“状态的变量”
public:
	BUTTON_STATE state;	//按钮的当前状态
	RECT rect;			//按钮的区域
	string text;		//按钮的文字
	//-----------相关函数--------------
	Button();
	~Button();
	//-----------计划中的函数-----------
	//No.1 判断是否被点击了(若当前情况为0，则当x,y在区域内的时候…………比例巴拉）
	BUTTON_STATE onClicked(int x, int y,bool clicked);
	//No.2 在当前状态下应该绘制什么画面
	bool DrawButton();
};

#endif BUTTON_H