/*
	ʱ�䣺2016.2.27
	���ߣ�Link
	����Ϸ����Ļ���ͷ�ļ�
*/
#pragma once
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
//�����ﶨ��һЩ�����ĺ�
#define LEFT_COLLISION 101
#define UP_COLLISION 102
#define RIGHT_COLLISION 103
#define BOTTOM_COLLISION 104
//����һЩ״̬
#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_ROLL 2
//����ͷ�ļ�
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<ctime>
using namespace std;
//------------------------------------------------------
//������Ҫ�����Ŀ��ļ�
//����Ҫ��direct3d���ܾ��
#pragma comment(lib,"d3d9.lib")
//�ܶ�����ĺ���������massagebox����Ϣ���ӣ�Ҫ���������
#pragma comment(lib,"user32.lib")
//direct3d���ܴ���ģ�����ͼƬ�Ŀ�(���羫�鰡������
#pragma comment(lib,"d3dx9.lib")
//��������Ŀ⣬��windows��صı�̶�Ҫ�������
#pragma comment(lib,"winmm.lib")
//����ʹ��direct��˵������Ŀ�
#pragma comment(lib,"dxguid.lib")
//ʹ��directinput��Ҳ���ǲٿ���
#pragma comment(lib,"dinput8.lib")
//��ͼ�Ĺؼ��⣬�������кܶ�Create�������������ֲ���
#pragma comment(lib,"gdi32.lib")

//------------------------------------------------------
//Ȼ���ǻ����ı�������
//���ȶ���һЩ�������õõ��ı�������������Ļ�Ĵ�С
const int SCREENH = 768;
const int SCREENW = 1024;
const string APPTITLE = "Game Demo";
//��һ����direct3dʹ�õĻ��������ֻ����������������������ܶ�direct���в���
//���device���Ǿ���Ĳ�������,ע�������������������������ļ���ҲҪ��������Ҫ����Ϊextern
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern bool gameover;//������Ϸ��ѭ���Ƿ�����
//Ȼ������صĺ���
//��ʼ��Direct,���Ծ�����Ļ�Ĵ�С
bool Direct_Init(HWND hwnd, int width, int height);
//����Direct
void Direct_ShutDown();

//��һ����directinput����ض��壬ֻ����������ļ����������ܶ�directinput����
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern DIMOUSESTATE2 mouse_state;
//Ȼ������صĳ�ʼ������������һЩ�������ܱ��硰���̵İ�ť������ ����λ�á�
bool DirectInput_Init(HWND hwnd);
//���¼��̲���
void DirectInput_Update();
void DirectInput_ShutDown();
//�۲�����Ƿ���
int Mouse_Button(int);
//��������λ��
int Mouse_X();
int Mouse_Y();
int Mouse_PosX(HWND);
int Mouse_PosY(HWND);
//���ؼ��̶�Ӧ��λ���Ƿ񱻰���
bool Key_Down(int);
//��ǿ��İ�ť���ж϶�Ӧλ���Ƿ��ڱ����º�����
//bool Key_Up(int);

//�������ǹؼ���ͼƬ��ػ���,������Ҳ���ڲ�ͬ���ļ�ʹ�ã���Gameѭ����Ҫ����ʹ�ã�������Ҳ����Ϊextern
//���µ����������ǿ�����DirectInit�г�ʼ����
extern LPDIRECT3DSURFACE9 backbuffer;			//��ؼ��Ĳ��֣�����һ����������Ļ�������еĲ�����Ҫ�����ύ������
extern LPD3DXSPRITE spriteobj;					//�ٿؾ���Ĺؼ���

//������صĺ������������ؼ���ͼƬ/�����ȡ�봦��
//��ȡ��ͨ��ͼƬ����Ϊ����)
LPDIRECT3DSURFACE9 LoadSurface(string filename);
//��ȡ��Ϊ��������ͼƬ������
LPDIRECT3DTEXTURE9 LoadTexture(string filename ,D3DCOLOR transcolor = D3DCOLOR_XRGB(255,255,255));
//����ԴͼƬ���ļ��ж�ȡ�ģ����Ƶ�Ŀ��ͼƬ���󣨴����ģ�
void DrawSurface(LPDIRECT3DSURFACE9 surface, float x, float y, LPDIRECT3DSURFACE9 &dest,int width = 0,int height = 0,int dest_x = 0,int dest_y = 0);
//�������ļ�(�����˶��Ļ����ǵ�֡���������Ƶ���Ļ��
void Draw_Sprite_Texture(LPDIRECT3DTEXTURE9 teture,
	float x, float y,											//��Ҫ���Ƶ�����
	int frame = 0,											//���Ƶĸ�֡��Ĭ��Ϊ0
	int framew = 64,										//��֡ÿһ���Ŀ��
	int frameh = 64,										//��֡ÿһ���ĸ߶�
	int columns	= 1,										//��������ж�����
	D3DCOLOR tanscolor = D3DCOLOR_XRGB(255, 255, 255));		//͸������ɫ
//����һ����ʱ��˳�򲥷ž���Ķ���
void Sprite_Animate(int &frame, 
	int &starttime,								//�����Ŀ�ʼʱ��
	int columns,								//�����ж�����
	int startframe,								//��ʼ��֡��
	int endframe,								//������֡��
	int direction,								//ÿ�ο�Խ����֡
	int delay);									//�ӳپ���˵����ý�����һ֡

void Transform_Sprite(LPDIRECT3DTEXTURE9 teture,
	float x, float y,										//��Ҫ���Ƶ�����
	int frame = 0,											//���Ƶĸ�֡��Ĭ��Ϊ0
	int framew = 64,										//��֡ÿһ���Ŀ��
	int frameh = 64,										//��֡ÿһ���ĸ߶�
	int columns = 1,										//��������ж�����
	float rotation = 0.0f,									//��ת�ĽǶ�
	float scaling = 1.0f,									//���ŵĳ̶�
	D3DCOLOR tanscolor = D3DCOLOR_XRGB(255,255,255));		//͸������ɫ


//===================================================================
//һ���Ǳ������֣�����˵���Ը��ĵĺ����ͽṹ��/��
//�����ҵ��뷨��Ϊ�����ӵ��ù����еļ���ԣ���ʹ���˽ṹ�壬��Ϊ��̫��Ҫ���鱾��ĺ�����Ҳ��̫��Ҫ�����ܹ��̳�ʲô��
//Ȼ���Ҿ��õ�ȷ����Ҫ������Ϊ���õķ����ģ���Ϊ���ǻ��а�ť���������ť�Ǻ��б�Ҫ��Ϊ������ڵģ���Ϊ����Ҫ���
//��ť�ܹ��о������ĵ��֮���
//�������ڿ�����ṹ����ͬ���������������
/*
typedef struct SPRITE{
	float x, y;//����
	int width, height;//��С
	int framenum;//����Ŀǰ���ڵ�֡�ı��
	int startframe, endframe;//�������Ŀ�ʼ�ͽ���֡
	int direction, delay;//����ÿ��ǰ����֡���������ӳ�
	int columns;//�ܹ��ж�����
	float scaling, rotation;//���ű�������ת�����
	LPDIRECT3DTEXTURE9 image;//�����ͼƬ
	D3DCOLOR color;//��Ϊ������͸����ɫ
	float velx, vely;//�ں᷽���ϵ��ٶȺ��ݷ����ϵ��ٶ�
	SPRITE(){
		x = y = 0;
		width = height = 64;
		framenum = 0;
		startframe = endframe = 0;
		direction = 1;
		delay = 30;
		columns = 0;
		scaling = 1.0f;
		rotation = 0.0f;
		image = NULL;
		color = D3DCOLOR_XRGB(255, 255, 255);
		velx = vely = 0;
	}
}SPRITE;
*/
class SPRITE {
public:
	float x, y;//����
	int width, height;//��С
	int framenum;//����Ŀǰ���ڵ�֡�ı��
	int startframe, endframe;//�������Ŀ�ʼ�ͽ���֡
	int direction, delay;//����ÿ��ǰ����֡���������ӳ�
	int columns;//�ܹ��ж�����
	float scaling, rotation;//���ű�������ת�����
	LPDIRECT3DTEXTURE9 image;//�����ͼƬ
	D3DCOLOR color;//��Ϊ������͸����ɫ
	float velx, vely;//�ں᷽���ϵ��ٶȺ��ݷ����ϵ��ٶ�

	bool dialog_show;				//չʾ�Ի���
	bool dialogIsShowing;			//��ʾ��ǰ�Ի���������ʾ
	int dialog_num;				//ĿǰΪֹ�ڽ��ڼ��λ�
	SPRITE(){
		x = y = 0;
		width = height = 64;
		framenum = 0;
		startframe = endframe = 0;
		direction = 1;
		delay = 30;
		columns = 1;
		scaling = 1.0f;
		rotation = 0.0f;
		image = NULL;
		color = D3DCOLOR_XRGB(255, 255, 255);
		velx = vely = 0;

		dialog_show = false;
		dialogIsShowing = false;
		dialog_num = 1;
	}
	~SPRITE(){}
	//������һ����������ʾ��ѡȡ��λ�á�
	bool In(int pos_x, int pos_y){
		return (pos_x > x&&pos_x< x + width &&pos_y>y&&pos_y < y + height);
	}
};
//��ײ���ԣ�������int��Ϊ������ȷ��������ײ��λ��
int CollisionRect(SPRITE sprite1,SPRITE block);				//�����������������ں��涨��
//��ײ���ԣ�������int��Ϊ������ȷ��������ײ��λ��
int CollisionD(SPRITE sprite1,SPRITE block);

//��Ϸ�ĳ�ʼ��
bool Game_Init(HWND hwnd);
//��Ϸ����(MSG Ϊ�ڿƼ���
void Game_Run(HWND window,MSG messsage);
//��Ϸ����
void Game_End();

//��Ϸ��������Ĵ���(�����������������Ϊ��ȫ�ֱ������������贫�������
bool CreateBackgroud(string filename, int width, int height);

//---------------------
//�Ի�����
//---------------------
//LPDIRECT3DSURFACE9 normal_dialog;
//�Ի���Ľ������о��ö�ط���Ҫ�õ����������ֲ�С�ĺ�FirstStage�ĶԻ�������ˡ�����������Ǻá���������
//����Ĺ���ֻ�Ǵ����Ǹ�ͼ������������������ݵĻ��ƻ���Ҫ�ڸ����ط���ɵ�
LPDIRECT3DTEXTURE9 CreateMyDialog(string filaname);
//���ƶԻ���
bool DrawMyDialog(LPDIRECT3DTEXTURE9 dialog, float x, float y);

//---------------------
//�������ֵĺ���
//---------------------
LPD3DXFONT MakeFont(string fontname,int size);
//�����ֻ��Ƶ���Ļ��
void PrintFont(LPD3DXFONT font, float x, float y, string text, 
	D3DCOLOR color = D3DCOLOR_XRGB(255,255,255),bool setFixed = false);

//��ѧ��ļ��ɣ��ҵ�֡��
extern DWORD FrameCnt;				 // �����ˣ���Ⱦ�˵ģ�֡��
extern float TimeElapsed;			// �Ѿ�������ʱ��
extern float FPS;					// ��ƽ����֡��
void CalcFPS(HWND,float timeDelta);//���㷢����֡����ע�������timeDeltaΪ��֮֡���ʱ���