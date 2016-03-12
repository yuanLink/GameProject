#include"Button.h"

Button::Button(){
	text = "";
	rect = { x, y,x+width, y+width };
	state = BUTTON_NONE;
	//***����Ӧ�û�Ҫ��ͼƬ����ȡ�ˣ����������ʱ��д�ˡ���������Ϊû�а�ťͼƬ
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
		//��ʱ����λ�ڰ�ť��������
		//���һ����ʱ��ť�Ѿ�����ȥ��
		if (clicked){
			if (state = BUTTON_OVER)
				state = BUTTON_CLICKED;
		}
		//���������ʱ��û�а���ȥ
		else
			state = BUTTON_OVER;
	}
	else{
		//�����������ڵĻ��������Ǹոհ��꣬Ҳ�����Ǹ���û�а�
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