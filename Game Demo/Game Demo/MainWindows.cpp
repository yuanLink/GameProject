/*
	ʱ�䣺2016.2.27
	���ߣ�Link
	��ں�������Ҫ��̫�����ûʲôʵ�ʵ����õ�֪ʶ������ڶ���Ҫ��
	����ľ��������ǰ�����ڵ�MainGame�鿴����
*/
#include"MyDirect.h"
//����������Ϸ�Ľ���
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

	//����Ҫ����һ��windowsclass���������ǵĴ���
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;										//����Ŀռ��С
	wc.cbSize = sizeof(WNDCLASSEX);							//����Ĵ�С�������Ǳ���࣬Ȼ����������ʹ�õ�ʹ�䱾��
	wc.cbWndExtra = 0;										//����Ĵ���
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//����ȷ��������ڱ������ɫ�����Ǵ�ʱ����ɫ��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//ȷ�����ϵͳ�еļ�ͷ��״̬
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);					//������������ͼ��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);				//������������֪ͨ����ͼ��
	wc.hInstance = hInstance;								//�ؼ����������ʹ���ľ��������������Ӷ�ʵ�ֶ���Ĳ���
	wc.lpfnWndProc = (WNDPROC)WinProc;						//��֮ǰд�õ���Ϣ�����������
	wc.lpszClassName = "Game Demo";							//���������������
	wc.lpszMenuName = NULL;									//����������ڵĲ˵�������
	wc.style = CS_HREDRAW | CS_VREDRAW;						//����������ڵ�״̬

	if (!RegisterClassEx(&wc)){
		return false;
	}

	//Ȼ������Ҫ����һ��������ڵľ����ͨ���������������ĺ������н���
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
	//Ҫ�ǵý�������ڵ���̬�ϴ����Ҹ���
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//���������������Ϸ��ʼ��������
	if (!Game_Init(window)){
		MessageBox(window, "Error Create Game", "Error", MB_OK);
		return 0;
	}

	//������������Ϣ�Ĵ��ݲ���
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


