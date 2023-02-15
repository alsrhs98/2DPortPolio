#include "stdafx.h"

Rifle::Rifle()
{
	col->scale = Vector2(27,7)*3;
	col->pivot = OFFSET_B;

	img = new ObImage(L"ak47.png");
	img->scale = col->scale;
	img->pivot = OFFSET_B;
	img->SetParentRT(*col);

	firePos->SetParentRT(*col);
	firePos->pivot = OFFSET_B;
	firePos->SetLocalPos(Vector2(-200, col->scale.y / 2));
	//firePos->visible = true;

	maxBullet = 30;
	magazine.resize(maxBullet);
	for (int i = 0; i < magazine.size(); i++)
	{
		magazine[i] = new Bullet((int)PLAYER);
		magazine[i]->SetBullet(2, 500.0f);
	}
	
	gunstate = GUNSTATE::RIFLE;
}

Rifle::~Rifle()
{
	SafeDelete(col);
	SafeDelete(img);
	for (int i = 0; i < magazine.size(); i++)
	{
		SafeDelete(magazine[i]);
	}
}
void Rifle::Shot()
{
	Vector2 dir = INPUT->GetMouseWorldPos() - col->GetWorldPos();
	dir.Normalize();
	bool prev_button = false;


	if (bulletCnt == maxBullet - 1)
	{
		//cout << "재장전이 필요합니다... 자동재장전" << endl;
		//Reload();
		return;
	}
	if(TIMER->GetTick(per,0.2f))
	{
		prev_button = true;
	}
	
	if (button and prev_button)
	{
		magazine[bulletCnt]->dir = dir;
		magazine[bulletCnt]->col->SetWorldPos(firePos->GetWorldPos());
		magazine[bulletCnt]->Shot();
		//cout << bulletCnt << endl;
		bulletCnt++;
		

	}

	
	


}





