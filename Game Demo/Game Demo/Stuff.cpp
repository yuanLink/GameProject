#include"Stuff.h"

#include"Stuff.h"

Stuff::Stuff(){
	visible = true;
	No = 0;
	feature = NULL;
	dialog = new string[1];
	dialog[0] = "��������е����";
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
//No.1 ����ȷ���Ƿ�ѡ��ĺ���
void Stuff::setVisible(bool Selected){

	//��������ļ����������Ʒ�Ļ��������ｫ����Ϊ���ɼ�
	if (stuff&&Selected)
		visible = false;
}

//No.2 ����ֵΪ����Ʒ�Ƿ��ڴ��ͼ�ɼ�
bool Stuff::isVisible(){
	return visible;
}

//No.3 ��ʾ���������ĶԻ��򣬷���ֵΪfalseʱֹͣ����
bool Stuff::ShowSelectedText(){
	static int index = 0;
	if (!dialog_image)
		return false;
	if (index == dialog_num)
		return true;

	//Ȼ����ƶԻ���
	D3DXVECTOR3 position = { 0, SCREENH - 180, 0 };
	spriteobj->Draw(dialog_image, NULL, NULL, &position, 0xFFFFFFFF);
	//�����žͻ��ƶԻ�����
	PrintFont(font_garamond, 30, SCREENH - 128, dialog[index], D3DCOLOR_XRGB(255, 255, 200));
	//������һ�ζԻ�
	index++;
	//��ʱ��û�н��������Ƿ���false
	return false;
}

//No.4 ������ȡͼƬ
void Stuff::setImage(string filename, D3DCOLOR transcolor){

	//��ȡ����Ҫ��ͼƬ
	image = LoadTexture(filename, transcolor);
	if (!image){
		MessageBox(NULL, "Error read picture at setImage!", "ERROR", MB_OK);
		return;
	}
	//��ȡ�Ի���
	dialog_image = CreateMyDialog("normaldialog.png");
	//��ȡ�Ŵ���ͼƬ������

}

//No.5 ��ʾ��Ʒ����д������ȶ��
void Stuff::ShowSelectedImage(){}

//No.6 ��ʾ����Ʒ���Ա����뵽��Ʒ��
bool Stuff::isStuff(){
	return stuff;
}

//NO.7 ���������¼�
void Stuff::researchHere(SPRITE &Character, int collisionState){

	//��������ײ�¼�
	if (collisionState){
		dialog_show = true;
		Character.velx = Character.vely = 0;
		return;
	}
}