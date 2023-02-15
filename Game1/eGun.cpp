#include "stdafx.h"

eGun::eGun()
{
	col->scale = Vector2(17, 13) * 3;
	col->pivot = OFFSET_LB;

	img = new ObImage(L"Magnum.png");
	img->scale = col->scale;
	img->pivot = OFFSET_LB;
	img->SetParentRT(*col);

	firePos->SetLocalPos(Vector2(col->scale.x, col->scale.y / 2));

	maxBullet = 30;

	magazine.resize(maxBullet);
	for (int i = 0; i < magazine.size(); i++)
	{
		magazine[i] = new Bullet(int(ENEMY));
		
		magazine[i]->SetBullet(1, 400.0f);
	}

	gunstate = GUNSTATE::MAGNUM;
}

eGun::~eGun()
{
	SafeDelete(col);
	SafeDelete(img);

	for (int i = 0; i < magazine.size(); i++)
	{
		SafeDelete(magazine[i]);
	}
}

void eGun::Shot2(Vector2 target)
{
	if (button)
	{
		Vector2 dir = target - col->GetWorldPos();
		dir.Normalize();

		if (bulletCnt == maxBullet - 1)
		{
			Reload();
			return;
		}

		{
			//cout << "Àû ÃÑ ½ô!" << endl;
			magazine[bulletCnt]->dir = dir;
			magazine[bulletCnt]->isFlying = true;
			magazine[bulletCnt]->col->SetWorldPos(firePos->GetWorldPos());
			magazine[bulletCnt]->Shot();
			//cout << bulletCnt << endl;
			bulletCnt++;
		}

	}

}
