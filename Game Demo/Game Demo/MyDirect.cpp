#include"MyDirect.h"

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;			//最关键的部分，这是一个“缓冲屏幕”，所有的操作都要优先提交到这里
LPD3DXSPRITE spriteobj = NULL;					//操控精灵的关键类

//这一段是directinput的相关定义，只有利用下面的几个变量才能对directinput操作
LPDIRECTINPUT8 dinput = NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
LPDIRECTINPUTDEVICE8 dimouse = NULL;
//这个其实是记录的鼠标点击的信息
DIMOUSESTATE2 mouse_state;
POINT mouse_pos;
//增加一个字符串数组来存储按下了哪个按钮
char keys[256];



//================================================
//初始化Direct,可以决定屏幕的大小
bool Direct_Init(HWND hwnd, int width, int height){
	//首先初始化总变量
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d){
		return false;
	}
	//然后创建一个变量来存储到d3ddev各类属性
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.EnableAutoDepthStencil = 1;							//设置深度缓冲
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//设置深度缓冲的格式
	d3dpp.BackBufferCount = 1;									//背景缓冲数量
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;					//设置为只保留红蓝绿三种颜色的通道（无alpha通道）
	d3dpp.BackBufferHeight = SCREENH;							//设置缓冲的高度
	d3dpp.BackBufferWidth = SCREENW;							//设置缓冲的宽度
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;			//设置缓冲种类
	d3dpp.hDeviceWindow = hwnd;									//设置操作的句柄，然后这个参数就可以对其进行操作
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//渲染的格式，即使快速渲染，可能造成屏幕撕裂
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//使用渲染方式
	d3dpp.Windowed = true;										//窗口化

	//创建d3ddev,也就是之后的操作设备
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	if (!d3ddev) return false;
	
	//然后要将一些基本的元素顺便也创建了，比如缓冲和精灵
	HRESULT result = d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (result != D3D_OK){
		return false;
	}

	//精灵创建
	result = D3DXCreateSprite(d3ddev, &spriteobj);
	if (result != D3D_OK){
		return false;
	}

	return true;
}
//销毁Direct
void Direct_ShutDown(){
	if (spriteobj)spriteobj->Release();
	if (backbuffer)backbuffer->Release();
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

//============================================================
//然后是输入设备的初始化函数，还有一些辅助功能比如“键盘的按钮”，“ 鼠标的位置”
bool DirectInput_Init(HWND hwnd){
	//同d3d，首先初始化dinput句柄变量
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL
		);
	if (!dinput) return false;

	//然后，将对应的设备设置上
	//首先获得键盘的情况
	HRESULT result = dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	if (result != DI_OK){
		return false;
	}
	//然后设置信息的格式
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	//并且设置其在软件中的优先级的情况(不占用系统，只在屏幕出现的时候可以操作）
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//最后获得该设备(其实不是很懂这一步……因为此时都没有初始化完成肯定会拒绝访问的……）
	result = dikeyboard->Acquire();

	//然后是同样的获得键盘的设备
	result = dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	if (result != DI_OK){
		return false;
	}
	dimouse->SetDataFormat(&c_dfDIMouse2);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	result = dimouse->Acquire();

	//最后的最后,把鼠标展示
	d3ddev->ShowCursor(true);

	return true;
}

//更新输入设备的状态
void DirectInput_Update(){
	//获得新的鼠标状态
	dimouse->Poll();
	if (!SUCCEEDED(dimouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mouse_state))){
	//若不能成功的获得状态的话，说明发生了事件（比如说失去了焦点之类的）所以要重新获得设备(毕竟不能重新初始化）
		dimouse->Acquire();
	}

	//获得新的键盘状态
	dikeyboard->Poll();
	if (!SUCCEEDED(dikeyboard->GetDeviceState(sizeof(keys), (void**)keys))){
		//若不能成功的获得状态的话，说明发生了事件（比如说失去了焦点之类的）所以要重新获得设备(毕竟不能重新初始化）
		dikeyboard->Acquire();
	}

	
}

//销毁输入设备
void DirectInput_ShutDown(){
	if (dikeyboard)dikeyboard->Release();
	if (dimouse)dimouse->Release();
	if (dinput)dinput->Release();
}

//观察鼠标是否按下

int Mouse_Button(int button){
	//MessageBox(NULL, "Test", "test", MB_OK);
	return mouse_state.rgbButtons[button] & 0x80;
}
//返回鼠标X方向上的位移
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
//返回键盘对应的位置
bool Key_Down(int key){
	return (bool)(keys[key] & 0x80);
}
//返回键盘此时是否按下过某个按钮
/*bool Key_Up(int key){
	static bool keystate;
}
*/
//========================================================
//这是相关的函数————关键的图片/精灵读取与处理
//读取普通的图片（作为背景)
LPDIRECT3DSURFACE9 LoadSurface(string filename){
	//设置的变量可以读取图片的基本信息
	D3DXIMAGE_INFO info;
	//图片变量
	LPDIRECT3DSURFACE9 image = NULL;
	//首先读取图片的基本信息
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK){
		MessageBox(NULL, "Error read the picture infomation","Error", MB_OK);
		return NULL;
	}
	//然后是利用这个变量来创建图片
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
	//*******最后读取这张图片(讲真这个地方我可能要修改这个函数可以改变读取的图片的透明通道）
	//****************		*******		*******		*******		******		*******
	result = D3DXLoadSurfaceFromFile(image, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, &info);
	if (result != D3D_OK){
		MessageBox(NULL, "Error Load image from file", "Error", MB_OK);
		return NULL;
	}

	return image;

}
//读取作为精灵对象的图片（纹理）
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor){
	//设置的变量可以读取图片的基本信息
	D3DXIMAGE_INFO info;
	//图片变量
	LPDIRECT3DTEXTURE9 texture;
	//首先读取图片的基本信息
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK){
		MessageBox(NULL, "Error read the picture infomation", "Error", MB_OK);
		return NULL;
	}
	//然后是利用这个变量来创建图片
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
	//*******最后读取这张图片(讲真这个地方我可能要修改这个函数可以改变读取的图片的透明通道）
	//****************		*******		*******		*******		******		*******
	//result = D3DXCreateTextureFromFile(d3ddev, filename.c_str(), &texture);
	if (result != D3D_OK){
		MessageBox(NULL, "Error Load texture from file", "Error", MB_OK);
		return NULL;
	}
	
	return texture;

}
//将资源图片（文件中读取的）绘制到目标图片对象（处理后的）
void DrawSurface(LPDIRECT3DSURFACE9 surface, float x, float y, LPDIRECT3DSURFACE9 &dest,int width,int height,int resource_x , int resource_y){
	//这里如果要重新得到读入的图片信息的话，要重新定义一个专门的变量来d读取
	D3DSURFACE_DESC desc;
	surface->GetDesc(&desc);
	//设置目标图片的位置
	RECT rect1;
	rect1.left = (long)x;
	rect1.top = (long)y;
	rect1.right = (long)(x + width);
	rect1.bottom = (long)(y + height);
	//设置需要绘制的范围（这里打算要更改一下这个函数了，绘制的位置应该可以由人为的决定）
	//RECT rect_source = { resour_x, resour_y, (long)(resour_x + SCREENW), (long)(resour_y + SCREENH) };
	RECT rect_source;
	if (width == 0 && height == 0)
		rect_source = { 0, 0, (long)desc.Width, (long)desc.Height };
	else
		rect_source = { resource_x, resource_y, (long)resource_x + width, (long)resource_y + height };
	//利用d3ddev设备进行绘制
	d3ddev->StretchRect(surface, &rect_source, dest, &rect1, D3DTEXF_NONE);
	
}
//将精灵文件(可以运动的或者是单帧的纹理）绘制到屏幕上
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
//按照一定的时间顺序播放精灵的动画
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
	
	//这个本质上来说和之前的绘制差不多，只不过多了一些功能
	//使用rect1来确定这个即将绘制的矩阵的位置（注意这里要把缩放考虑在里面）
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

//碰撞测试，这里用int作为返回来确定发生碰撞的位置
//注意这个韩式只能判断是否发生了碰撞
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
//碰撞测试，这里用int作为返回来确定发生碰撞的位置
//这个碰撞测试为我从其他地方看到的，可以判断是在哪个位置发生的碰撞
int CollisionD(SPRITE sprite, SPRITE block){
	D3DXVECTOR2 sp1, sp2;
	long int width_dis, height_dis;
	sp1.x = (long)sprite.x+ sprite.width/2;
	sp1.y = (long)sprite.y + sprite.height / 2;

	sp2.x = (long)block.x + block.width / 2;
	sp2.y = (long)block.y + block.height / 2;

	width_dis = (sprite.width + block.width) / 2;
	height_dis = (sprite.height + block.height) / 2;
	//然后利用这两个变量计算两个精灵之间的距离：
	//首先，若满足下等式子，说明此时横坐标上可能发生了碰撞
	if (abs(sp1.x - sp2.x) < width_dis){
		//若在纵方向上也实现了这一点的话，说明 此时的确是发生了碰撞的
		if (abs(sp1.y - sp2.y) < height_dis){
			//开始寻找碰撞的位置：
			//碰撞发生在侧边（横向覆盖长度小于纵向）
			if (height_dis - abs(sp1.y - sp2.y) > width_dis - abs(sp1.x - sp2.x)){
				//情况一：碰撞发生在精灵右边
				if (sp1.x < sp2.x)
					return RIGHT_COLLISION;
				else
				//情况二:碰撞发生在精灵左边
					return LEFT_COLLISION;
			}
			//否则，碰撞发生在上下两面
			else{
				//情况三：碰撞发生在精灵的上方
				if (sp1.y < sp2.y)
					return UP_COLLISION;
				else
					//情况二:碰撞发生在精灵的下方
					return BOTTOM_COLLISION;
			}
		}
	}
	//否则视为未发生碰撞
	return 0;
}

//用来读取并且绘制对话框的函数（先试试这个黑科技）
LPDIRECT3DTEXTURE9 CreateMyDialog(string filename){
	LPDIRECT3DTEXTURE9 dialog = NULL;
	dialog = LoadTexture(filename);
	if (!dialog){
		MessageBox(NULL, "Error read Dialog image", "Error", MB_OK);
		return NULL;
	}
	return dialog;
}

//绘制对话框
bool DrawMyDialog(LPDIRECT3DTEXTURE9 dialog,float x,float y){
	D3DXVECTOR3 position = { x, y, 0 };
	spriteobj->Draw(dialog, NULL, NULL,&position, D3DCOLOR_ARGB(50, 102, 102, 102));
	return true;
}

//忘记了一个用来创建文字的函数
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
//将汉字绘制到屏幕上
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


