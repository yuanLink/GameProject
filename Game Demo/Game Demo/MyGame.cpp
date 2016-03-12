/*
	日期：2016.02.28
	作者：Link
	游戏的主要逻辑部分
	PS：强调一下，这个游戏没有鼠标！！文字“开始测试：放在中间的坐标为：x：27，y:13
*/

#include"MyDirect.h"
#include"Button.h"
//#include"Stage.h"
#include"FirstStage.h"
//首先要确定游戏的状态，模仿声明游戏状态
//确定一点：读取游戏的时候不要每次都完成一次初始化，所以这些状态不如来代指那些“绘制的过程”
enum GAME_STATE{
	GAME_BEGIN = 0,
	GAME_PLAYING = 1,
	GAME_OVER = 2,
	GAME_STAGE1_LOAD = 3, 
	GAME_STAGE1_RUNNING = 4
};

GAME_STATE game_state = GAME_BEGIN;
//由于这些变量在整个游戏中都要使用，所以声明为全局变量
//游戏背景
LPDIRECT3DSURFACE9 background;
//游戏状态（该状态下是在绘制还是在处理信息）
bool beginToDraw = false;
//使用的字体情况
LPD3DXFONT font_Arial;
//还有创建按钮
Button *button;
//以及按钮的下标
int button_index;
//按钮发生过变化吗？
bool button_change;
//后台缓冲（重点）
//LPDIRECT3DSURFACE9 backbuffer = NULL;
//第一关
FirstStage *stage = NULL;

float timeDelta = 0;
//创建背景的函数(为了保证重用性，就是可以把更大的长条形状的图片画进去，这里记录下了代码）
bool CreateBackgroud(string filename, int width, int height){
	//首先保证这个图片里面没有别的信息
	if (background) {
		background->Release();
		background = NULL;
	}

	//然后读取这张图片
	LPDIRECT3DSURFACE9 image = NULL;
	image = LoadSurface(filename.c_str());
	if (!image)
		return false;
//	D3DXIMAGE_INFO info;

	//注意，这里要把image画到原来的图像上面，此时可以选择扩展更大的画面
	HRESULT	result = d3ddev->CreateOffscreenPlainSurface(
		width,
		height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&background, NULL);

	if (result != D3D_OK) return false;
	RECT rect_source = { 0, 0, SCREENW, SCREENH };
	RECT rect_dest = { 0, 0, SCREENW, SCREENH };
	d3ddev->StretchRect(image, &rect_source, background, &rect_dest, D3DTEXF_NONE);
	//以上这段代码是可以重用的

	//然后是将这张图片提交到缓冲区上
	//HRESULT result = d3ddev->StretchRect(background, NULL, backbuffer, NULL,D3DTEXF_NONE);
	//现在不是画画的时候！要等d3ddev->BeginScene才能画画的好吧
	//if (result != D3D_OK) return false;

	//记得将图片消除
	image->Release();

	return true;

}

bool Game_Init(HWND hwnd){
	if (!Direct_Init(hwnd, SCREENW, SCREENH)){
		MessageBox(hwnd, "Error Create Direct", "Error", MB_OK);
		return false;
	}
	if (!DirectInput_Init(hwnd)){
		MessageBox(hwnd, "Error Create Direct Input", "Error", MB_OK);
		return false;
	}

	//在这里要把基本的准备准备好，比如说读取图片,读入关卡之类的
	//当然首先第一件事是缓冲区的设立
	if (d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK){
		return false;
	}

	//然后是建立屏幕(此时已经提交了）
	if (!CreateBackgroud("background.jpg", SCREENW, SCREENH)) return false;

	//之后是创建所需要的字体
	font_Arial = MakeFont("Arial", 42);
	//按钮得要三个，每次选择的默认为第一个
	button = new Button[3];
	//下标默认为第一个
	button_index = 0;
	//开始设置各个button的情况
	for (int i = 0; i < 3; i++){
		button[i].width = 210;
		button[i].height = 70;
		button[i].state = BUTTON_NONE;
		button[i].x = (SCREENW - 210)/2;
		button[i].y =(float) 300 + i * 100;
	}
	button[button_index].framenum = 1;
	button_change = false;
	//button->width = 210; button->height = 70;
	//button->x = 300; button->y = 300; button->state = BUTTON_NONE;

	//获取当前时间来搞个大新闻
	timeDelta = GetTickCount();

	//打开第一关
	stage = new FirstStage();

	return true;
}

void Game_Run(HWND hwnd, MSG message){

	//首先要更新键盘的操作情况
	DirectInput_Update();
	
	//检测在登录页面中，这个按钮选择是哪个
	if (game_state == GAME_BEGIN){
		if (Key_Down(DIK_DOWN))
		{

			if (!button_change){
				button_change = true;
				button_index = (button_index + 1) % 3;
			}
		}
		else if (Key_Down(DIK_UP))
		{
			if (!button_change){
				button_change = true;
				button_index = (button_index + 3 - 1) % 3;
			}
		}
		else{
			button_change = false;
		}
		//然后将画面调整至可以行动的画面上
		button[button_index].state = BUTTON_OVER;
		//剩下的自然就调整为未被选中
		button[(button_index + 3 - 1) % 3].state = BUTTON_NONE;
		button[(button_index + 3 + 1) % 3].state = BUTTON_NONE;
		//若此时第一个按钮被按下，则进入第一关
		if (Key_Down(DIK_SPACE)){
			if (button[0].state == BUTTON_OVER){
				game_state = GAME_STAGE1_LOAD;
			}
		}
	}
	//检查此时第一关的状态
	if (stage->GetGameState() == STAGE_RUNING)
	{
	//假如此时的状态为不在绘画的循环
		if (!beginToDraw)
			stage->Stage_Run();
		beginToDraw = true;

	}

	//然后要清空屏幕
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	//然后是开始游戏的屏幕绘制
	if (d3ddev->BeginScene()){
		//这里可以将精灵和背景绘制上去
		timeDelta = GetTickCount();
		//首先判断这里的游戏状态
		switch (game_state){
			//状态一：正在开始画面
		case GAME_BEGIN:{
			//将背景绘制上去
			DrawSurface(background, 0, 0, backbuffer,SCREENW,SCREENH);
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			//这里可以设置按钮，这里暂时用文字代替
			for (int i = 0; i < 3; i++){
				button[i].DrawButton();
			}
			PrintFont(font_Arial, 434, 313, "开始测试", D3DCOLOR_ARGB(255, 100, 100, 100), true);
			if (Key_Down(DIK_1))
				game_state = GAME_STAGE1_LOAD;
			//然后更新屏幕，检测是否被点击（暂时不做）
			spriteobj->End();
			break;
		}//case BEING
		case GAME_STAGE1_RUNNING:{
			//将游戏进行绘制
			if (beginToDraw){
				stage->Stage_Draw();
				if (stage->GetGameState() == STAGE_OVER)
					//画上结束的画面，若完全的绘制结束后，退出关卡（平时都是未完成的）
					if (stage->Stage_End_Draw()){
						//完成游戏的最后绘制后结束游戏
						stage->Stage_End();
						break;
					}
				beginToDraw = false;
			}
		}//case Stage 1
		}//switch
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
		timeDelta = GetTickCount() - timeDelta;
		CalcFPS(hwnd, timeDelta);
	}
	//根据之前的状态表，指示现在的状态


	//此时将游戏进行初始化
	//初始化只进行一次好吧，用一个标志来表示
	switch (game_state){
	case GAME_STAGE1_LOAD:{
		if (!stage->Stage_Init(hwnd)){
			MessageBox(hwnd, "Error Create the First Stage", "Error", MB_OK);
			gameover = true;
			break;
		}
		game_state = GAME_STAGE1_RUNNING;
	}//case stage 1
	}
	if (Key_Down(DIK_ESCAPE))
		gameover = true;

}

void Game_End(){
	if (background)background->Release();
	if (backbuffer) backbuffer->Release();
	DirectInput_ShutDown();
	Direct_ShutDown();

}
//这里的这个函数是用来计算帧数的
DWORD FrameCnt = 0; // 发生了（渲染了的）帧数
float TimeElapsed = 0; // 已经经过的时间
float FPS = 0; // （平均）帧率
//float timeDelta = 0;//好吧我不想的QVQ可是这个必须得是全局变量啊
void CalcFPS(HWND hwnd,float timeDelta){
//每播放一帧，增加时间
	FrameCnt++;
	TimeElapsed += timeDelta;
	if (TimeElapsed > 1000.0f){
		FPS =(float) FrameCnt / 1;
		FrameCnt = 0;
		TimeElapsed = 0;
		std::ostringstream os;
		os << "FPS:" << FPS;
		string fpsString = os.str();
		SetWindowText(hwnd, fpsString.c_str());

	}
}

/*
//更新键盘操作
//获取此时鼠标点击的位置
POINT mouse_pos;
mouse_pos = message.pt;
//ClientToScreen(hwnd, &mouse_pos);
ScreenToClient(hwnd, &mouse_pos);
if (mouse_pos.x>button->x&mouse_pos.x<button->x + button->width&& mouse_pos.y>button->y & mouse_pos.y<button->y + button->height)
if (button->state == BUTTON_CLICKED)
button->state = BUTTON_RELEASED;
else
button->state = BUTTON_OVER;
else
button->state = BUTTON_NONE;
//点中了按钮

if (Mouse_Button(0)){
//确认已经按到规定的位置
//if (Mouse_PosX(hwnd) > 300 && Mouse_PosX(hwnd) < 450 && Mouse_PosY(hwnd) > 200 && Mouse_PosY(hwnd) < 300  ){
//if (mouse_pos.x>button->x&mouse_pos.x<button->x + button->width&& mouse_pos.y>button->y & mouse_pos.y<button->y + button->height){
if (button->In(mouse_pos.x,mouse_pos.y)){
//当做是按到了入口位置
//MessageBox(hwnd, "CLIck", "Click", MB_OK);
button->state = BUTTON_CLICKED;
//game_state = GAME_PLAYING;
stringstream ss;
ss << mouse_pos.x << "," << mouse_pos.y;
string temp;
temp = ss.str();
MessageBox(hwnd, temp.c_str(), "position", MB_OK);
}
}
*/