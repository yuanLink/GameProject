#include"FirstStage.h"

FirstStage::FirstStage(){
	score = 0;//���ط���
	background = NULL;//����
	backgroundWidth = SCREENW*2;  
	backgroundHeight = SCREENH*2;

	barrier = NULL;//�ùؿ��ϵ�����Ԫ�أ�����ʲô�ϰ��ﰡ֮��ģ�
	tool = NULL;//�ùؿ��ϵĵ���Ԫ�أ�Ҳ�趨Ϊ�գ�

	numOfBarrier = 2;//������������һ����
	numOfTool = 1;//���ߵ�����

	string_index = 0;//�ùؿ��ϵĵڼ��ζԻ�

	dialog = new string[1];//�Ի���ֻ��һ��
	font_arial = NULL;//����
	spaceButton = false;

	button = NULL;//��ť�ĸ���
	
	staff = NULL;//������

	X = Y = 0;//�趨���ͼ���������

	Character.velx = Character.vely = SPEED;//�趨��ɫ���ж��ٶ�

	menu_pos.x = 0; menu_pos.y = 568;//�˵�����Ĭ��λ��

	game_state = STAGE_INIT;//��Ϸ�ĵ�ǰ״̬

	//���¾�Ϊ�������������
	starttime = (int)GetTickCount();
	startframe = 0;
	endframe = 19;
	direction = 1;
	delay = 30;
}

FirstStage::~FirstStage(){
	if (background) background->Release();
	delete[]button;
}

bool FirstStage::Stage_Init(HWND hwnd){

	LPDIRECT3DSURFACE9 image = NULL;
	//����Ҫ��ȡ��Ҫ�õ�ͼƬ(��ʱ�ñ��������һ������
	//background = LoadSurface("stage1_background.jpg");
	
	image = LoadSurface("stage1_background2.bmp");
	if (!image){
		MessageBox(hwnd, "Error Read image", "Error", MB_OK);
		return false;
	}
	D3DSURFACE_DESC desc;
	image->GetDesc(&desc);
	/*
	HRESULT result = d3ddev->CreateOffscreenPlainSurface(
		SCREENW,
		SCREENH,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&background, NULL);
	if (result != D3D_OK){
		MessageBox(NULL, "Error create surface", "Error", MB_OK);
		return NULL;
	}
	//load background
	LPDIRECT3DSURFACE9 image = NULL;
	image = LoadSurface("space2.bmp");
	if (!image) return false;
	*/

	//create background
	HRESULT result =
		d3ddev->CreateOffscreenPlainSurface(
		BUFFERW,
		BUFFERH,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&background,
		NULL);
	if (result != D3D_OK) return false;
	
	d3ddev->CreateOffscreenPlainSurface(
		BUFFERW,
		BUFFERH,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&background,
		NULL);

	RECT rect_source = { 0, 0, desc.Width, desc.Height };
	RECT dest_ul = { 0, 0, 1024, 768 };
	d3ddev->StretchRect(image, &rect_source, background, &dest_ul, D3DTEXF_NONE);

	RECT rect_ur = { SCREENW, 0, SCREENW * 2, SCREENH };
	d3ddev->StretchRect(image, &rect_source, background, &rect_ur, D3DTEXF_NONE);

	RECT rect_dl = { 0, SCREENH, SCREENW, SCREENH * 2};
	d3ddev->StretchRect(image, &rect_source, background, &rect_dl, D3DTEXF_NONE);

	RECT rect_dr = { SCREENW, SCREENH, SCREENW * 2, SCREENH * 2 };
	d3ddev->StretchRect(image, &rect_source, background, &rect_dr, D3DTEXF_NONE);

	image->Release();
	
	//Ȼ���ǰѾ�������Ҳ��ȡ����
	Character.image = LoadTexture("sprite.png");
	if (!Character.image){
		MessageBox(hwnd, "Error Read character.png", "Error", MB_OK);
		return false;
	}
	//����������һ������Ҫʹ�õİ�ť����״
	//���ϽǵĶ���Ϊ�˵������½Ǳ���Ϊ���죬����������
	button = new Button[2];

	//Ȼ��ѻ����Ĳ˵�������һ��
	/*
	if (!CreateMenu()){
		MessageBox(hwnd, "Error Create Mene", "Error", MB_OK);
		return false;
	}
	*/
	//���������·��ĵ����������Ҫ�úü���һ�£�
	staff = LoadTexture("staffbag.png");
	if (!staff){
		MessageBox(hwnd, "Error Read Staff", "Error", MB_OK);
		return false;
	}
	//Ȼ�����趨һ����Ҫ���ϰ��
	
	barrier = new Barrier[2];
	string temp_name;
	stringstream ss;
	ss << "barrier";
	for (int i = 0; i < numOfBarrier; i++){
		ss << i << ".png";
		temp_name = ss.str();
		barrier[i].setImage(temp_name);
		if (!barrier[i].image){
			MessageBox(hwnd, "Error Read Staff", "Error", MB_OK);
			return false;
		}
		ss.str("");
		ss << "barrier";
	}
	barrier[1].dialog = "����һ���ִβ�Ҫ����";

	//Ȼ���Ƕ�ȡ����ʰȡ����Ʒ��
	tool = new Stuff[1];
	ss.str("");
	ss << "stuff";
	for (int i = 0; i < numOfTool; i++){
		ss << i << ".png";
		temp_name = ss.str();
		tool[i].setImage(temp_name);
		if (!tool[i].image){
			MessageBox(hwnd, "Error Read Stuff", "Error", MB_OK);
			return false;
		}
		ss.str("");
		ss << "stuff";
	}
	//=====================================================================
	//һ���Ǹ������ݵĳ�ʼ��
	//��ɫ���ڵ�λ��
	Character.x = 500; Character.y = 390;
	//Ȼ���Ǹ����ϰ����λ�ã���ʱ��Ҫ̫�����
	barrier[0].x = 300; barrier[0].y = 400;
	barrier[1].x = 128; barrier[1].y = 80;

	//�����ȷ��һ�µ��ߵ�λ��(ע�⣬���߱�����Ҫȷ��һ�¸������ߵĻ�����Ϣ��
	stone.x = 200; stone.y = 200;
	tool[0].x = 500; tool[0].y = 600;
	//�趨һ�¸������ߵ����ʱ���˵ʲô

	delete[]tool[0].dialog;
	tool[0].dialog = new string[2];
	tool[0].dialog_num = 2;
	tool[0].dialog[0] = "���С����������˼���������û�ȥ����";
	tool[0].dialog[1] = "����ô���¡���̫���˲�������QVQ";
	//����˻��������ݵ����ú󣬿��Կ�ʼ��Ϸ����Ļ�����
	game_state = STAGE_RUNING;
	return true;
}

void FirstStage::Stage_Run(){
	static bool canMove = true;				//��������˾������ʲô��ģ����ʱ�������ƶ�
	//��Ϸ��������Ҫ�����豸�����ⲿ�����ˣ�
	//Ȼ������������ⲿ������Clear(������
	//��ʼ���в����ĸ��£�
	//��ǰΪ��Ϸ���еĻ��棬���Խ��и���
	if (game_state == STAGE_RUNING){
		//���ȣ�Ĭ���ƶ��ٶ�Ϊ0
		Character.velx = Character.vely = 0;
		if (Key_Down(DIK_DOWN)){
			if (canMove)
				Character.vely = SPEED;
			else
				Character.vely = 0;
		}
		else if (Key_Down(DIK_UP) ){
			if (canMove)
				Character.vely = -SPEED;
			else
				Character.vely = 0;
		}
		if (Key_Down(DIK_LEFT) ){
			if (canMove)
				Character.velx = -SPEED;
			else
				Character.velx = 0;
		}
		else if (Key_Down(DIK_RIGHT)){
			if (canMove)
				Character.velx = SPEED;
			else
				Character.velx = 0;
		}
		//-------------------------------------------------------
		//------                  ��ײ����                  ------
		//-------------------------------------------------------

		//  -*-  -*-  -*-   ���ͼ  -*-  -*-  -*-
		//-------
		//x�Ჿ��
		//-------
		//ע���ʱҪ�ж�һ���Ƿ�ײ���˱߿�
		if (Character.x < 0){
			//����ʱ���������Ҳ��X = 0��˵����ʱ�Ѿ������������
			if (X <= 0){
				Character.x = 0;
				stage_valx = 0;
			}
			else{
				//����ʱ��û�е��˵�������ͼ��߲��ֻ�û�б�̽�����������������ǰ��
				Character.x = 0;
				stage_valx = -SPEED;
			}
		}
		//����Ļ����ǲ���ײ�������ұߣ�����������ͬʱ������
		else if (Character.x + Character.width > SCREENW){
			//����ʱ���������Ҳ��X = 0��˵����ʱ�Ѿ��������ұ���
			if (X + SCREENW >= backgroundWidth){
				Character.x = SCREENW - Character.width;
				X = backgroundWidth - SCREENW;
				stage_valx = 0;
			}
			else{
				//����ʱ��û�е��˵�������ͼ��߲��ֻ�û�б�̽�����������������ǰ��
				Character.x = SCREENW - Character.width;
				stage_valx = SPEED;
			}
		}
		//��δ�����Ļ���˵����ʱ���������ﶼû��ײ��(��ʵ�󲿷�ʱ���������ģ�
		else{
			stage_valx = 0;
		}
		//----------
		//y�Ჿ��
		//----------

		if (Character.y < 0){
			//����ʱ���������Ҳ��X = 0��˵����ʱ�Ѿ��������ϱ���
			if (Y <= 0){
				Character.y = 0;
				stage_valy = 0;
			}
			else{
				//����ʱ��û�е��˵�������ͼ��߲��ֻ�û�б�̽�����������������ǰ��
				Character.y = 0;
				stage_valy = -SPEED;
			}
		}
		//���ߣ�ײ������Ļ�����·�
		else if (Character.y + Character.height > SCREENH){
			//����ʱ���������Ҳ��X = 0��˵����ʱ�Ѿ��������±���
			if (Y + SCREENH >= backgroundHeight){
				Character.y = SCREENH - Character.height;
				Y = backgroundHeight - SCREENH;
				stage_valy = 0;
			}
			else{
				//����ʱ��û�е��˵�������ͼ��߲��ֻ�û�б�̽�����������������ǰ��
				Character.y = SCREENH - Character.height;
				stage_valy = SPEED;
			}
		}
		//����Ļ����������ﶼûײ��
		else{
			stage_valy = 0;
		}

		//------------------------------------------------------
		//   -*- -*- -*- ��Ʒ����ײ����  -*- -*- -*-
		int temp_collision = 0;//Ĭ��δ������ײ
		for (int i = 0; i < numOfBarrier; i++){
			canMove = reserchHere(Character, barrier[i],canMove);
			/*
			//��ײ���ԣ��õ���ɫ���ĸ���Ʒ��������ײ
			temp_collision = CollisionD(Character, barrier[i]);
			if (temp_collision == LEFT_COLLISION)
				Character.velx = SPEED;
			else if (temp_collision == RIGHT_COLLISION)
				Character.velx = -SPEED;
			else if (temp_collision == UP_COLLISION)
				Character.vely = -SPEED;
			else if (temp_collision == BOTTOM_COLLISION)
				Character.vely = SPEED;

			//Ȼ���ж��Ƿ����е���(ע�⣬�����ʱ�Ѿ���ʾ�˶Ի��������ٴ���ʾ��
			float temp_destx = abs(Character.x +Character.width/2- (barrier[i].x + barrier[i].width/2));
			float temp_desty = abs(Character.y + Character.height/2- (barrier[i].y+ barrier[i].height / 2));

			//�ж��Ƿ��Ѿ����Ե���
			if (temp_destx < Character.width / 2 + barrier[i].width / 2 + 30 &&
				temp_desty < Character.height / 2 + barrier[i].height / 2 + 30)
				if (Key_Down(DIK_SPACE) && !barrier[i].dialog_show &&!barrier[i].dialogIsShowing){
					if (!spaceButton){
						barrier[i].dialog_show = true;
						spaceButton = true;
						canMove = false;
					}
				}
			//���ǣ�����Ի����Ѿ����Ƴ��������һ��ǰ����˰�ť�Ļ����Ͳ��ڻ���
				else if (barrier[i].dialog_show&&Key_Down(DIK_SPACE)&&barrier[i].dialogIsShowing){
					if (!spaceButton){
						barrier[i].dialog_show = false;
						spaceButton = true;
						barrier[i].dialogIsShowing = false;
						canMove = true;
					}
				}
				else
					spaceButton = false;
					*/
					
		}
		
		//��Ȼ������Ҳ�ǲ��ܿ�Խ��
		for (int i = 0; i < numOfTool; i++){
			canMove = reserchHere(Character, tool[0], canMove);
		}
		//һ��һ���׶����ɣ������Ȱ���Щ�����ĺ���ȷ������
		//------------------------------------------------------


		//���������ƶ����е���Ʒ��
		//---------����

		Character.x += Character.velx;
		Character.y += Character.vely;

		//---------����ͼƬ
		X += stage_valx;
		Y += stage_valy;

		//--------������Ʒ
		barrier[0].x -= stage_valx;
		barrier[0].y -= stage_valy;

		barrier[1].x -= stage_valx;
		barrier[1].y -= stage_valy;

		//�ڲ���������֮�����ǿ�ʼ������Ҫ�Ĳ���
	}
}

//======================================================
//===         ��ʼ����Ҫ���ƵĶ���������Ļ��
//======================================================
void FirstStage::Stage_Draw(){
 
	//------------------------------
	//--          ���Ʊ���
	//------------------------------
	
	RECT rect_source = { X, Y, SCREENW + X, SCREENH + Y };
	RECT rect_dest = { 0, 0, SCREENW, SCREENH };
	d3ddev->StretchRect(background, &rect_source, backbuffer, &rect_dest,D3DTEXF_NONE);
	//���ȼǵ�Ҫ��spriteobj
	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

	//Ȼ�����Ҫ�Ĳ��ֻ�����Ļ��
	//ĿǰΪֹ�Ƚ���ɫ���Ƶ���Ļ�Ϻ���
	//���飨������Ƚ��м��ɡ���Ϊ�˱�֤�ܹ���̬�Ĳ���������
	Draw_Sprite_Texture(Character.image, Character.x, Character.y, Character.framenum, 
		Character.width,  Character.height,8);
	Sprite_Animate(Character.framenum, starttime, 8, startframe, endframe, direction, delay);

	//Ȼ���ǵ���
	for (int i = 0; i < numOfBarrier; i++){

		//�����ǵ��߱���
		Draw_Sprite_Texture(barrier[i].image, barrier[i].x, barrier[i].y, barrier[i].framenum, barrier[i].width,
			barrier[i].height, barrier[i].columns);

		//Ȼ���ǵ��ߵ���ضԻ���
		if (barrier[i].dialog_show){
			barrier[i].ShowSelectedText();
			barrier[i].dialogIsShowing = true;
		}
	}

	//Ȼ������Ʒ
	for (int i = 0; i < numOfTool; i++){

		//��������Ʒ����
		Draw_Sprite_Texture(tool[i].image, tool[i].x, tool[i].y, tool[i].framenum, tool[i].width,
			tool[i].height, tool[i].columns);

		//Ȼ������Ʒ����ضԻ���
		if (tool[i].dialog_show){
		//����ע�⣬���ڻ��ж���Ի�����������������Ļ�����Ҫһ������������һ���Ƿ����
		//�����Ϊ������û�н����Ի����򷵻�ֵΪfalse�������˵Ļ�����true
			tool[i].dialogIsShowing = tool[i].ShowSelectedText();
		}
	}


	//���ﻹҪ���ǵ��˵�����Ľ������������ﻹ��Ҫ����һ�±仯�Ĺ��̵�
	//Ȼ����ǽ�������
	spriteobj->End();
	 
	//�����Ĺ��������ⲿ���ͺ���
}

//==========================================
//==         No.3  �ùؿ��Ľ���
//==========================================

//����ǻ�����������
bool FirstStage::Stage_End(){
	//��֮ǰ��������ͼƬȫ������
	if (Character.image)Character.image->Release();
	if (background)background->Release();
	if (barrier[0].image)barrier[0].image->Release();
	return true;
}
bool FirstStage::Stage_End_Draw(){

	static int alpha = 0;//һ������ִ�С�����䰵�ı�����
	static LPDIRECT3DSURFACE9 shade;//����������Ӱ
	HRESULT result = d3ddev->CreateOffscreenPlainSurface(
		SCREENW,                //width of the surface
		SCREENH,                //height of the surface
		D3DFMT_A8R8G8B8,    //surface format
		D3DPOOL_DEFAULT,    //memory pool to use
		&shade,           //pointer to the surface
		NULL);              //reserved (always NULL)

	static RECT rect = { 0, 0, SCREENW, SCREENH };
	if (alpha > 255)
		return true;
	d3ddev->ColorFill(shade, &rect, D3DCOLOR_ARGB(alpha++, 0, 0, 0));

	//����Ҫȷ��һ�£��ڲ˵���ѡ���ˡ��˳���������˵����������Ϸ������ʱ��Ҫ���йؿ��Ľ���
	if (game_state == STAGE_OVER){
	//��ǰ�ؿ�����������һ������ĺ�ɫ������ȥ�𽥸�ס��ǰ���棨����(Ӧ�ÿ������÷���ֵ��ȷ����ʱ�Ƿ��ܹ���ɽ�����Ϸ�Ļ���)
	//�߼�Ϊ����Ȼ���Game_Draw",�������״̬ΪGAME_OVER����ʱ�ѽ������������ȥ
		d3ddev->StretchRect(shade, &rect, backbuffer, NULL, D3DTEXF_NONE);
	}
	return false;
}

//No.4  ���ƶԻ���
void FirstStage::Dialog_Draw(){
	dialog[0] = "Now this is dialog in the game for test";
	font_arial = MakeFont("Arial", 45);
	PrintFont(font_arial, 50, 50, dialog[0]);
}

bool FirstStage::reserchHere(SPRITE& Character,SPRITE &object,bool canMove)
{

	/*
	���ȵ����ȣ������ʱ�Ѿ������˵����¼��Ļ�������ֵΪtrue��ʱ������Ӧ��ȥ����Ƿ����˿ո����ʹ�Ի�
	������������������ķ���ֵcanMoveӦ�������㺬�壨�������������ﻹҪע�⣬canMoveΪfalse��ʱ�򣬲�������
	�ж������Ҳ����ٴν��е����Լ�����Ϸ���ɣ�����������ͬʱֻ�����ⲿ�ĶԻ����¼������Ի��������ʱ�򣬽�camMove
	����Ϊtrue��ʹ��Ϸ��������
	*/
	//��ײ���ԣ��õ���ɫ���ĸ���Ʒ��������ײ
	int temp_collision;
	temp_collision = CollisionD(Character, object);
	if (temp_collision == LEFT_COLLISION)
		Character.velx = SPEED;
	else if (temp_collision == RIGHT_COLLISION)
		Character.velx = -SPEED;
	else if (temp_collision == UP_COLLISION)
		Character.vely = -SPEED;
	else if (temp_collision == BOTTOM_COLLISION)
		Character.vely = SPEED;

	//Ȼ���ж��Ƿ����е���(ע�⣬�����ʱ�Ѿ���ʾ�˶Ի��������ٴ���ʾ��
	float temp_destx = abs(Character.x + Character.width / 2 - (object.x + object.width / 2));
	float temp_desty = abs(Character.y + Character.height / 2 - (object.y + object.height / 2));

	//�ж��Ƿ��Ѿ����Ե���
	if (temp_destx < Character.width / 2 + object.width / 2 + 30 &&
		temp_desty < Character.height / 2 + object.height / 2 + 30){
		if (Key_Down(DIK_SPACE) && !object.dialog_show &&!object.dialogIsShowing){
			if (!spaceButton){
				//MessageBox(NULL, "SPACE!", "SPAXE", MB_OK);
				object.dialog_show = true;
				spaceButton = true;
				canMove = false;
			}
		}
		//���ǣ�����Ի����Ѿ����Ƴ��������һ��ǰ����˰�ť�Ļ����Ͳ��ڻ���
		else if (object.dialog_show&&Key_Down(DIK_SPACE) && object.dialogIsShowing){
			if (!spaceButton){
				object.dialog_show = false;
				spaceButton = true;
				object.dialogIsShowing = false;
				canMove = true;
			}
		}
		else
			spaceButton = false;
	}

	return canMove;
}
