/*
	时间：2016.2.27
	作者：Link
	入口函数，不要想太多这个没什么实际的作用的知识程序入口而已要看
	程序的具体代码请前往隔壁的MainGame查看……
*/
#include"MyDirect.h"
//用来控制游戏的进行
extern bool gameover = false;
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
	case WM_DESTROY:{
		PostQuitMessage(0);
		gameover = true;
		return 0;
	}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow){

	//首先要创建一个windowsclass来设置我们的窗口
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;										//额外的空间大小
	wc.cbSize = sizeof(WNDCLASSEX);							//该类的大小（可以是别的类，然而这里我们使用的使其本身）
	wc.cbWndExtra = 0;										//额外的窗口
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//用来确定这个窗口本身的颜色（就是打开时的颜色）
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//确定这个系统中的箭头的状态
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);					//设置这个软件的图标
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);				//设置这个软件在通知栏的图标
	wc.hInstance = hInstance;								//关键，将这个类和传入的句柄参数相关联，从而实现对其的操作
	wc.lpfnWndProc = (WNDPROC)WinProc;						//与之前写好的信息处理函数相关联
	wc.lpszClassName = "Game Demo";							//这个是这个类的名字
	wc.lpszMenuName = NULL;									//设置这个窗口的菜单的名字
	wc.style = CS_HREDRAW | CS_VREDRAW;						//设置这个窗口的状态

	if (!RegisterClassEx(&wc)){
		return false;
	}

	//然后我们要设置一下这个窗口的句柄，通过这个句柄与其他的函数进行交互
	HWND window;
	//ZeroMemory(window, sizeof(HWND));
	window = CreateWindow(
		"Game Demo",
		APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		SCREENW,
		SCREENH,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (window == NULL){
		return 0;
	}
	//要记得将这个窗口的形态上传并且更行
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//这里是最基本的游戏初始化处理部分
	if (!Game_Init(window)){
		MessageBox(window, "Error Create Game", "Error", MB_OK);
		return 0;
	}

	//接下来就是信息的传递部分
	MSG message;
	while (!gameover){
		if (PeekMessage(&message, window, 0, 0, PM_REMOVE)){
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		Game_Run(window,message);
	}
	Game_End();

	return message.wParam;
}


