#include "stdafx.h"
Scene2::Scene2()
{
	
	boss1 = new Boss(1);
	map = new ObTileMap();
	map->scale = Vector2(60,60);
	map->AutoBossMap();
}


Scene2::~Scene2()
{
	SafeDelete(boss1);
	SafeDelete(map);
}

void Scene2::Init()
{
	//리스폰
	Int2 pos;
	pos.x= map->GetTileSize().x/2;
	pos.y=3;
	respawnPoint[0] = map->TileIdxToWorldPos(pos);
	pl->SetPos(respawnPoint[0]);

	Int2 bossPos;
	bossPos.x = map->GetTileSize().x / 2;
	bossPos.y = map->GetTileSize().y / 2;
	respawnPoint[1] = map->TileIdxToWorldPos(bossPos);
	boss1->state == BossState::INTRO;
	boss1->SetPos(respawnPoint[1]);
	


	
	boss1->Update();
}

void Scene2::Release()
{
}

void Scene2::Update()
{
	if (ImGui::Button("ChangingScene"))
	{
		SCENE->ChangeScene("SC0", 1.0f);
		FadeOut = true;
	}
	

	boss1->SetTarget(pl->col->GetWorldPos());

	
	float startTime = 1.0f;
	time += DELTA;


	pl->Update();
	map->Update();
	boss1->Update();

	//카메라 작동
	Int2 startCAMpos;
	if (time < startTime+4.0f)
	{
		startCAMpos.x = map->GetTileSize().x/2;
		startCAMpos.y = map->GetTileSize().y/2;
		CAM->position = map->TileIdxToWorldPos(startCAMpos);
	}
	else if (time > startTime+4.0f and time< startTime+ 7.0f)
	{
		Vector2 dir = pl->GetPos() - CAM->position;
		CAM->position += dir * DELTA;
	}
	else if(time > startTime+7.0f)
	{
		Vector2 dir = ((pl->GetPos() + INPUT->GetMouseWorldPos()) / 2.0f) - CAM->position;
		CAM->position += 25*dir * DELTA;
	}
	
}

void Scene2::LateUpdate()
{
	//플레이어의 총알과의 충돌 구현
	for (int i = 0; i < pl->getGun(pl->gunidx)->magazine.size(); i++)
	{
		Bullet* bullet = pl->getGun(pl->gunidx)->magazine[i];
		
		if (bullet->colOb(boss1->col) and boss1->collable)
		{
			//뒤로 밀려나기
			Vector2 backmove = bullet->col->GetWorldPos() - pl->GetPos();
			backmove.Normalize();

			boss1->isDamaging = true;
			boss1->col->MoveWorldPos(backmove * 20);
			boss1->Damaged(bullet->GetDamage());
			bullet->isFire = false;

			//break;
		}
		
		//총알과 맵 충돌;
		bullet->colMap(map);

	}
	//적 총알과 플레이어 충돌;
	for (int i = 0; i < boss1->getGun()->magazine.size(); i++)
	{
		Bullet* bullet = boss1->getGun()->magazine[i];
		if (bullet->colOb(pl->col))
		{
			//뒤로 밀려나기

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

void Scene2::Render()
{
	map->Render();
	boss1->Render();
	pl->Render();
}

void Scene2::ResizeScreen()
{
}
