/*
	Date:2016.03.02
	Link
	�������Ϊ��һ�أ�Ŀ��Ϊ����һ����Ĺ�����Ļ���ռ�ͨ�ص�Ԫ�أ��ں��ʵĵط�ʹ�ú���ɹؿ���
*/

#ifndef FIRSTSTAGE_H
#define FIRSTSTAGE_H
#define SPEED 0.4
#define BUFFERW SCREENW*2
/*
	Author��Link
	Date:2016.03.08
	��һ�أ���ɫ����ĵ���
	ͨ��ʰȡ���ߺ�
*/

#define BUFFERH SCREENH*2

#include"Stage.h"

class FirstStage :public Stage{

	//LPDIRECT3DSURFACE9 background;//����
	LPDIRECT3DTEXTURE9 staff;				//����
	D3DXVECTOR2 menu_pos;					//�˵�����λ��
	SPRITE stone;							//ʯͷ
	float stage_valx, stage_valy;			//��ͼ�ƶ����ٶ�
	int backgroundWidth, backgroundHeight;	//��ͼ�ĳ��ȺͿ��
	int numOfBarrier;						//	��Ʒ������
	int numOfTool;							//���ߵ�����
	bool nextDialog;						//�ڶԻ������ϵġ��ո��Ƿ��µ��жϡ�
	bool DialogEvent;				//�Ի������¼��������ǽ������
	bool canMove;
	/*
	���ڶԻ���Ĵ���
	���ȵ����ȣ������ʱ�Ѿ������˵����¼��Ļ�������ֵΪtrue��ʱ������Ӧ��ȥ����Ƿ����˿ո����ʹ�Ի�
	������������������ķ���ֵcanMoveӦ�������㺬�壨�������������ﻹҪע�⣬canMoveΪfalse��ʱ�򣬲�������
	�ж������Ҳ����ٴν��е����Լ�����Ϸ���ɣ�����������ͬʱֻ�����ⲿ�ĶԻ����¼������Ի��������ʱ�򣬽�camMove
	����Ϊtrue��ʹ��Ϸ��������
	*/
public:
	FirstStage();
	~FirstStage();

	//Ȼ���һ��Ҫ���صĺ���
	//No.1  �ùؿ��ĳ�ʼ��
	bool Stage_Init(HWND hwnd);

	//No.2  �ùؿ������л���
	void Stage_Run();

	//No��2.5 �ùؿ��Ļ���
	void Stage_Draw();

	//No.3  �ùؿ��Ľ���
	bool Stage_End();

	//No 3.5  ���ƽ�������

	bool Stage_End_Draw();

	//No.4  ���ƶԻ���
	void Dialog_Draw();

	//No.5  ���������˵�(���񲻴�Ҫ���ذѡ�����
	//bool CreateMenu();

	//No.8 ���鹦�ܣ������ɫ����Ʒ������ֹ��ɫǰ����ͬʱ�����䷢�������¼������紫��Ĳ���������������Ļ������Ǿ�
	//��ֹ��ɫ���ж������ҿ�ʼ��ʾ�Ի��򣨼�����������¼����ڷ��������ٽ��м���Ƿ����˵���,����ֻȥ����Ƿ���
	//�ո��
	bool reserchHere(SPRITE &Character, SPRITE &object,bool canMove);
};

#endif FIRSTSTAGE_H