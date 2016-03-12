#ifndef BARRIER_H
#define BARRIER_H

#include"MyDirect.h"

class Barrier :public SPRITE{

public:
	bool visible;					//����Ʒ�Ƿ�ɼ�
	int No;							//����Ʒ�ı�ţ��Ա�ȷ��
	LPDIRECT3DSURFACE9 feature;		//������������ʰȡ��Ʒ�������ʾ
	string dialog;					//��Ʒ�ĵ���ʱ����ʾ������
	//bool dialog_show;				//չʾ�Ի���
	//bool dialogIsShowing;			//��ʾ��ǰ�Ի���������ʾ
	bool selected;					//��ʾ��Ʒ��ʱ��ѡ�У���ʱǿ����ֹ���е��ƶ�����������ĳ����ʵ�ֿ缶��������
	bool stuff;						//��ʾ����Ʒ�ǿ��Ա�ʰȡ�ģ���ʱ���Խ���Ʒ������Ʒ��
	LPDIRECT3DTEXTURE9 dialog_image;//�Ի���
	LPD3DXFONT font_garamond;		//�Ի����е���������
	//-----------------------------------------
	//----             ��غ���        --------
	//-----------------------------------------

	Barrier();
	~Barrier();
	//No.1 ����ȷ���Ƿ�ѡ��ĺ���
	void setVisible(bool Selected);						

	//No.2 ����ֵΪ����Ʒ�Ƿ��ڴ��ͼ�ɼ�
	bool isVisible();							

	//No.3 ��ʾ���������ĶԻ��򣬷���ֵΪfalseʱֹͣ����
	bool ShowSelectedText();						

	//No.4 ������ȡ�Ŵ�ͼƬ
	void setImage(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));

	//No.5 ��ʾ��Ʒ����д������ȶ��
	void ShowSelectedImage();						
	
	//No.6 ��ʾ����Ʒ���Ա����뵽��Ʒ��
	bool isStuff();					

	//No.7 ���������¼�(Ϊ���Ժ�Ĺ������ӣ��������Ϸ��ɫ��Ϊ�������룩
	void researchHere(SPRITE &Character,int collisionState);
};

#endif BARRIER_H