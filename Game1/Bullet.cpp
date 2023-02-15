#include "stdafx.h"

Bullet::Bullet()
{
}
// 1 플레이어 0 몬스터
Bullet::Bullet(int i)
{
	col = new ObCircle();
	col->scale = Vector2(15, 15)*2.0f;
	col->visible = false;

	if (i == (int)PLAYER)
	{
		img = new ObImage(L"playerBullet.png");
	}
	else if( i == (int)ENEMY)
	{
		img = new ObImage(L"enemyBullet.png");
	}

	img->scale = col->scale;
	img->SetParentRT(*col);

	accel = 0;
	isFire = false;
	isFlying = false;
	isReload = true;


}

Bullet::~Bullet()
{
	SafeDelete(col);
	SafeDelete(img);
}

void Bullet::Shot()
{
	//accel += 10.0f * DELTA;
	isFire = true;
	isFlying = true;
	/// += accel;
	col->MoveWorldPos(dir * speed * DELTA);
}

bool Bullet::colOb(GameObject* ob)
{
	if (col->Intersect(ob))
	{
		accel = 0;
		isFlying = false;
		isFire = false;
		return true;
	}
	else return false;
}

void Bullet::colMap(ObTileMap* map)
{
	Int2 on;
	if (map->WorldPosToTileIdx(col->GetWorldPos(), on))
	{
		//ImGui::Text("TileState %d", map->GetTileState(on));
		if ( map->GetTileState(on) == TILE_WALL)
		{
			accel = 0;
			isFire = false;
			isFlying = false;
			return;
		}
	}
}




void Bullet::Update()
{
	if (isFire)
	{
		Shot();
	}

	if (isFlying)
	{
		img->Update();
		col->Update();
	}
}

void Bullet::Render()
{
	if (isFlying)
	{
		img->Render();
		col->Render();
	}
}
