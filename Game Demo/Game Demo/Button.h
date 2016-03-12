/*
	Date:2016.03.01
	Link
	��������Button����
*/
#include"MyDirect.h"

#ifndef BUTTON_H
#define BUTTON_H

enum BUTTON_STATE{
	BUTTON_NONE = 0,		//��ť��ȫû����ᡭ��
	BUTTON_OVER = 1,		//�ڰ�ť���Ϸ�
	BUTTON_CLICKED = 2,		//��ť������
	BUTTON_RELEASED = 3		//��ť���ɿ�
};
class Button :public SPRITE{
	//���ӵ�Ϊһ��������״̬�ı�����
public:
	BUTTON_STATE state;	//��ť�ĵ�ǰ״̬
	RECT rect;			//��ť������
	string text;		//��ť������
	//-----------��غ���--------------
	Button();
	~Button();
	//-----------�ƻ��еĺ���-----------
	//No.1 �ж��Ƿ񱻵����(����ǰ���Ϊ0����x,y�������ڵ�ʱ�򡭡���������������
	BUTTON_STATE onClicked(int x, int y,bool clicked);
	//No.2 �ڵ�ǰ״̬��Ӧ�û���ʲô����
	bool DrawButton();
};

#endif BUTTON_H