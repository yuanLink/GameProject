/*
	���ڣ�2016.02.28
	���ߣ�Link
	��Ϸ����Ҫ�߼�����
	PS��ǿ��һ�£������Ϸû����꣡�����֡���ʼ���ԣ������м������Ϊ��x��27��y:13
*/

#include"MyDirect.h"
#include"Button.h"
//#include"Stage.h"
#include"FirstStage.h"
//����Ҫȷ����Ϸ��״̬��ģ��������Ϸ״̬
//ȷ��һ�㣺��ȡ��Ϸ��ʱ��Ҫÿ�ζ����һ�γ�ʼ����������Щ״̬��������ָ��Щ�����ƵĹ��̡�
enum GAME_STATE{
	GAME_BEGIN = 0,
	GAME_PLAYING = 1,
	GAME_OVER = 2,
	GAME_STAGE1_LOAD = 3, 
	GAME_STAGE1_RUNNING = 4
};

GAME_STATE game_state = GAME_BEGIN;
//������Щ������������Ϸ�ж�Ҫʹ�ã���������Ϊȫ�ֱ���
//��Ϸ����
LPDIRECT3DSURFACE9 background;
//��Ϸ״̬����״̬�����ڻ��ƻ����ڴ�����Ϣ��
bool beginToDraw = false;
//ʹ�õ��������
LPD3DXFONT font_Arial;
//���д�����ť
Button *button;
//�Լ���ť���±�
int button_index;
//��ť�������仯��
bool button_change;
//��̨���壨�ص㣩
//LPDIRECT3DSURFACE9 backbuffer = NULL;
//��һ��
FirstStage *stage = NULL;

float timeDelta = 0;
//���������ĺ���(Ϊ�˱�֤�����ԣ����ǿ��԰Ѹ���ĳ�����״��ͼƬ����ȥ�������¼���˴��룩
bool CreateBackgroud(string filename, int width, int height){
	//���ȱ�֤���ͼƬ����û�б����Ϣ
	if (background) {
		background->Release();
		background = NULL;
	}

	//Ȼ���ȡ����ͼƬ
	LPDIRECT3DSURFACE9 image = NULL;
	image = LoadSurface(filename.c_str());
	if (!image)
		return false;
//	D3DXIMAGE_INFO info;

	//ע�⣬����Ҫ��image����ԭ����ͼ�����棬��ʱ����ѡ����չ����Ļ���
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
	//������δ����ǿ������õ�

	//Ȼ���ǽ�����ͼƬ�ύ����������
	//HRESULT result = d3ddev->StretchRect(background, NULL, backbuffer, NULL,D3DTEXF_NONE);
	//���ڲ��ǻ�����ʱ��Ҫ��d3ddev->BeginScene���ܻ����ĺð�
	//if (result != D3D_OK) return false;

	//�ǵý�ͼƬ����
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

	//������Ҫ�ѻ�����׼��׼���ã�����˵��ȡͼƬ,����ؿ�֮���
	//��Ȼ���ȵ�һ�����ǻ�����������
	if (d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK){
		return false;
	}

	//Ȼ���ǽ�����Ļ(��ʱ�Ѿ��ύ�ˣ�
	if (!CreateBackgroud("background.jpg", SCREENW, SCREENH)) return false;

	//֮���Ǵ�������Ҫ������
	font_Arial = MakeFont("Arial", 42);
	//��ť��Ҫ������ÿ��ѡ���Ĭ��Ϊ��һ��
	button = new Button[3];
	//�±�Ĭ��Ϊ��һ��
	button_index = 0;
	//��ʼ���ø���button�����
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

	//��ȡ��ǰʱ�������������
	timeDelta = GetTickCount();

	//�򿪵�һ��
	stage = new FirstStage();

	return true;
}

void Game_Run(HWND hwnd, MSG message){

	//����Ҫ���¼��̵Ĳ������
	DirectInput_Update();
	
	//����ڵ�¼ҳ���У������ťѡ�����ĸ�
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
		//Ȼ�󽫻�������������ж��Ļ�����
		button[button_index].state = BUTTON_OVER;
		//ʣ�µ���Ȼ�͵���Ϊδ��ѡ��
		button[(button_index + 3 - 1) % 3].state = BUTTON_NONE;
		button[(button_index + 3 + 1) % 3].state = BUTTON_NONE;
		//����ʱ��һ����ť�����£�������һ��
		if (Key_Down(DIK_SPACE)){
			if (button[0].state == BUTTON_OVER){
				game_state = GAME_STAGE1_LOAD;
			}
		}
	}
	//����ʱ��һ�ص�״̬
	if (stage->GetGameState() == STAGE_RUNING)
	{
	//�����ʱ��״̬Ϊ���ڻ滭��ѭ��
		if (!beginToDraw)
			stage->Stage_Run();
		beginToDraw = true;

	}

	//Ȼ��Ҫ�����Ļ
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	//Ȼ���ǿ�ʼ��Ϸ����Ļ����
	if (d3ddev->BeginScene()){
		//������Խ�����ͱ���������ȥ
		timeDelta = GetTickCount();
		//�����ж��������Ϸ״̬
		switch (game_state){
			//״̬һ�����ڿ�ʼ����
		case GAME_BEGIN:{
			//������������ȥ
			DrawSurface(background, 0, 0, backbuffer,SCREENW,SCREENH);
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			//����������ð�ť��������ʱ�����ִ���
			for (int i = 0; i < 3; i++){
				button[i].DrawButton();
			}
			PrintFont(font_Arial, 434, 313, "��ʼ����", D3DCOLOR_ARGB(255, 100, 100, 100), true);
			if (Key_Down(DIK_1))
				game_state = GAME_STAGE1_LOAD;
			//Ȼ�������Ļ������Ƿ񱻵������ʱ������
			spriteobj->End();
			break;
		}//case BEING
		case GAME_STAGE1_RUNNING:{
			//����Ϸ���л���
			if (beginToDraw){
				stage->Stage_Draw();
				if (stage->GetGameState() == STAGE_OVER)
					//���Ͻ����Ļ��棬����ȫ�Ļ��ƽ������˳��ؿ���ƽʱ����δ��ɵģ�
					if (stage->Stage_End_Draw()){
						//�����Ϸ�������ƺ������Ϸ
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
	//����֮ǰ��״̬��ָʾ���ڵ�״̬


	//��ʱ����Ϸ���г�ʼ��
	//��ʼ��ֻ����һ�κðɣ���һ����־����ʾ
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
//����������������������֡����
DWORD FrameCnt = 0; // �����ˣ���Ⱦ�˵ģ�֡��
float TimeElapsed = 0; // �Ѿ�������ʱ��
float FPS = 0; // ��ƽ����֡��
//float timeDelta = 0;//�ð��Ҳ����QVQ��������������ȫ�ֱ�����
void CalcFPS(HWND hwnd,float timeDelta){
//ÿ����һ֡������ʱ��
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
//���¼��̲���
//��ȡ��ʱ�������λ��
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
//�����˰�ť

if (Mouse_Button(0)){
//ȷ���Ѿ������涨��λ��
//if (Mouse_PosX(hwnd) > 300 && Mouse_PosX(hwnd) < 450 && Mouse_PosY(hwnd) > 200 && Mouse_PosY(hwnd) < 300  ){
//if (mouse_pos.x>button->x&mouse_pos.x<button->x + button->width&& mouse_pos.y>button->y & mouse_pos.y<button->y + button->height){
if (button->In(mouse_pos.x,mouse_pos.y)){
//�����ǰ��������λ��
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