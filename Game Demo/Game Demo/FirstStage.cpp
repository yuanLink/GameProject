#include"FirstStage.h"

FirstStage::FirstStage(){
	score = 0;//本关分数
	background = NULL;//背景
	backgroundWidth = SCREENW*2;  
	backgroundHeight = SCREENH*2;

	barrier = NULL;//该关卡上的其他元素（比如什么障碍物啊之类的）
	tool = NULL;//该关卡上的道具元素（也设定为空）

	numOfBarrier = 2;//用两个来测试一下先
	numOfTool = 1;//工具的数量

	string_index = 0;//该关卡上的第几段对话

	dialog = new string[1];//对话框只有一个
	font_arial = NULL;//字体
	spaceButton = false;

	button = NULL;//按钮的个数
	
	staff = NULL;//道具栏

	X = Y = 0;//设定大地图的相对坐标

	Character.velx = Character.vely = SPEED;//设定角色的行动速度

	menu_pos.x = 0; menu_pos.y = 568;//菜单栏的默认位置

	game_state = STAGE_INIT;//游戏的当前状态

	//以下均为动画的相关设置
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
	//首先要读取需要用的图片(暂时拿别的来代替一下啦）
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
	
	//然后是把精灵主角也读取进来
	Character.image = LoadTexture("sprite.png");
	if (!Character.image){
		MessageBox(hwnd, "Error Read character.png", "Error", MB_OK);
		return false;
	}
	//接下来创建一波打算要使用的按钮的形状
	//右上角的定义为菜单，右下角本意为音响，现在随意了
	button = new Button[2];

	//然后把基本的菜单栏创建一下
	/*
	if (!CreateMenu()){
		MessageBox(hwnd, "Error Create Mene", "Error", MB_OK);
		return false;
	}
	*/
	//接下来是下方的道具栏（这个要好好计算一下）
	staff = LoadTexture("staffbag.png");
	if (!staff){
		MessageBox(hwnd, "Error Read Staff", "Error", MB_OK);
		return false;
	}
	//然后是设定一下主要的障碍物：
	
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
	barrier[1].dialog = "这是一个胖次不要看啊";

	//然后是读取可以拾取的物品：
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
	//一下是各类数据的初始化
	//角色所在的位置
	Character.x = 500; Character.y = 390;
	//然后是各个障碍物的位置（暂时不要太多个）
	barrier[0].x = 300; barrier[0].y = 400;
	barrier[1].x = 128; barrier[1].y = 80;

	//最后是确定一下道具的位置(注意，道具本身需要确定一下各个道具的基本信息）
	stone.x = 200; stone.y = 200;
	tool[0].x = 500; tool[0].y = 600;
	//设定一下各个道具调查的时候会说什么

	delete[]tool[0].dialog;
	tool[0].dialog = new string[2];
	tool[0].dialog_num = 2;
	tool[0].dialog[0] = "这个小东西好有意思啊，不如拿回去玩玩";
	tool[0].dialog[1] = "诶怎么回事……太重了不想拿了QVQ";
	//完成了基本的数据的设置后，可以开始游戏本身的绘制了
	game_state = STAGE_RUNING;
	return true;
}

void FirstStage::Stage_Run(){
	static bool canMove = true;				//加入进入了剧情或者什么别的，则此时不允许移动
	//游戏运行首先要更新设备（在外部处理了）
	//然后这里假设在外部进行了Clear(清理）、
	//开始进行操作的更新：
	//当前为游戏运行的画面，所以进行更新
	if (game_state == STAGE_RUNING){
		//首先，默认移动速度为0
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
		//------                  碰撞测试                  ------
		//-------------------------------------------------------

		//  -*-  -*-  -*-   大地图  -*-  -*-  -*-
		//-------
		//x轴部分
		//-------
		//注意此时要判断一下是否撞到了边框，
		if (Character.x < 0){
			//若此时的相对坐标也是X = 0，说明此时已经到达最左边了
			if (X <= 0){
				Character.x = 0;
				stage_valx = 0;
			}
			else{
				//若此时还没有到达，说明这个地图左边部分还没有被探索，所以允许继续的前进
				Character.x = 0;
				stage_valx = -SPEED;
			}
		}
		//否则的话，是不是撞到了最右边（反正不可能同时发生）
		else if (Character.x + Character.width > SCREENW){
			//若此时的相对坐标也是X = 0，说明此时已经到达最右边了
			if (X + SCREENW >= backgroundWidth){
				Character.x = SCREENW - Character.width;
				X = backgroundWidth - SCREENW;
				stage_valx = 0;
			}
			else{
				//若此时还没有到达，说明这个地图左边部分还没有被探索，所以允许继续的前进
				Character.x = SCREENW - Character.width;
				stage_valx = SPEED;
			}
		}
		//都未发生的话，说明此时横向上哪里都没有撞到(其实大部分时候都是这样的）
		else{
			stage_valx = 0;
		}
		//----------
		//y轴部分
		//----------

		if (Character.y < 0){
			//若此时的相对坐标也是X = 0，说明此时已经到达最上边了
			if (Y <= 0){
				Character.y = 0;
				stage_valy = 0;
			}
			else{
				//若此时还没有到达，说明这个地图左边部分还没有被探索，所以允许继续的前进
				Character.y = 0;
				stage_valy = -SPEED;
			}
		}
		//或者，撞到了屏幕的最下方
		else if (Character.y + Character.height > SCREENH){
			//若此时的相对坐标也是X = 0，说明此时已经到达最下边了
			if (Y + SCREENH >= backgroundHeight){
				Character.y = SCREENH - Character.height;
				Y = backgroundHeight - SCREENH;
				stage_valy = 0;
			}
			else{
				//若此时还没有到达，说明这个地图左边部分还没有被探索，所以允许继续的前进
				Character.y = SCREENH - Character.height;
				stage_valy = SPEED;
			}
		}
		//否则的话，就是哪里都没撞到
		else{
			stage_valy = 0;
		}

		//------------------------------------------------------
		//   -*- -*- -*- 物品的碰撞测试  -*- -*- -*-
		int temp_collision = 0;//默认未发生碰撞
		for (int i = 0; i < numOfBarrier; i++){
			canMove = reserchHere(Character, barrier[i],canMove);
			/*
			//碰撞测试，得到角色与哪个物品发生了碰撞
			temp_collision = CollisionD(Character, barrier[i]);
			if (temp_collision == LEFT_COLLISION)
				Character.velx = SPEED;
			else if (temp_collision == RIGHT_COLLISION)
				Character.velx = -SPEED;
			else if (temp_collision == UP_COLLISION)
				Character.vely = -SPEED;
			else if (temp_collision == BOTTOM_COLLISION)
				Character.vely = SPEED;

			//然后，判断是否会进行调查(注意，如果此时已经显示了对话框，则不能再次显示）
			float temp_destx = abs(Character.x +Character.width/2- (barrier[i].x + barrier[i].width/2));
			float temp_desty = abs(Character.y + Character.height/2- (barrier[i].y+ barrier[i].height / 2));

			//判断是否已经可以调查
			if (temp_destx < Character.width / 2 + barrier[i].width / 2 + 30 &&
				temp_desty < Character.height / 2 + barrier[i].height / 2 + 30)
				if (Key_Down(DIK_SPACE) && !barrier[i].dialog_show &&!barrier[i].dialogIsShowing){
					if (!spaceButton){
						barrier[i].dialog_show = true;
						spaceButton = true;
						canMove = false;
					}
				}
			//但是，如果对话框已经绘制出来，并且还是按下了按钮的话，就不在绘制
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
		
		//当然，道具也是不能跨越的
		for (int i = 0; i < numOfTool; i++){
			canMove = reserchHere(Character, tool[0], canMove);
		}
		//一个一个阶段来吧，现在先把这些基本的函数确定了先
		//------------------------------------------------------


		//接下来是移动所有的物品：
		//---------精灵

		Character.x += Character.velx;
		Character.y += Character.vely;

		//---------背景图片
		X += stage_valx;
		Y += stage_valy;

		//--------各类物品
		barrier[0].x -= stage_valx;
		barrier[0].y -= stage_valy;

		barrier[1].x -= stage_valx;
		barrier[1].y -= stage_valy;

		//在操作定下来之后，我们开始绘制需要的部分
	}
}

//======================================================
//===         开始把需要绘制的东西画到屏幕上
//======================================================
void FirstStage::Stage_Draw(){
 
	//------------------------------
	//--          绘制背景
	//------------------------------
	
	RECT rect_source = { X, Y, SCREENW + X, SCREENH + Y };
	RECT rect_dest = { 0, 0, SCREENW, SCREENH };
	d3ddev->StretchRect(background, &rect_source, backbuffer, &rect_dest,D3DTEXF_NONE);
	//首先记得要打开spriteobj
	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

	//然后把需要的部分画到屏幕上
	//目前为止先将角色绘制到屏幕上好了
	//精灵（这个还比较有技巧……为了保证能够动态的播出……）
	Draw_Sprite_Texture(Character.image, Character.x, Character.y, Character.framenum, 
		Character.width,  Character.height,8);
	Sprite_Animate(Character.framenum, starttime, 8, startframe, endframe, direction, delay);

	//然后是道具
	for (int i = 0; i < numOfBarrier; i++){

		//首先是道具本身
		Draw_Sprite_Texture(barrier[i].image, barrier[i].x, barrier[i].y, barrier[i].framenum, barrier[i].width,
			barrier[i].height, barrier[i].columns);

		//然后是道具的相关对话框
		if (barrier[i].dialog_show){
			barrier[i].ShowSelectedText();
			barrier[i].dialogIsShowing = true;
		}
	}

	//然后是物品
	for (int i = 0; i < numOfTool; i++){

		//首先是物品本身
		Draw_Sprite_Texture(tool[i].image, tool[i].x, tool[i].y, tool[i].framenum, tool[i].width,
			tool[i].height, tool[i].columns);

		//然后是物品的相关对话框
		if (tool[i].dialog_show){
		//这里注意，由于会有多个对话框的情况，所以这里的绘制需要一个变量来控制一下是否结束
		//这里记为，若还没有结束对话，则返回值为false，结束了的话返回true
			tool[i].dialogIsShowing = tool[i].ShowSelectedText();
		}
	}


	//这里还要考虑到菜单画面的建立，所以这里还是要考虑一下变化的过程的
	//然后就是结束画面
	spriteobj->End();
	 
	//其他的工作交给外部做就好了
}

//==========================================
//==         No.3  该关卡的结束
//==========================================

//这个是画面结束后调用
bool FirstStage::Stage_End(){
	//把之前读进来的图片全部销毁
	if (Character.image)Character.image->Release();
	if (background)background->Release();
	if (barrier[0].image)barrier[0].image->Release();
	return true;
}
bool FirstStage::Stage_End_Draw(){

	static int alpha = 0;//一个用来执行“画面变暗的变量”
	static LPDIRECT3DSURFACE9 shade;//用来绘制阴影
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

	//首先要确定一下，在菜单栏选择了“退出”，或者说是正常的游戏结束的时候，要进行关卡的结束
	if (game_state == STAGE_OVER){
	//当前关卡结束（绘制一个超大的黑色画面上去逐渐盖住当前画面（？）(应该可以利用返回值来确定此时是否能够完成结束游戏的绘制)
	//逻辑为“依然完成Game_Draw",但是如果状态为GAME_OVER，此时把结束画面绘制上去
		d3ddev->StretchRect(shade, &rect, backbuffer, NULL, D3DTEXF_NONE);
	}
	return false;
}

//No.4  绘制对话框
void FirstStage::Dialog_Draw(){
	dialog[0] = "Now this is dialog in the game for test";
	font_arial = MakeFont("Arial", 45);
	PrintFont(font_arial, 50, 50, dialog[0]);
}

bool FirstStage::reserchHere(SPRITE& Character,SPRITE &object,bool canMove)
{

	/*
	首先的首先，如果此时已经发生了调查事件的话（返回值为true的时候，我们应该去检测是否按下了空格键来使对话
	继续发生，所以这里的返回值canMove应该有两层含义（所以我们在这里还要注意，canMove为false的时候，不但不能
	行动，而且不用再次进行调查以减少游戏负荷（？），并且同时只进行外部的对话框事件，当对话框结束的时候，将camMove
	调整为true，使游戏继续进行
	*/
	//碰撞测试，得到角色与哪个物品发生了碰撞
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

	//然后，判断是否会进行调查(注意，如果此时已经显示了对话框，则不能再次显示）
	float temp_destx = abs(Character.x + Character.width / 2 - (object.x + object.width / 2));
	float temp_desty = abs(Character.y + Character.height / 2 - (object.y + object.height / 2));

	//判断是否已经可以调查
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
		//但是，如果对话框已经绘制出来，并且还是按下了按钮的话，就不在绘制
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
