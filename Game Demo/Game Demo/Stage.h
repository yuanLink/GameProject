/*
	Date:2016.03.03
	Link
	�ð�ʱ���е�ִ١���������ʱ�������Ϊ�ҵ���ʽ��Ϸ��
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
	int score;//���ط���
	LPDIRECT3DSURFACE9 background;//����
	SPRITE Character;//��Ϸ��ɫ
	int starttime, startframe, endframe,direction,delay;//��Ϸ��ɫ�ı���Ŀɶ��ı������ǲ��ǿ���Ҫ��һ���ࣿ��
	Barrier *barrier;//�ùؿ��ϵ�����Ԫ�أ�����ʲô�ϰ��ﰡ֮��ģ�
	Stuff *tool;//��Ϸ�п���ʰȡ�ĵ��ߣ����߲������ڴ��ͼ�ϵģ�

	string *dialog;//�Ի����е�����
	int string_index;//�ùؿ��ϵĵڼ��ζԻ�

	LPD3DXFONT font_arial;//���������
	bool spaceButton;//��ʾ���space�Ѿ�������

	Button *button;//�ڸùؿ���ӵ�еİ�ť

	LPDIRECT3DSURFACE9 menu;//�˵�
	STAGE_STATE  game_state;//��Ϸ��״̬
	float X, Y;//�ó������������
	//music �ùؿ��е�����

public:
	//Stage();
	//~Stage();
	//�������ǱȽ���Ҫ�ļ�������
	//No.1  �ùؿ��ĳ�ʼ��
	virtual bool Stage_Init(HWND hwnd) = 0;

	//No.2  �ùؿ�������ʱ�ĸ���
	virtual void Stage_Run() = 0;

	//No.2.5 �ùؿ���ͼƬ����
	virtual void Stage_Draw() = 0;

	//No.3  �ùؿ��Ľ���(����ֵȷ����ʱ�Ƿ���ȫ���˳�����Ϸ��
	virtual bool Stage_End() = 0;

	//No.4  ���ƶԻ���
	virtual void Dialog_Draw() = 0;

	//No.5  ���������˵�
	bool CreateMenu(){
		menu = LoadSurface("Menu.png");
		if (!menu){
			MessageBox(NULL, "Error Read menu,png", "Error", MB_OK);
			return false;
		}
	}
	//No.6  ���ƻ����˵�
	void DrawMenu(){
		DrawSurface(menu, 20, 20, backbuffer);
	}

	//No.7 ���ص�ǰ�Ĺؿ�״��
	STAGE_STATE GetGameState(){
		return game_state;
	}
	/*
	//No.8 ���鹦�ܣ������ɫ����Ʒ������ֹ��ɫǰ����ͬʱ�����䷢�������¼������紫��Ĳ���������������Ļ������Ǿ�
	//��ֹ��ɫ���ж������ҿ�ʼ��ʾ�Ի��򣨼�����������¼����ڷ��������ٽ��м���Ƿ����˵���,����ֻȥ����Ƿ���
	//�ո��
	bool reserchHere(SPRITE &Character, SPRITE object){

		/*
		���ȵ����ȣ������ʱ�Ѿ������˵����¼��Ļ�������ֵΪtrue��ʱ������Ӧ��ȥ����Ƿ����˿ո����ʹ�Ի�
		������������������ķ���ֵcanMoveӦ�������㺬�壨�������������ﻹҪע�⣬canMoveΪfalse��ʱ�򣬲�������
		�ж������Ҳ����ٴν��е����Լ�����Ϸ���ɣ�����������ͬʱֻ�����ⲿ�ĶԻ����¼������Ի��������ʱ�򣬽�camMove
		����Ϊtrue��ʹ��Ϸ��������
		
		//��ײ���ԣ��õ���ɫ���ĸ���Ʒ��������ײ
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

		//Ȼ���ж��Ƿ����е���(ע�⣬�����ʱ�Ѿ���ʾ�˶Ի��������ٴ���ʾ��
		float temp_destx = abs(Character.x + Character.width / 2 - (object.x + object.width / 2));
		float temp_desty = abs(Character.y + Character.height / 2 - (object.y + object.height / 2));

		//�ж��Ƿ��Ѿ����Ե���
		if (temp_destx < Character.width / 2 + object.width / 2 + 30 &&
			temp_desty < Character.height / 2 + object.height / 2 + 30){
			if (Key_Down(DIK_SPACE) && !object.dialog_show &&!object.dialogIsShowing){
				if (!spaceButton){
					object.dialog_show = true;
					spaceButton = true;
					canMove = false;
				}
			}
			//���ǣ�����Ի����Ѿ����Ƴ��������һ��ǰ����˰�ť�Ļ����Ͳ��ڻ���
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