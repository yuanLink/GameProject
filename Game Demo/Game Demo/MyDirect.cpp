#include"MyDirect.h"

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;			//��ؼ��Ĳ��֣�����һ����������Ļ�������еĲ�����Ҫ�����ύ������
LPD3DXSPRITE spriteobj = NULL;					//�ٿؾ���Ĺؼ���

//��һ����directinput����ض��壬ֻ����������ļ����������ܶ�directinput����
LPDIRECTINPUT8 dinput = NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
LPDIRECTINPUTDEVICE8 dimouse = NULL;
//�����ʵ�Ǽ�¼�����������Ϣ
DIMOUSESTATE2 mouse_state;
POINT mouse_pos;
//����һ���ַ����������洢�������ĸ���ť
char keys[256];



//================================================
//��ʼ��Direct,���Ծ�����Ļ�Ĵ�С
bool Direct_Init(HWND hwnd, int width, int height){
	//���ȳ�ʼ���ܱ���
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d){
		return false;
	}
	//Ȼ�󴴽�һ���������洢��d3ddev��������
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.EnableAutoDepthStencil = 1;							//������Ȼ���
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//������Ȼ���ĸ�ʽ
	d3dpp.BackBufferCount = 1;									//������������
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;					//����Ϊֻ����������������ɫ��ͨ������alphaͨ����
	d3dpp.BackBufferHeight = SCREENH;							//���û���ĸ߶�
	d3dpp.BackBufferWidth = SCREENW;							//���û���Ŀ��
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;			//���û�������
	d3dpp.hDeviceWindow = hwnd;									//���ò����ľ����Ȼ����������Ϳ��Զ�����в���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//��Ⱦ�ĸ�ʽ����ʹ������Ⱦ�����������Ļ˺��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ʹ����Ⱦ��ʽ
	d3dpp.Windowed = true;										//���ڻ�

	//����d3ddev,Ҳ����֮��Ĳ����豸
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	if (!d3ddev) return false;
	
	//Ȼ��Ҫ��һЩ������Ԫ��˳��Ҳ�����ˣ����绺��;���
	HRESULT result = d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (result != D3D_OK){
		return false;
	}

	//���鴴��
	result = D3DXCreateSprite(d3ddev, &spriteobj);
	if (result != D3D_OK){
		return false;
	}

	return true;
}
//����Direct
void Direct_ShutDown(){
	if (spriteobj)spriteobj->Release();
	if (backbuffer)backbuffer->Release();
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

//============================================================
//Ȼ���������豸�ĳ�ʼ������������һЩ�������ܱ��硰���̵İ�ť������ ����λ�á�
bool DirectInput_Init(HWND hwnd){
	//ͬd3d�����ȳ�ʼ��dinput�������
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL
		);
	if (!dinput) return false;

	//Ȼ�󣬽���Ӧ���豸������
	//���Ȼ�ü��̵����
	HRESULT result = dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	if (result != DI_OK){
		return false;
	}
	//Ȼ��������Ϣ�ĸ�ʽ
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	//����������������е����ȼ������(��ռ��ϵͳ��ֻ����Ļ���ֵ�ʱ����Բ�����
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//����ø��豸(��ʵ���Ǻܶ���һ��������Ϊ��ʱ��û�г�ʼ����ɿ϶���ܾ����ʵġ�����
	result = dikeyboard->Acquire();

	//Ȼ����ͬ���Ļ�ü��̵��豸
	result = dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	if (result != DI_OK){
		return false;
	}
	dimouse->SetDataFormat(&c_dfDIMouse2);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	result = dimouse->Acquire();

	//�������,�����չʾ
	d3ddev->ShowCursor(true);

	return true;
}

//���������豸��״̬
void DirectInput_Update(){
	//����µ����״̬
	dimouse->Poll();
	if (!SUCCEEDED(dimouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mouse_state))){
	//�����ܳɹ��Ļ��״̬�Ļ���˵���������¼�������˵ʧȥ�˽���֮��ģ�����Ҫ���»���豸(�Ͼ��������³�ʼ����
		dimouse->Acquire();
	}

	//����µļ���״̬
	dikeyboard->Poll();
	if (!SUCCEEDED(dikeyboard->GetDeviceState(sizeof(keys), (void**)keys))){
		//�����ܳɹ��Ļ��״̬�Ļ���˵���������¼�������˵ʧȥ�˽���֮��ģ�����Ҫ���»���豸(�Ͼ��������³�ʼ����
		dikeyboard->Acquire();
	}

	
}

//���������豸
void DirectInput_ShutDown(){
	if (dikeyboard)dikeyboard->Release();
	if (dimouse)dimouse->Release();
	if (dinput)dinput->Release();
}

//�۲�����Ƿ���

int Mouse_Button(int button){
	//MessageBox(NULL, "Test", "test", MB_OK);
	return mouse_state.rgbButtons[button] & 0x80;
}
//�������X�����ϵ�λ��
int Mouse_X(){
	return mouse_state.lX;
}
int Mouse_Y(){
	return mouse_state.lY;
}
int Mouse_PosX(HWND hwnd){
	GetCursorPos(&mouse_pos);
	ScreenToClient(hwnd, &mouse_pos);
	return mouse_pos.x;
}
int Mouse_PosY(HWND hwnd){
	GetCursorPos(&mouse_pos);
	ScreenToClient(hwnd, &mouse_pos);
	return mouse_pos.y;
}
//���ؼ��̶�Ӧ��λ��
bool Key_Down(int key){
	return (bool)(keys[key] & 0x80);
}
//���ؼ��̴�ʱ�Ƿ��¹�ĳ����ť
/*bool Key_Up(int key){
	static bool keystate;
}
*/
//========================================================
//������صĺ������������ؼ���ͼƬ/�����ȡ�봦��
//��ȡ��ͨ��ͼƬ����Ϊ����)
LPDIRECT3DSURFACE9 LoadSurface(string filename){
	//���õı������Զ�ȡͼƬ�Ļ�����Ϣ
	D3DXIMAGE_INFO info;
	//ͼƬ����
	LPDIRECT3DSURFACE9 image = NULL;
	//���ȶ�ȡͼƬ�Ļ�����Ϣ
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK){
		MessageBox(NULL, "Error read the picture infomation","Error", MB_OK);
		return NULL;
	}
	//Ȼ���������������������ͼƬ
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width, 
		info.Height,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&image, NULL);
	if (result != D3D_OK){
		MessageBox(NULL, "Error create surface","Error", MB_OK);
		return NULL;
	}

	//****************		*******		*******		*******		******		*******
	//*******����ȡ����ͼƬ(��������ط��ҿ���Ҫ�޸�����������Ըı��ȡ��ͼƬ��͸��ͨ����
	//****************		*******		*******		*******		******		*******
	result = D3DXLoadSurfaceFromFile(image, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, &info);
	if (result != D3D_OK){
		MessageBox(NULL, "Error Load image from file", "Error", MB_OK);
		return NULL;
	}

	return image;

}
//��ȡ��Ϊ��������ͼƬ������
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor){
	//���õı������Զ�ȡͼƬ�Ļ�����Ϣ
	D3DXIMAGE_INFO info;
	//ͼƬ����
	LPDIRECT3DTEXTURE9 texture;
	//���ȶ�ȡͼƬ�Ļ�����Ϣ
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK){
		MessageBox(NULL, "Error read the picture infomation", "Error", MB_OK);
		return NULL;
	}
	//Ȼ���������������������ͼƬ
	result = D3DXCreateTextureFromFileEx(
		d3ddev,
		filename.c_str(),
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor,
		&info,
		NULL,
		&texture);
	//****************		*******		*******		*******		******		*******
	//*******����ȡ����ͼƬ(��������ط��ҿ���Ҫ�޸�����������Ըı��ȡ��ͼƬ��͸��ͨ����
	//****************		*******		*******		*******		******		*******
	//result = D3DXCreateTextureFromFile(d3ddev, filename.c_str(), &texture);
	if (result != D3D_OK){
		MessageBox(NULL, "Error Load texture from file", "Error", MB_OK);
		return NULL;
	}
	
	return texture;

}
//����ԴͼƬ���ļ��ж�ȡ�ģ����Ƶ�Ŀ��ͼƬ���󣨴����ģ�
void DrawSurface(LPDIRECT3DSURFACE9 surface, float x, float y, LPDIRECT3DSURFACE9 &dest,int width,int height,int resource_x , int resource_y){
	//�������Ҫ���µõ������ͼƬ��Ϣ�Ļ���Ҫ���¶���һ��ר�ŵı�����d��ȡ
	D3DSURFACE_DESC desc;
	surface->GetDesc(&desc);
	//����Ŀ��ͼƬ��λ��
	RECT rect1;
	rect1.left = (long)x;
	rect1.top = (long)y;
	rect1.right = (long)(x + width);
	rect1.bottom = (long)(y + height);
	//������Ҫ���Ƶķ�Χ���������Ҫ����һ����������ˣ����Ƶ�λ��Ӧ�ÿ�������Ϊ�ľ�����
	//RECT rect_source = { resour_x, resour_y, (long)(resour_x + SCREENW), (long)(resour_y + SCREENH) };
	RECT rect_source;
	if (width == 0 && height == 0)
		rect_source = { 0, 0, (long)desc.Width, (long)desc.Height };
	else
		rect_source = { resource_x, resource_y, (long)resource_x + width, (long)resource_y + height };
	//����d3ddev�豸���л���
	d3ddev->StretchRect(surface, &rect_source, dest, &rect1, D3DTEXF_NONE);
	
}
//�������ļ�(�����˶��Ļ����ǵ�֡���������Ƶ���Ļ��
void Draw_Sprite_Texture(LPDIRECT3DTEXTURE9 texture, float destx, float desty, int framenum, int framew, int frameh, int columns, D3DCOLOR tanscolor)
{
	D3DXVECTOR3 position((float)destx, (float)desty, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left =(long) (framenum % columns) * framew;
	rect.top = (long)(framenum / columns) * frameh;
	rect.right = (long)rect.left + framew;
	rect.bottom = (long)rect.top + frameh;

	spriteobj->Draw(texture, &rect, NULL, &position, white);
}
//����һ����ʱ��˳�򲥷ž���Ķ���
void Sprite_Animate(int &frame, int &starttime, int columns, int startframe, int endframe, int direction, int delay){
	long temp_time = 0;
	temp_time = (long)GetTickCount();
	if (starttime + delay < temp_time){
		starttime = GetTickCount();
		frame += direction;
		if (frame < startframe)
			frame = endframe;
		if (frame > endframe)
			frame = startframe;
	}
	
}

void Transform_Sprite(LPDIRECT3DTEXTURE9 image, float x, float y, int frame, int width, int height,
	 int columns , float rotation , float scaling , D3DCOLOR color){
	
	//�����������˵��֮ǰ�Ļ��Ʋ�ֻ࣬��������һЩ����
	//ʹ��rect1��ȷ������������Ƶľ����λ�ã�ע������Ҫ�����ſ��������棩
	//create a scale vector
	D3DXVECTOR2 scale(scaling, scaling);

	//create a translate vector
	D3DXVECTOR2 trans(x, y);

	//set center by dividing width and height by two
	D3DXVECTOR2 center((float)(width * scaling) / 2, (float)(height * scaling) / 2);

	//create 2D transformation matrix
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, rotation, &trans);

	//tell sprite object to use the transform
	spriteobj->SetTransform(&mat);

	//calculate frame location in source image
	int fx = (frame % columns) * width;
	int fy = (frame / columns) * height;
	RECT srcRect = { fx, fy, fx + width, fy + height };

	//draw the sprite frame
	spriteobj->Draw(image, &srcRect, NULL, NULL, color);
}

//��ײ���ԣ�������int��Ϊ������ȷ��������ײ��λ��
//ע�������ʽֻ���ж��Ƿ�������ײ
int CollisionRect(SPRITE sprite1, SPRITE sprite2){
	RECT rect1, rect2;
	rect1.top = (long)sprite1.y;
	rect1.left = (long)sprite1.x;
	rect1.right = (long)sprite1.width + rect1.left;
	rect1.bottom = (long)sprite1.height + rect1.top;

	rect2.top = (long)sprite2.y;
	rect2.left = (long)sprite2.x;
	rect2.right = (long)sprite2.width + rect2.left;
	rect2.bottom = (long)sprite2.height + rect2.top;

	RECT dest;
	return IntersectRect(&dest, &rect1,& rect2);
}
//��ײ���ԣ�������int��Ϊ������ȷ��������ײ��λ��
//�����ײ����Ϊ�Ҵ������ط������ģ������ж������ĸ�λ�÷�������ײ
int CollisionD(SPRITE sprite, SPRITE block){
	D3DXVECTOR2 sp1, sp2;
	long int width_dis, height_dis;
	sp1.x = (long)sprite.x+ sprite.width/2;
	sp1.y = (long)sprite.y + sprite.height / 2;

	sp2.x = (long)block.x + block.width / 2;
	sp2.y = (long)block.y + block.height / 2;

	width_dis = (sprite.width + block.width) / 2;
	height_dis = (sprite.height + block.height) / 2;
	//Ȼ����������������������������֮��ľ��룺
	//���ȣ��������µ�ʽ�ӣ�˵����ʱ�������Ͽ��ܷ�������ײ
	if (abs(sp1.x - sp2.x) < width_dis){
		//�����ݷ�����Ҳʵ������һ��Ļ���˵�� ��ʱ��ȷ�Ƿ�������ײ��
		if (abs(sp1.y - sp2.y) < height_dis){
			//��ʼѰ����ײ��λ�ã�
			//��ײ�����ڲ�ߣ����򸲸ǳ���С������
			if (height_dis - abs(sp1.y - sp2.y) > width_dis - abs(sp1.x - sp2.x)){
				//���һ����ײ�����ھ����ұ�
				if (sp1.x < sp2.x)
					return RIGHT_COLLISION;
				else
				//�����:��ײ�����ھ������
					return LEFT_COLLISION;
			}
			//������ײ��������������
			else{
				//���������ײ�����ھ�����Ϸ�
				if (sp1.y < sp2.y)
					return UP_COLLISION;
				else
					//�����:��ײ�����ھ�����·�
					return BOTTOM_COLLISION;
			}
		}
	}
	//������Ϊδ������ײ
	return 0;
}

//������ȡ���һ��ƶԻ���ĺ���������������ڿƼ���
LPDIRECT3DTEXTURE9 CreateMyDialog(string filename){
	LPDIRECT3DTEXTURE9 dialog = NULL;
	dialog = LoadTexture(filename);
	if (!dialog){
		MessageBox(NULL, "Error read Dialog image", "Error", MB_OK);
		return NULL;
	}
	return dialog;
}

//���ƶԻ���
bool DrawMyDialog(LPDIRECT3DTEXTURE9 dialog,float x,float y){
	D3DXVECTOR3 position = { x, y, 0 };
	spriteobj->Draw(dialog, NULL, NULL,&position, D3DCOLOR_ARGB(50, 102, 102, 102));
	return true;
}

//������һ�������������ֵĺ���
LPD3DXFONT MakeFont(string fontname, int size){
	LPD3DXFONT font = NULL;
	D3DXFONT_DESC desc = {
		size,
		0,
		0,
		0,
		false,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		""
	};
	strcpy(desc.FaceName, fontname.c_str());
	D3DXCreateFontIndirect(d3ddev, &desc, &font);
	return font;
}
//�����ֻ��Ƶ���Ļ��
void PrintFont(LPD3DXFONT font, float x, float y, string text, D3DCOLOR color, bool setFixed){
	RECT rect;
	if (!setFixed){
		rect = { x, y, 0, 0 };
		font->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_CALCRECT, color);
	}
	else
		rect = { x, y, 150 + x, 100 + y };
	font->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_LEFT, color);
}


