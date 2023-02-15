#include "stdafx.h"

HP::HP()
{
	col = new ObRect();
	col->scale = Vector2(30, 30);
	col->visible =  false;
	col->space = SPACE::SCREEN;

	halfHeart = new ObImage(L"Half_Heart.png");
	halfHeart->scale = Vector2(30, 30);
	halfHeart->SetParentRT(*col);
	halfHeart->space = SPACE::SCREEN;
	heart =  new ObImage(L"Heart.png");
	heart->scale = Vector2(30, 30);
	heart->SetParentRT(*col);
	heart->space = SPACE::SCREEN;
}
HP::~HP()
{
	SafeDelete(col);
	SafeDelete(halfHeart);
	SafeDelete(heart);
	
}
void HP::full()
{
	col->visible = false;
	halfHeart->visible = false;
	heart->visible = true;
}
void HP::half()
{
	col->visible = false;
	halfHeart->visible = true;
	heart->visible = false;
}
void HP::none()
{
	col->visible = false;
	halfHeart->visible = false;
	heart->visible = false;
}
void HP::Update()
{
	col->Update();
	halfHeart->Update();
	heart->Update();
}
void HP::Render()
{
	col->Render();
	halfHeart->Render();
	heart->Render();
}
//////////////////////////////////////////////////
Player::Player()
{
	foot.resize(4);
	col = new ObRect();
	col->isFilled = false;
	col->pivot = OFFSET_N;
	col->scale = Vector2(70.0f, 85.0f);
	
	idle = new ObImage(L"playerIdle.png");
	idle->scale = Vector2(96.0f, 96.0f);
	idle->maxFrame = Int2(6,7);
	idle->SetParentRT(*col);

	walk = new ObImage(L"playerWalk.png");
	walk->scale = Vector2(96.0f, 96.0f);
	walk->maxFrame = Int2(6, 7);
	walk->SetParentRT(*col);

	roll = new ObImage(L"playerRoll.png");
	roll->scale = Vector2(96.0f, 96.0f);
	roll->maxFrame = Int2(9, 7);
	roll->visible = false;
	roll->SetParentRT(*col);

	hand = new ObImage(L"playerHand.png");
	hand->scale = Vector2(30, 30);
	hand->visible = true;
	
	die = new ObImage(L"playerDie.png");
	die->scale = Vector2(22, 26)*4.0f;
	die->maxFrame = Int2(10, 1);
	die->visible = false;
	die->SetParentRT(*col);

	reloadGage = new ObRect();
	reloadGage->scale.x = 0;
	reloadGage->scale.y = 10.0f;
	reloadGage->pivot = OFFSET_L;
	reloadGage->visible = false;
	reloadGage->SetParentRT(*col); 
	reloadGage->SetLocalPosX(-30.0f);
	reloadGage->SetLocalPosY(40.0f);

	gun[0] = new Magnum();
	gun[1] = new Rifle();

	gunidx = 0;

	playerHp.resize(3);
	hp = 6;

	// B 앞 T 뒤 
	frameY[Dir_R] = 2;
	frameY[Dir_T] = 4;
	frameY[Dir_L] = 3;
	frameY[Dir_B] = 0;
	frameY[Dir_RB] = 2;
	frameY[Dir_LB] = 3;
	frameY[Dir_LT] = 6;
	frameY[Dir_RT] = 5;

	//1번 총으로 초기화
	inven = Inven::inven1;

	//Blank관련
	blankRange = new ObCircle();
	blankActive = false;

	//1개의 Blank 넣어놓기
	blank = new Blank();
	SetItem(blank);

	playerState = PlayerState::IDLE;
	walk->visible = false;
	roll->visible = false;
	idle->visible = true;

	

}
Player::~Player()
{
	SafeDelete(col);
	for (int i = 0; i < playerHp.size(); i++)
	{
		SafeDelete(playerHp[i]);
	}
	SafeDelete(idle);
	SafeDelete(walk);
	SafeDelete(roll);
	SafeDelete(hand);
	SafeDelete(gun[0]);
	SafeDelete(gun[1]);
	SafeDelete(blank);
	SafeDelete(blankRange);

}

void Player::SettingHp(int hp)
{
	//hp : 6 -> size : 3
	//hp : 7 -> size : 4
	//cout << "체력 : " << hp << endl;
	int temp;
	if (hp % 2 != 0)temp = hp + 1;
	else temp = hp;

	playerHp.resize(temp / 2);
	//hp  :  6   -> playerHp  :  3
	for (int i = 0; i < playerHp.size(); i++)
	{
		playerHp[i] = new HP();
		if (i == 0)
		{
			playerHp[i]->col->SetWorldPos(Vector2(-App.GetHalfWidth()+20,App.GetHalfHeight()-30));
		}
		if (i > 0)
		{
			playerHp[i]->col->SetParentRT(*playerHp[0]->col);
			playerHp[i]->col->SetLocalPosX(i* col->scale.x/2);
		}
	}

	
	//hp  :  6   -> playerHp  :  3
	//hp  :  7   -> playerHp  :  4 / 0~2까지 full 3에서 half
	//hp  :  3   -> PlayerHp  :  2 / 0까지 full  1에서 half 
	if (hp % 2 == 0) //체력이 짝수 일때,
	{
		for (int i = 0; i <playerHp.size(); i++)
		{
			playerHp[i]->full();
		}
	}
	else             //체력이 홀수 일때,
	{
		int size = playerHp.size()-1;
		for (int i = 0; i < size; i++)
		{
			playerHp[i]->full();
		}
		playerHp[size]->half();
	}

	

}

void Player::SetItem(Blank* item)
{
	itemInven.push_back(item);
	item->IsObtain = true;
	if (!itemInven.empty())
	{
		Vector2 pos;
		pos.x = 0 + item->GetScale().x * itemInven.size();
		pos.y = itemInven[0]->GetPos().y;
	}
	else
	{
		item->SetPos(Vector2(0,0));
		//item->SetPos(Vector2(-App.GetHalfWidth()+20, App.GetHalfHeight()-50));
	}
}

void Player::Update()
{
	Vector2 dir = target - GetPos();
	target = INPUT->GetMouseWorldPos();

	LookDir(dir, idle);

	lastPos = col->GetWorldPos();
	//손 조작
	HandPos(Vector2(0.4, 0.3), target);



	//총 조작
	for (int i = 0; i < 2; i++)
	{
		gun[i]->col->SetParentRT(*hand);
	}
	gun[gunidx]->TargetDir(dir, gun[gunidx]->img, gun[gunidx]->gunstate);
	GunChange();

	
	if(isDying) playerState = PlayerState::DIED;

	//HP조작	
	SettingHp(hp);
	if (isDamaging)
	{
		if (GetHp() <= 0)
		{
			playerState = PlayerState::DIED;
		}
	}

	//장전 모션
	GageRoad();

	switch (playerState)
	{
	case PlayerState::IDLE:
		Idle();
		break;
	case PlayerState::WALK:
		Walk();
		break;
	case PlayerState::ROLL:
		Roll();
		break;
	case PlayerState::DIED:
		Died();
		break;
	}


	col->Update();
	idle->Update();
	walk->Update();
	roll->Update();
	hand->Update();
	die->Update();
	reloadGage->Update();
	gun[0]->Update();
	gun[1]->Update();

	for (int i = 0; i < playerHp.size(); i++)
	{
		playerHp[i]->Update();
	}
}
void Player::Render()
{
	if (playerState != PlayerState::DIED)
	{
		if (isBack())
		{
			gun[gunidx]->Render();
			col->Render();
			idle->Render();
			walk->Render();
			roll->Render();
			hand->Render();
		}
		else
		{
			col->Render();
			idle->Render();
			walk->Render();
			roll->Render();
			gun[gunidx]->Render();
			hand->Render();
		}
	}
	else
	{
		die->Render();
	}
	reloadGage->Render();
	for (int i = 0; i < playerHp.size(); i++)
	{

		playerHp[i]->Render();
	}
}
void Player::Stepback()
{
	col->SetWorldPos(lastPos);
	col->Update();
	idle->Update();
	walk->Update();
	roll->Update();
}
void Player::Input()
{
	moveDir = Vector2(0.0f, 0.0f);
	if (INPUT->KeyPress('A'))
	{
		moveDir.x = -1.0f;
	}
	else if (INPUT->KeyPress('D'))
	{
		moveDir.x = 1.0f;
	}
	if (INPUT->KeyPress('W'))
	{
		moveDir.y = 1.0f;
	}
	else if (INPUT->KeyPress('S'))
	{
		moveDir.y = -1.0f;
	}
	moveDir.Normalize();

	
	//재장전
	if (INPUT->KeyDown('R'))
	{
		Reloading = true;
		//gun[gunidx]->Reload();
		//reloadGage->scale.x = 0;
		
	}
	//총 쏘기
	if (INPUT->KeyPress(VK_LBUTTON))
	{
		gun[gunidx]->button = true;
		gun[gunidx]->Shot();
	}
	if (INPUT->KeyUp(VK_LBUTTON))
	{
		gun[gunidx]->button = false;
		gun[gunidx]->Shot();
	}
	if (INPUT->KeyPress(VK_SPACE))
	{
		if (!itemInven.empty())
		{
			cout << "Size : " << itemInven.size() << endl;
			Blanking();
		}
	}
	if (INPUT->KeyUp(VK_SPACE))
	{
		itemInven.pop_back();
	}

	/*if (INPUT->KeyDown('2'))
	{
		gunidx = 1;
		gun[gunidx]->active = true;
	}
	if (INPUT->KeyDown('1'))
	{
		gunidx = 0;
		gun[gunidx]->active = true;
	}*/
	
}
void Player::GunChange()
{
	if (inven == Inven::inven1)
	{
		HandPos(Vector2(0.4, 0.3),target);
		gun[0]->active = true;
		gun[1]->active = false;

		if (INPUT->KeyDown('2'))
		{
			inven = Inven::inven2;
			gunidx = 1;
		}
		
	}
	else if (inven == Inven::inven2)
	{
		HandPos(Vector2(1, 0.3), target);
		gun[0]->active = false;
		gun[1]->active = true;
		if (INPUT->KeyDown('1'))
		{
			inven = Inven::inven1;
			gunidx = 0;
		}
	}
}
void Player::GageRoad()
{
	if (Reloading)
	{
		reloadGage->visible = true;
		reloadGage->scale.x += 30.0*DELTA;
		if (reloadGage->scale.x > col->scale.x-10.0f)
		{
			gun[gunidx]->Reload();
		    reloadGage->scale.x = 0;
			Reloading = false;
			reloadGage->visible = false;

		}
	}
}

void Player::Blanking()
{
	//아이템 인벤에 아이템이 있으면, 사용가능
	if (!itemInven.empty())
	{

	}

}

void Player::Idle()
{
	Input();
	Vector2 Dir = INPUT->GetMouseWorldPos() - col->GetWorldPos();
	LookDir(Dir, walk);
	//idle to walk
	idle->visible = true;
	if (moveDir != Vector2(0.0f, 0.0f))
	{
		playerState = PlayerState::WALK;
		walk->visible = true;
		idle->visible = false;
		walk->ChangeAnim(ANISTATE::LOOP, 0.1f);
	}
}

void Player::Walk()
{
	Input();

	col->MoveWorldPos(moveDir * 150.0f * DELTA);

	Vector2 Dir = INPUT->GetMouseWorldPos() - col->GetWorldPos();
	LookDir(Dir, walk);
	//walk -> idle

	if (moveDir == Vector2(0.0f, 0.0f))
	{
		playerState = PlayerState::IDLE;
		walk->visible = false;
		idle->visible = true;
		idle->ChangeAnim(ANISTATE::LOOP, 0.2f);
	}
	//walk -> roll
	else if (INPUT->KeyPress(VK_RBUTTON))
	{
		playerState = PlayerState::ROLL;
		hand->visible = false;
		roll->visible = true;
		walk->visible = false;
		gun[gunidx]->img->visible = false;
		roll->ChangeAnim(ANISTATE::ONCE, 0.1f);
		rollTime = 0.0f;
	}
}

void Player::Roll()
{
	rollTime += DELTA;
	col->MoveWorldPos(moveDir * 500.0f * 
		//     0 ~ 1    * 90  - 45
		cosf(rollTime /0.9f * PI_DIV2 - PI_DIV4) * DELTA);
	LookDir(moveDir, roll);
	collable = false;
	hand->visible = false;
	//roll -> idle
	if (roll->frame.x >7)
	{
		playerState = PlayerState::IDLE;
		roll->visible = false;
		idle->visible = true;
		hand->visible = true;


		gun[gunidx]->img->visible = true;

		collable = true;

		idle->ChangeAnim(ANISTATE::LOOP, 0.2);
	}
}

void Player::Died()
{
	die->visible = true;
	die->ChangeAnim(ANISTATE::LOOP, 0.1f);
	col->MoveWorldPos(10*Vector2(1,0)*DELTA);

	if (die->frame.x > 8)
	{
		die->frame.x = 9;
	}

}


