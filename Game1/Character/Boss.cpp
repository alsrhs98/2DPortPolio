#include "stdafx.h"

Boss::Boss()
{
}

Boss::Boss(int num)
{
	frameY[Dir_R] = 0;
	frameY[Dir_L] = 1;
	frameY[Dir_RB] = 0;
	frameY[Dir_LT] = 3;
	frameY[Dir_T] = 3;
	frameY[Dir_B] = 0;
	frameY[Dir_LB] = 1;
	frameY[Dir_RT] = 2;
	//여기다 정의
	if (num == 1)
	{
		col = new ObRect();
		col->scale = Vector2(90, 120);
		col->visible = true;
		col->isFilled = false;

		hand = new ObImage(L"enemyHand.png");
		hand->scale = Vector2(30, 30);

		intro = new ObImage(L"enemyIntro.png");
		intro->scale = Vector2(39, 40) * 3.0f;
		intro->SetParentRT(*col);
		intro->maxFrame.x = 17;
		intro->maxFrame.y = 1;

		nextIntro = new ObImage(L"boss1Intro.png");
		nextIntro->scale = Vector2(83, 87) * 4.0f;
		nextIntro->SetParentRT(*col);
		nextIntro->maxFrame.x = 10;
		nextIntro->maxFrame.y = 1;

		idle = new ObImage(L"boss1Idle.png");
		idle->scale = Vector2(30, 40) * 4.0f;
		idle->SetParentRT(*col);
		idle->maxFrame = Int2(4, 4);
		
		
		walk =  new ObImage(L"boss1Walk.png");
		walk->scale = Vector2(30,168/4) *4.0f;
		walk->SetParentRT(*col);
		walk->maxFrame = Int2(6,4);

		dash =  new ObImage(L"boss1Dash.png");
		dash->scale = Vector2(30,152/4) *4.0f;
		dash->SetParentRT(*col);
		dash->maxFrame = Int2(4, 4);

		jump =  new ObImage(L"boss1Jump.png");
		jump->scale = Vector2(30, 41) * 4.0f;
		jump->SetParentRT(*col);
		jump->maxFrame = Int2(8, 1);

		die = new ObImage(L"boss1Die.png");
		die->scale = Vector2(40, 32) * 3.0f;
		die->SetParentRT(*col);
		die->maxFrame = Int2(8, 1);

	}
	else if (num == 2)
	{
		
	}

	egun = new eGun();
	
	hp = 20;

	hpGage = new ObRect();
	hpGage->pivot = OFFSET_L;
	hpGage->scale.x = App.GetHalfWidth()/20 * hp;
	hpGage->scale.y = 15;
	hpGage->space = SPACE::SCREEN;
	Vector2 pos = Vector2(-App.GetHalfWidth()*0.5, -App.GetHalfHeight()+10.0f);
	hpGage->SetWorldPos(pos);
	hpGage->color = Color(1,0,0,1);
	hpGage->visible = true;

	isDying = true;


	for (int i = 0; i < 8; i++)
	{
		float rad_step = PI / 4;
		targetPoint[i] = new ObCircle;
		targetPoint[i]->visible = true;
		targetPoint[i]->SetParentRT(*col);
		if (i < 4)
		{
			targetPoint[i]->SetLocalPos(10*Vector2(cosf(rad_step * i ), sinf(rad_step * i )));
		}
		else
		{
			float theta = rad_step *(i % 4);
			targetPoint[i]->SetLocalPos(10*Vector2(cosf(-PI + theta), sinf(-PI + theta)));
		}

		jumpShotBull[i] = new Bullet(0);
		jumpShotBull[i]->col->scale = Vector2(60, 60);
		jumpShotBull[i]->SetBullet(1,400);
	}

	startTime = 1.5f;

	state = BossState::INTRO;
	intro->visible = true;
	nextIntro->visible = false; 
	idle ->visible = false;
	walk ->visible = false;
	dash ->visible = false;
	jump ->visible = false;
	die  ->visible = false;
	hand ->visible = false;
	egun ->active = false;

}


Boss::~Boss()
{
	SafeDelete(egun);
	SafeDelete(intro);
	SafeDelete(nextIntro);
	SafeDelete(idle);
	SafeDelete(walk);
	SafeDelete(dash);
	SafeDelete(jump);
	SafeDelete(die );
	SafeDelete(hpGage);
	for (int i = 0; i < 8; i++)
	{
		SafeDelete(targetPoint[i]);
	}

}


//공격 패턴 3가지 //
void Boss::TrippleShot()
{
	
}

void Boss::JumpShot(Vector2 firepos)
{
	for (int i = 0; i < 8; i++)
	{
		// 총알  발사 위치 받아오기
		jumpShotBull[i]->col->SetWorldPos(firepos);
		
		// 발사 방향 정하기
		Vector2 dir = targetPoint[i]->GetWorldPos() - firepos;
		dir.Normalize();
		jumpShotBull[i]->dir = dir;
		
		//발사
		jumpShotBull[i]->Shot();
	}
}

void Boss::RotateShot(float theta)
{
	//rotateBull 초기화
	for (int i = 0; i < 8; i++)
	{
		rotateBull[i] = new Bullet(0);
		rotateBull[i]->col->scale = Vector2(20, 20);
		rotateBull[i]->firepos = egun->firePos->GetWorldPos();
	}

	float rad_step = PI/6* theta;

	//탄환 각도 계산
	float rad;
	
	
	rad  = -8 / 2 * rad_step;
	
	

	for (int i = 0; i < 8; i++ , rad+= rad_step)
	{
		float c = cosf(rad), s = sinf(rad);
		Vector2 dir = Vector2(c,s);
		rotateBull[i]->dir = dir;
		rotateBull[i]->SetBullet(1,400.0f );
		rotateBull[i]->Shot();
	}
}
////////////////////////
void Boss::Intro()
{

	intro->visible = true;
	hand->visible = false;
	intro->ChangeAnim(ANISTATE::LOOP, 0.1f);
	
	if (intro->frame.x >= 14)
	{
		intro->visible = false;
		state = BossState::INTRO2;
	}
	
}

void Boss::Intro2()
{
	nextIntro->visible = true;
	nextIntro->ChangeAnim(ANISTATE::LOOP, 0.1f);
	if (nextIntro->frame.x >= 8)
	{
		idle->visible = true;
		hand->visible = true;
		egun->active = true;
		nextIntro->visible = false;
		idle->ChangeAnim(ANISTATE::LOOP, 0.3f);
		canMove = true;
		state = BossState::IDLE;
	}
}

void Boss::Idle()
{
	if (hp <= 0)return;
	Vector2 dir = target - GetPos();
	LookDir(dir, idle);
	idle->ChangeAnim(ANISTATE::LOOP, 0.2f);
	idle->visible = true;
	walk->visible = false;
	dash->visible = false;
	jump->visible = false;
}

void Boss::Walk()
{
	if (hp <= 0 )return;
	idle->visible = false;
	walk->visible = true;
	dash->visible = false;
	jump->visible = false;

	if (canMove)
	{
		walk->ChangeAnim(ANISTATE::LOOP, 0.2f);
		Vector2 dir = target - GetPos();
		LookDir(dir, walk);
		dir.Normalize();
		col->MoveWorldPos(dir * 70.0f * DELTA);
	}
	

}

void Boss::Dash()
{
	if (hp <= 0)return;

	idle->visible = false;
	walk->visible = false;
	dash->visible = true;
	jump->visible = false;


	if (canMove)
	{
		dash->ChangeAnim(ANISTATE::LOOP, 0.2f);
		Vector2 dir = target - GetPos();
		LookDir(moveDir, dash);
		dir.Normalize();
		col->MoveWorldPos(dir * 100.0f * DELTA);

		if (dash->frame.x > 2)
		{
			state = BossState::IDLE;
		}
	}
}

void Boss::Jump()
{
	if (hp <= 0 or isJump )return;

	idle->visible = false;
	walk->visible = false;
	dash->visible = false;
	jump->visible = true;

	if (canMove)
	{
		jump->ChangeAnim(ANISTATE::LOOP, 0.1);

		Vector2 dir = target - GetPos();
		dir.Normalize();

		col->MoveWorldPos(dir * (100.0f+20*cosf(jumpTime*ToRadian)*DELTA) * DELTA);
		
		Vector2 temp = jump->scale;
		jump->scale.x += 20*cosf(jump->frame.x * ToRadian) * DELTA;
		jump->scale.y += 20*cosf(jump->frame.x * ToRadian) * DELTA;

		if (jump->frame.x > 6)
		{
			JumpShot(col->GetWorldPos());
			jump->visible = false;
			jump->frame.x = 0;
			jump->scale = temp;
			isJump = true;
			JCoolTime = 5.0f;
			state = BossState::IDLE;
		}
	}
}

void Boss::Attk()
{
	if (hp <= 0)return;
	walk->visible = false;
	idle->visible = true;
	idle->ChangeAnim(ANISTATE::LOOP, 0.2);
	state = BossState::ATTK;

	egun->button = false;
	if (TIMER->GetTick(attkTime, 0.2f))
	{
		//cout << "공격중" << endl;
		egun->button = true;
		egun->Shot2(target);
	}
}

void Boss::Died()
{
	die->visible = true;
	die->ChangeAnim(ANISTATE::LOOP, 0.3f);
	if (die->frame.x >= 4)
	{
		isDying = true;
		collable = true;
		intro->visible = false;
		idle->visible = false;
		walk->visible = false;
		die->frame.x = 5;
	}
	/*if (hp > 0)
	{
		isDying = false;
		collable = true;
		die->visible = false;
		intro->visible = true;
		intro->ChangeAnim(ANISTATE::LOOP, 0.1);
		state = BossState::INTRO;
	}*/
}

void Boss::Update()
{
	Vector2 dir = target - GetPos();
	//손
	HandPos(Vector2(1.5, 0.3), target);
	egun->col->SetParentRT(*hand);
	egun->TargetDir(dir, egun->img, egun->gunstate);

	//스킬 쿨타임 관리
	if (isJump)
	{
		JCoolTime -= DELTA;
		//cout <<"점프 쿨타임 : "<< JCoolTime << endl;
		if (JCoolTime < 0.0f)
		{
			isJump = false;
		}
	}
	
	if (isCharge)
	{
		RCoolTime -= DELTA;
		if (RCoolTime < 0.0f)
		{
			isCharge = false;
		}
	}
	else
	{
		RCoolTime = 5.0f;
	}

	//랜덤 Boss State 
	if (TIMER->GetTick(timer, 2.0f+startTime))
	{
		int temp = (int)state;
		if (state != BossState::DIED and
			state != BossState::INTRO)
		{
			{
				state = (BossState)RANDOM->Int(0, 5);
			}

			if (temp == (int)state)
			{
				state = (BossState)RANDOM->Int(0, 5);
			}

		}
		startTime = 0.0f;
		cout << "state :" << (int)state << endl;

	}
	//체력바 관리
	hpGage->scale.x = App.GetHalfWidth() / 20 * hp;

	if (hp == 0)
	{
		state = BossState::DIED;
		hpGage->visible = false;
	}


	switch (state)
	{
	case BossState::INTRO:
		Intro();
		break;
	case BossState::INTRO2:
		Intro2();
		break;
	case BossState::IDLE:
		Idle();
		break;
	case BossState::WALK:
		Walk();
		break;
	case BossState::DASH:
		Dash();
		break;
	case BossState::JUMP:
		Jump();
		break;
	case BossState::ATTK:
		Attk();
		break;
	case BossState::DIED:
		Died();
		break;
	}
	

	if (!(state == BossState::DIED))
	{
		col->Update();
		idle->Update();
		walk->Update();
		hand->Update();
		dash->Update();
		jump->Update();
		egun->Update();
		hpGage->Update();
	}
	for (int i = 0; i < 8; i++)
	{
		targetPoint[i]->Update();
	}

	for (int i = 0; i < 8; i++)
	{
		if (jumpShotBull[i]->isFlying)
		{
			jumpShotBull[i]->Update();
		}

	}
	hpGage->Update();
	intro->Update();
	nextIntro->Update();
	die->Update();
	
}

void Boss::Render()
{
	for (int i = 0; i < 8; i++)
	{
		if (jumpShotBull[i]->isFlying)
		{
			jumpShotBull[i]->Render();
		}
		
	}

	intro->Render();
	nextIntro->Render();
	die->Render();

	if (!(state == BossState::DIED))
	{
		if (isBack())
		{
			col->Render();
			egun->Render();
			idle->Render();
			walk->Render();
			dash->Render();
			jump->Render();
			hand->Render();
		}
		else
		{
			col->Render();
			idle->Render();
			walk->Render();
			dash->Render();
			jump->Render();
			egun->Render();
			hand->Render();
		}
		hpGage->Render();
	}
}
