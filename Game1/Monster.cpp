#include "stdafx.h"
#include "eGun.h"
Monster::Monster()
{
	Vector2 charScale = Vector2(15, 20) * 5;
	col = new ObRect();
	col->isFilled = false;
	col->scale = charScale;
	col->visible = false;

	intro = new ObImage(L"enemyIntro.png");
	intro->scale = Vector2(39, 40) * 3.0f;
	intro->SetParentRT(*col);
	intro->maxFrame.x = 17;
	intro->maxFrame.y = 1;

	idle = new ObImage(L"anemyIdle.png");
	idle->scale = Vector2(15,25)*4.0f;
	idle->SetParentRT(*col);
	idle->maxFrame = Int2(2, 4);

	walk = new ObImage(L"anemyWalk.png");
	walk->scale = charScale;
	walk->SetParentRT(*col);
	walk->maxFrame = Int2(6, 4);

	hit = new ObImage(L"enemyHit.png");
	hit->scale = Vector2(20, 23) * 4.0f;
	hit->SetParentRT(*col);
	hit->maxFrame = Int2(13, 1);

	die = new ObImage(L"enemyDie.png");
	die->scale = Vector2(21, 22) * 4.0f;
	die->SetParentRT(*col);
	die->maxFrame = Int2(5, 1);

	hand = new ObImage(L"enemyHand.png");
	hand->scale = Vector2(30, 30);


	frameY[Dir_R] = 2;
	frameY[Dir_L] = 1;
	frameY[Dir_RB] = 2;
	frameY[Dir_LT] = 3;
	frameY[Dir_T] = 3;
	frameY[Dir_B] = 0;
	frameY[Dir_LB] = 1;
	frameY[Dir_RT] = 3;

	egun = new eGun();
	egun->col->pivot = OFFSET_LB;


	//���� �⺻ ����
	hp = 8;


	idle->visible = false;
	walk->visible = false;
	hit->visible = false;
	die->visible = false;
	intro->visible = true;
	egun->active = false;


	isIntro = false;
	monsterState = MonsterState::DIED;
	isDying = true;
}
Monster::~Monster()
{
	SafeDelete(col);
	SafeDelete(intro);
	SafeDelete(idle);
	SafeDelete(walk);
	SafeDelete(hit);
	SafeDelete(die);
	SafeDelete(hand);
	SafeDelete(egun);
}

void Monster::Intro()
{
	hand->visible = false;
	intro->ChangeAnim(ANISTATE::LOOP, 0.1f);
	if (intro->frame.x >= 12)
	{
		idle->visible = true;
		idle->ChangeAnim(ANISTATE::LOOP, 0.3f);
	}
	if (intro->frame.x >= 14)
	{
		intro->visible = false;
		isDying = false;
		isIntro = false;
		intro->frame.x = 0;
		monsterState = MonsterState::IDLE;

	}
}

void Monster::Idle()
{
	if (hp <= 0)return;
	idle->visible = false;
	walk->visible = true;
	egun->active = true;

	Vector2 dir = target - GetPos();
	LookDir(dir, idle);


	//LookTime += DELTA;
	/*if (LookTime > Time[(int)MonsterState::IDLE] )
	{
		idle->visible = false;
		walk->visible = true;
		LookTime -= LookTime;
		walk->ChangeAnim(ANISTATE::LOOP, 0.2f);
		monsterState = MonsterState::MOVE;
	}*/

}

void Monster::Move()
{
	if (hp <= 0)return;
	idle->visible = false;
	walk->visible = true;
	LookTime -= LookTime;
	walk->ChangeAnim(ANISTATE::LOOP, 0.2f);
	monsterState = MonsterState::MOVE;


	Vector2 dir = target - GetPos();
	LookDir(dir, walk);
	dir.Normalize();
	col->MoveWorldPos(dir * 80.0f * DELTA);

	//MoveTime += DELTA;
	/*if ( MoveTime > Time[(int)MonsterState::MOVE])
	{
		walk->visible = false;
		idle->visible = true;
		MoveTime -= MoveTime;
		idle->ChangeAnim(ANISTATE::LOOP, 0.2);
		monsterState = MonsterState::ATKK;
	}*/
}

void Monster::Atkk()
{
	if (hp <= 0)return;
	walk->visible = false;
	idle->visible = true;
	idle->ChangeAnim(ANISTATE::LOOP, 0.2);
	monsterState = MonsterState::ATKK;

	egun->button = false;
	if (TIMER->GetTick(AttkTime, 0.8f))
	{
		//cout << "������" << endl;
		egun->button = true;
		egun->Shot2(target);
	}

	//ATimer += DELTA;
	/*if (ATimer > Time[(int)MonsterState::ATKK])
	{
		idle->visible = false;
		walk->visible = true;
		ATimer -= ATimer;
		monsterState = MonsterState::MOVE;
	}*/
}

void Monster::Hit()
{
	hit->ChangeAnim(ANISTATE::LOOP, 0.1f);

	//cout << hit->frame.x << endl;
	if (hit->frame.x >= 12)
	{
		hit->visible = false;

		if (hp > 0)//hp�� ������, hit->idle
		{
			idle->visible = true;
			collable = true;
			hit->frame.x = 0;
			monsterState = MonsterState::IDLE;
			return;
		}
		else if (hp <= 0) // hp�� ������, ht->die
		{
			collable = false;
			hit->visible = false;
			hit->frame.x = 0;
			isDying = true;
			monsterState = MonsterState::DIED;
		}

	}
}

void Monster::Died()
{
	//cout << "���� ����" << endl;
	
	if (!isDying)
	{
		cout << "���� ��Ƴ�" << endl;
		collable = true;
		die->visible = false;
		intro->visible = true;
		isDying = false;
		monsterState = MonsterState::INTRO;
	}
	else
	{
		die->visible = true;
		die->ChangeAnim(ANISTATE::LOOP, 0.3f);
		if (die->frame.x >= 3)
		{
			die->visible = false;
			intro->visible = false;
			idle->visible = false;
			walk->visible = false;
			egun->active = false;
			collable = false;
			monsterState = MonsterState::INTRO;
		}
	}
}

void Monster::Stepback()
{
	col->SetWorldPos(lastPos);
	col->Update();
	idle->Update();
	walk->Update();
}

void Monster::Astar(ObTileMap * map)
{
	static bool beforeFind;
	if (monsterState == MonsterState::MOVE )
	{
		//   �����, ������
		Int2 sour, dest;
		//ã�Ҵ°�?
		bool isFind;
		isFind = map->WorldPosToTileIdx(GetPos(), sour);
		//isFind &= tm->VectorToIdx(g_WorldMouse, dest);
		isFind &= map->WorldPosToTileIdx(target, dest);
		//�Ѵ� Ÿ�ϸ� ����������
		if (isFind)
		{
			//���� �����Ѵٸ�
			if (map->PathFinding(sour, dest, Mway) and !beforeFind)
			{
				////�� �����ŭ �ݺ�
				//for (int i = 0; i < Mway.size(); i++)
				//{
				//	//Ȯ�ο�
				//	cout << "Way" << i << ":" << Mway[i]->idx.x <<
				//		"," << Mway[i]->idx.y << endl;
				//}
				

				g = 0.0f;
				MonSour = GetPos();
				Mway.pop_back(); //����� ����
				MonDest = Mway.back()->Pos;
			}
			beforeFind = isFind;
		}

		if (!Mway.empty())
		{
			//PlSour = pl->GetPos();
			SetPos(Util::Lerp(MonSour, MonDest, g));
			g += DELTA * 2.0f;
			//�������� ��������
			if (g > 1.0f)
			{
				g = 0.0f;
				MonSour = MonDest;
				Mway.pop_back(); //�ǵޱ� ����
				if (!Mway.empty())
					MonDest = Mway.back()->Pos;
				
			}
		}
		else
		{
			beforeFind = false;
		}
	}
}

Gun* Monster::getGun()
{
	return this->egun;
}


void Monster::Update()
{
	Vector2 edir = target - GetPos();
	//cout << "edir : " << edir.x << ", " << edir.y << endl;
	LookDir(edir, idle);
	distance = edir.Length();
	//�� ����
	HandPos(Vector2(1, 0.3), target);

	//LastPos�ޱ�
	lastPos = col->GetLocalPos();
	//�� ����
	egun->col->SetParentRT(*hand);
	egun->TargetDir(edir, egun->img, GUNSTATE::MAGNUM);

	/*if (hp <= 0)
	{
		isDying = true;
		monsterState = MonsterState::DIED;
	}*/

	if (isDamaging)
	{
		SetHp(this->hp - 1);
		hit->visible = true;
		idle->visible = false;
		walk->visible = false;
		/*if (GetHp() == 0)
		{
			isDying = true;
		}*/

		monsterState = MonsterState::HIT;
		isDamaging = false;

	}
	//���� ���¸� �������� �����Ѵ�.
	if (monsterState != MonsterState::HIT
		and monsterState != MonsterState::INTRO
		and monsterState != MonsterState::DIED)
	{
		//cout << (int)monsterState << endl;

		if (monsterState == MonsterState::MOVE)
		{
			MoveTime = 3.0f;
		}
		else
		{
			MoveTime = 0.0f;
		}
		if (TIMER->GetTick(ATimer, 2.0f+MoveTime) and !isDying)
		{
			//cout << "���� ��ȯ" << endl;
			int state = RANDOM->Int(0, 2);
			monsterState = (MonsterState)state;
			
		}
	}

	switch (monsterState)
	{
	case MonsterState::INTRO:
		Intro();
		break;
	case MonsterState::IDLE:
		Idle();
		break;
	case MonsterState::MOVE:
		Move();
		break;
	case MonsterState::ATKK:
		Atkk();
		break;
	case MonsterState::HIT:
		Hit();
		break;
	case MonsterState::DIED:
		Died();
		break;
	}


	//->Update();
	if (!isDying)
	{
		col->Update();
		intro->Update();
		idle->Update();
		walk->Update();
		hand->Update();
		hit->Update();
		egun->Update();
		die->Update();
	}

}

void Monster::Render()
{
	if (!isDying)
	{
		if (isBack())
		{
			col->Render();
			egun->Render();
			idle->Render();
			walk->Render();
			hand->Render();
		}
		else
		{
			col->Render();
			idle->Render();
			walk->Render();
			egun->Render();
			hand->Render();
		}
		intro->Render();
		hit->Render();
		die->Render();
	}

}
