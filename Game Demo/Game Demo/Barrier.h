#ifndef BARRIER_H
#define BARRIER_H

#include"MyDirect.h"

class Barrier :public SPRITE{

public:
	bool visible;					//该物品是否可见
	int No;							//该物品的编号，以便确认
	LPDIRECT3DSURFACE9 feature;		//待定（？）可拾取物品的特殊表示
	string dialog;					//物品的调查时会显示的内容
	//bool dialog_show;				//展示对话框
	//bool dialogIsShowing;			//表示当前对话框正在显示
	bool selected;					//表示物品此时被选中，此时强行终止所有的移动操作（利用某函数实现跨级操作？）
	bool stuff;						//表示此物品是可以被拾取的，此时可以将物品加入物品栏
	LPDIRECT3DTEXTURE9 dialog_image;//对话框
	LPD3DXFONT font_garamond;		//对话框中的文字类型
	//-----------------------------------------
	//----             相关函数        --------
	//-----------------------------------------

	Barrier();
	~Barrier();
	//No.1 用来确定是否被选择的函数
	void setVisible(bool Selected);						

	//No.2 返回值为该物品是否在大地图可见
	bool isVisible();							

	//No.3 显示发生调查后的对话框，返回值为false时停止调用
	bool ShowSelectedText();						

	//No.4 用来读取放大图片
	void setImage(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));

	//No.5 显示物品的特写（待商榷）
	void ShowSelectedImage();						
	
	//No.6 表示此物品可以被加入到物品栏
	bool isStuff();					

	//No.7 发生调查事件(为了以后的功能增加，这里把游戏角色作为参数传入）
	void researchHere(SPRITE &Character,int collisionState);
};

#endif BARRIER_H