#include"Barrier.h"

Barrier::Barrier(){
	visible = true;
	No = 0;
	feature = NULL;
	dialog = "���û��ʲô�ÿ���";
	selected = false;
	stuff = false;
	dialog_image = NULL;
	font_garamond = MakeFont("Garamond", 42);
	dialog_show = false;
	dialogIsShowing = false;
}
Barrier::~Barrier(){
	if (feature){
		feature->Release();
		feature = NULL;
	}

	if (image)image->Release();
}
//No.1 ����ȷ���Ƿ�ѡ��ĺ���
void Barrier::setVisible(bool Selected){

	//��������ļ����������Ʒ�Ļ��������ｫ����Ϊ���ɼ�
	if (stuff&&Selected)
		visible = false;
}

//No.2 ����ֵΪ����Ʒ�Ƿ��ڴ��ͼ�ɼ�
bool Barrier::isVisible(){
	return visible;
}

//No.3 ��ʾ���������ĶԻ��򣬷���ֵΪfalseʱֹͣ����
bool Barrier::ShowSelectedText(){

	static int index = 0;

	if (!dialog_image)
		return false;

	
	//Ȼ����ƶԻ���
	D3DXVECTOR3 position = { 0, SCREENH - 180, 0 };
	spriteobj->Draw(dialog_image, NULL, NULL, &position, 0xFFFFFFFF);
	//�����žͻ��ƶԻ�����
	PrintFont(font_garamond, 30, SCREENH -128, dialog, D3DCOLOR_XRGB(255, 255, 200));
	return false;

}

//No.4 ������ȡͼƬ
void Barrier::setImage(string filename, D3DCOLOR transcolor){

	//��ȡ����Ҫ��ͼƬ
	image = LoadTexture(filename, transcolor);

	//��ȡ�Ի���
	dialog_image = CreateMyDialog("normaldialog.png");
	//��ȡ�Ŵ���ͼƬ������
	
}

//No.5 ��ʾ��Ʒ����д������ȶ��
void Barrier::ShowSelectedImage(){}

//No.6 ��ʾ����Ʒ���Ա����뵽��Ʒ��
bool Barrier::isStuff(){
	return stuff;
}

//NO.7 ���������¼�
void Barrier::researchHere(SPRITE &Character, int collisionState){

	//��������ײ�¼�
	if (collisionState){
		dialog_show = true;
		Character.velx = Character.vely = 0;
		return;
	}
}