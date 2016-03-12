#include"Button.h"

Button::Button(){
	text = "";
	rect = { x, y,x+width, y+width };
	state = BUTTON_NONE;
	//***这里应该还要把图片给读取了，我这里就暂时不写了…………因为没有按钮图片
	image = LoadTexture("NormalButton.png", D3DCOLOR_ARGB(255, 0, 0, 0));
	framenum = 0;
	width = 210;
	height = 70;
	columns = 2;
	color = D3DCOLOR_ARGB(255, 0, 0, 0);
}

Button::~Button(){
	if (image)
	image->Release();
	image = NULL;
}
//------------------------------------------------------
BUTTON_STATE Button::onClicked(int x, int y,bool clicked){
	if (x<rect.right && x>rect.left && y<rect.bottom && y> rect.top){
		//此时坐标位于按钮的区域内
		//情况一：此时按钮已经按下去了
		if (clicked){
			if (state = BUTTON_OVER)
				state = BUTTON_CLICKED;
		}
		//情况二：此时还没有按下去
		else
			state = BUTTON_OVER;
	}
	else{
		//若不在区域内的话，可能是刚刚按完，也可能是更本没有按
		if (state == BUTTON_CLICKED)
			state = BUTTON_RELEASED;
		else
			state = BUTTON_NONE;
	}
	return state;
}
//------------------------------------------------------
bool Button::DrawButton(){
	switch (state){
	case(BUTTON_NONE) : {
		Draw_Sprite_Texture(image, x, y, 0, width, height,columns);
		break;
	}
	case(BUTTON_CLICKED) : {
		Draw_Sprite_Texture(image, x, y, 2, width, height,columns);
		break;
	}
	case(BUTTON_RELEASED) : {
		Draw_Sprite_Texture(image, x, y, 3, width, height,columns);
		break;
	}
	case(BUTTON_OVER) : {
		Draw_Sprite_Texture(image, x, y, 1, width, height, columns);
		break;
	}
	}
	return true;
}