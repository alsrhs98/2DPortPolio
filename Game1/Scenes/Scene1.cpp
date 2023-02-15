#include "stdafx.h"

Scene1::Scene1()
{
	pl = new Player();
	pl->SettingHp(6);
	
	map = new ObTileMap();
	map->scale = Vector2(70, 70);
	map->AutomaticMap(map->mtree);
	map->CreateTileCost();

	for (int i = 0; i < 3; i++)
	{
		blank[i] = new Blank();
		blank[i]->IsObtain = false;
	}
	
	
	mon.resize(4);
	for (int i = 0; i < mon.size(); i++)
	{
		mon[i] = new Monster();
	}
	enemyCnt = mon.size();

   
	//FadeOut = false;
	//FadeIn = 1.0f;
	//LIGHT->light.radius = 0.0f;
	//LIGHT->light.lightColor = Color(0.5f, 0.5f, 0.5f, 0.5f);

	LIGHT->light.radius = 3000;
}

Scene1::~Scene1()
{
	SafeDelete(map);
	for (int i = 0; i < 4; i++)
	{
		SafeDelete(mon[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		SafeDelete(blank[i]);
	}
}

//��ȯ�� �� ����
int Scene1::EnemyCnt()
{
	enemyCnt =  0;
	if (!(mon[0]->isDying))enemyCnt++;
	if (!(mon[1]->isDying))enemyCnt++;
	if (!(mon[2]->isDying))enemyCnt++;
	if (!(mon[3]->isDying))enemyCnt++;
	
	return enemyCnt;

}

//�ش� �� �湮üũ
//�湮�� �ȵ� ���̶�� -> ������ - > �湮 üũ true
//�湮�� �� ���̶�� -> ������ x 
void Scene1::VisitCheck(ObRect* col, MAPTREE* room)
{
	Int2 on;
	map->WorldPosToTileIdx(col->GetWorldPos(),on);
	
	if (on.x > room->LB.x+3 and on.x < room->RT.x-3 and
		on.y > room->LB.y+3 and on.y < room->RT.y-3)
	{
		//cout << "on : " << on.x << ", "<<on.y << endl;
		//cout << "Size X : " << room->LB.x + 3 << "~"<< room->RT.x - 3 <<endl;
		//cout << "Size Y : " << room->LB.y + 3 << "~"<< room->RT.y - 3 <<endl;
		
		//������ ����Ʈ ����
		enemyRespawnPoint(room);

		if (!room->visited)
		{
			for (int i = 0; i < mon.size(); i++)
			{
				if (mon[i]->isDying)
				{
					mon[i]->isDying = false;
					mon[i]->SetHp(8);
					mon[i]->isIntro = true;
					mon[i]->SetPos(enemyRespawnPoints.back());
					enemyRespawnPoints.pop_back();
					cout << EnemyCnt() << " : ";
					cout << "���� ����" << endl;
				}
				else
				{
					cout <<"state : " <<(int) mon[i]->GetState() << endl;
				}
			}
			room->visited = true; 
			cout << room->num << "���� �湮��" << endl;
			
		}
		
	}

}
//������ ����Ʈ �����
void Scene1::enemyRespawnPoint(MAPTREE* room)
{
	Int2 temp = Int2(0,0);
	for (int i = 0; i < 4; i++)
	{
		int X = RANDOM->Int(room->LB.x+3, room->RT.x-3);
		int Y = RANDOM->Int(room->LB.y+3, room->RT.y-3);
		
		if ((X < temp.x + 3 and X > temp.x - 3 )or
			(Y <temp.y +3 and Y > temp.x-3)
			or map->GetTileState(Int2(X,Y-1))==TILE_WALL
			or map->GetTileState(Int2(X, Y+1)) == TILE_WALL
			or map->GetTileState(Int2(X-1, Y)) == TILE_WALL
			or map->GetTileState(Int2(X+1, Y)) == TILE_WALL)
		{
			i--;
		}

		temp = Int2(X, Y);
		
		Vector2 worldPos = map->TileIdxToWorldPos(Int2(X, Y));
		enemyRespawnPoints.push_back(worldPos);
		//cout << "�� ������ ����Ʈ : " << worldPos.x << ", " << worldPos.y << endl;
	}
	
	

}
void Scene1::Init()
{
	//������ �� �湮����
	respawnPoint = map->TileIdxToWorldPos(map->mtreeList[0].midPoint());
	map->mtreeList[0].visited = true;
	map->Update();
	pl->SetPos(respawnPoint);

	//Blank ������ ���� ��ġ
	for (int i = 0; i < 3; i++)
	{
		Int2 randomPos;
		randomPos.x = RANDOM->Int(map->mtreeList[0].LB.x+2, map->mtreeList[0].RT.x-2);
		randomPos.y = RANDOM->Int(map->mtreeList[0].LB.y+2, map->mtreeList[0].RT.y-2);

		blank[i]->SetPos(map->TileIdxToWorldPos(randomPos));
	}
	CAM->position = pl->GetPos();
}

void Scene1::Release()
{
}
void Scene1::Update()
{
	if (ImGui::Button("ChangingScene"))
	{
		SCENE->ChangeScene("SC1", 1.0f);
		FadeOut = true;
	}
	if (ImGui::Button("EnemyRespawn"))
	{
		for (int i = 0; i < map->mtreeList.size(); i++)
		{
			//if (map->mtreeList[i].visited)continue;

			Int2 pos;
			map->WorldPosToTileIdx(pl->GetPos(), pos);
			if (map->isPlayerVisiting(pos, &map->mtreeList[i]))
			{
				map->mtreeList[i].visited = true;
			}
			

			if (map->mtreeList[i].visited)
			{
				cout << i <<"���� �湮��." << endl;
			}
			//VisitCheck(pl->col, &map->mtreeList[i]);
		}
	}

	


	//�湮üũ
	for (int i = 0; i < map->mtreeList.size(); i++)
	{
		if (map->mtreeList[i].visited)
		{
			//cout << i << " ���� �湮" << endl;
			continue;
		}
		
		VisitCheck(pl->col, &map->mtreeList[i]);
		
	}
	
	//�ʿ� ��ȯ�Ǿ� �ִ� �� ī����
	//cout << EnemyCnt() << endl;
	if (EnemyCnt() == 0)
	{
		roomClear = true;
		//cout << temp << " : " << "��Ŭ����!" << endl;
	}
	else
	{
		roomClear = false;
		//cout << temp << " : " << "�� ����" << endl;
	}
	//cout << "State : " << (int)mon[0]->GetState() << endl;
	//enemyRespawnPoint();
	for (int i = 0; i < 4; i++)
	{
		if (!mon[i]->isDying)
		{
			mon[i]->SetTarget(pl->GetPos());
			//mon[i]->Astar(map);
			//cout <<i <<" : " << (int)mon[i]->GetState() << endl;
		}
	}
	
	//���� Update
	mon[0]->Astar(map);
	mon[0]->Update();
	mon[1]->Update();
	mon[2]->Update();
	mon[3]->Update();
	map->Update();
	pl->Update();

	for (int i = 0; i < 3; i++)
	{
		if (blank[i]->col->Intersect(pl->col)and !blank[i]->IsObtain)
		{
			pl->SetItem(blank[i]);
			blank[i]->IsObtain = true;
		}

		blank[i]->Update();
	}

	time += DELTA;
	if (time <1.5f)
	{
		CAM->position = pl->GetPos();
	}
	else if (time > 1.5f)
	{
		Vector2 dir = ((pl->GetPos() + INPUT->GetMouseWorldPos()) / 2.0f) - CAM->position;
		CAM->position += 20 * dir * DELTA;
	}
}

void Scene1::LateUpdate()
{
	//�÷��̾� - �� �浹
	vector<Vector2> & Foot = pl->GetFoot();
	for (int i = 0; i < 4; i++)
	{
		Int2 on;
		if (map->WorldPosToTileIdx(Foot[i], on))
		{
			//ImGui::Text("TileState %d", map->GetTileState(on));
			if (map->GetTileState(on) == TILE_WALL)
			{
				pl->Stepback();
			}
		}
	}
	//���� �� �浹
	for (int i = 0; i < 4; i++)
	{
		Int2 on;
		Vector2  pos = mon[i]->GetPos() - Vector2(0,40);
		Vector2  pos2 = mon[i]->GetPos() + Vector2(0,40);
		if (map->WorldPosToTileIdx(pos, on))
		{
			Vector2 backmove = mon[i]->GetPos()- map->TileIdxToWorldPos(on);
			backmove.Normalize();
			//ImGui::Text("TileState %d", map->GetTileState(on));
			if (map->GetTileState(on) == TILE_WALL)
			{
				mon[i]->col->MoveWorldPos(backmove * 10);
			}
		}
		else if (map->WorldPosToTileIdx(pos2, on))
		{
			Vector2 backmove = mon[i]->GetPos() - map->TileIdxToWorldPos(on);
			backmove.Normalize();
			if (map->GetTileState(on) == TILE_WALL)
			{
				mon[i]->col->MoveWorldPos(backmove * 10);
			}
		}
	}

	//�÷��̾��� �Ѿ˰��� �浹 ����
	for (int i = 0; i < pl->getGun(pl->gunidx)->magazine.size(); i++)
	{
		Bullet* bullet = pl->getGun(pl->gunidx)->magazine[i];
		for (int i = 0; i < 4; i++)
		{
			if (bullet->col->Intersect(mon[i]->col) and !mon[i]->isDying)
			{
				//�ڷ� �з�����
				Vector2 backmove = bullet->col->GetWorldPos()-pl->GetPos();
				backmove.Normalize();

				if (mon[i]->isDying)cout << "���� ����" << endl;

				bullet->isFlying = false;
				bullet->isFire = false;

				mon[i]->isDamaging = true;
				mon[i]->col->MoveWorldPos(backmove * 20);
				mon[i]->Damaged(bullet->GetDamage());

				//break;
			}
		}
		//�Ѿ˰� �� �浹;
		bullet->colMap(map);
		
	}
	
	//�� �Ѿ˰� �÷��̾� �浹;
	for (int i = 0; i < mon[0]->getGun()->magazine.size(); i++)
	{
		for (int j = 0; j <mon.size(); j++)
		{
			//cout << j << "  ";
			Bullet* bullet = mon[j]->getGun()->magazine[i];
			if (bullet->colOb(pl->col))
			{
				//�ڷ� �з�����

				pl->isDamaging = true;
				pl->Stepback();
				bullet->isFire = false;

				if (pl->collable)
				{
					pl->Damaged(1);
				}
			}
			bullet->colMap(map);
		}
	}

	

	map->ControlDoor(roomClear);
	map->Update();
}

void Scene1::Render()
{
	map->Render();
	for (int i = 0; i < 4; i++)
	{
		mon[i]->Render();
	}
	for (int i = 0; i < 3; i++)
	{
		blank[i]->Render();
	}
	pl->Render();
}
void Scene1::ResizeScreen()
{
}
