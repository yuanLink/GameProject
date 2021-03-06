#include"Stuff.h"

#include"Stuff.h"

Stuff::Stuff(){
	visible = true;
	No = 0;
	feature = NULL;
	dialog = new string[1];
	dialog[0] = "这个东西有点奇怪";
	dialog_num = 0;
	selected = false;
	stuff = false;
	dialog_image = NULL;
	font_garamond = MakeFont("Garamond", 42);
	dialog_show = false;
	dialogIsShowing = false;
}
Stuff::~Stuff(){
	if (feature){
		feature->Release();
		feature = NULL;
	}

	if (image)image->Release();
}
//No.1 用来确定是否被选择的函数
void Stuff::setVisible(bool Selected){

	//假如这个文件是特殊的物品的话，在这里将其设为不可见
	if (stuff&&Selected)
		visible = false;
}

//No.2 返回值为该物品是否在大地图可见
bool Stuff::isVisible(){
	return visible;
}

//No.3 显示发生调查后的对话框，返回值为false时停止调用
bool Stuff::ShowSelectedText(bool SpaceButton){

	static int index = 0;

	if (index >= dialog_num){
		nextDialog = false;
		dialog_show = false;
		dialogIsShowing = false;
		index = 0;
		return true;
	}
	if (dialog_show){
		//然后绘制对话框
		D3DXVECTOR3 position = { 0, SCREENH - 180, 0 };
		spriteobj->Draw(dialog_image, NULL, NULL, &position, 0xFFFFFFFF);
		//紧接着就绘制对话内容
		PrintFont(font_garamond, 30, SCREENH - 128, dialog[index], D3DCOLOR_XRGB(255, 255, 200));
		if (nextDialog)
			index++;
	}
	//然后立刻把nextdialog这个变量设为false
	nextDialog = false;
	return false;
}

//No.4 用来读取图片
void Stuff::setImage(string filename, D3DCOLOR transcolor){

	//读取所需要的图片
	image = LoadTexture(filename, transcolor);
	if (!image){
		MessageBox(NULL, "Error read picture at setImage!", "ERROR", MB_OK);
		return;
	}
	//读取对话框
	dialog_image = CreateMyDialog("normaldialog.png");
	//读取放大版的图片（？）

}

//No.5 显示物品的特写（待商榷）
void Stuff::ShowSelectedImage(){}

//No.6 表示此物品可以被加入到物品栏
bool Stuff::isStuff(){
	return stuff;
}

//NO.7 发生调查事件
void Stuff::researchHere(SPRITE &Character, int collisionState){

	//发生了碰撞事件
	if (collisionState){
		dialog_show = true;
		Character.velx = Character.vely = 0;
		return;
	}
}