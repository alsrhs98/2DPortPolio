#include "stdafx.h"

Magnum::Magnum()
{
	col->scale = Vector2(17, 13)*3;
	col->pivot = OFFSET_LB;

	img = new ObImage(L"Magnum.png");
	img->scale = col->scale;
	img->pivot = OFFSET_LB;
	img->SetParentRT(*col);
	
	firePos->SetLocalPos(Vector2(col->scale.x, col->scale.y / 2));

	maxBullet = 10;

	magazine.resize(maxBullet);
	for (int i = 0; i < magazine.size(); i++)
	{
		magazine[i] = new Bullet((int)PLAYER);
		magazine[i]->SetBullet(1, 400.0f);
	}

	gunstate = GUNSTATE::MAGNUM;

}


Magnum::~Magnum()
{
	SafeDelete(col);
	SafeDelete(img);
	
	for (int i = 0; i < magazine.size(); i++)
	{
		SafeDelete(magazine[i]);
	}
}

void Magnum::Shot()
{
	Vector2 dir = INPUT->GetMouseWorldPos() - col->GetWorldPos();
	dir.Normalize();
	static bool prev_button = false;


	if (bulletCnt == maxBullet-1)
	{
		//cout << "재장전이 필요합니다... 자동재장전" << endl;
		//Reload();
		return;
	}
	if (button and !prev_button)
	{
		magazine[bulletCnt]->dir = dir;
		magazine[bulletCnt]->isFire = true;
		magazine[bulletCnt]->col->SetWorldPos(firePos->GetWorldPos());
		magazine[bulletCnt]->Shot();
		//cout << bulletCnt << endl;
		bulletCnt++;
	}
	
	
	
	prev_button = button;

}




