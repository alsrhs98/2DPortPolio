#include "stdafx.h"

Gun::Gun()
{
	col = new ObRect();
	col->visible = false;


	firePos = new ObRect();
	firePos->pivot = OFFSET_L;
	firePos->scale = Vector2(30, 30);
	firePos->color = Color(1, 0, 0, 1);
	firePos->visible = false;
	firePos->SetParentRT(*col);


}
Gun::~Gun()
{
	SafeDelete(firePos);
}
void Gun::TargetDir(Vector2 dir, ObImage* img, GUNSTATE gunstate)
{
	float radian = Util::DirToRadian(dir);
	int i = 0;
	if (-PI_DIV8 <= radian && radian < PI_DIV8)
	{
		i = 1;
		img->reverseHL = false;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LB;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_B;
		}
	}
	else if (PI_DIV8 <= radian && radian < PI_DIV8 + PI_DIV4)
	{
		i = 2;
		img->reverseHL = false;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LB;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_B;
		}

	}
	else if (PI_DIV8 + PI_DIV4 <= radian && radian < PI_DIV8 + PI_DIV4 * 2.0f)
	{
		i = 3;
		img->reverseHL = false;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LB;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_B;
		}

	}
	else if (PI_DIV8 + PI_DIV4 * 2.0f <= radian && radian < PI_DIV8 + PI_DIV4 * 3.0f)
	{
		i = 4;
		img->reverseHL = true;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LT;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_T;
		}

	}
	else if (-(PI_DIV8 + PI_DIV4 * 2.0f) > radian && radian >= -(PI_DIV8 + PI_DIV4 * 3.0f))
	{
		i = 5;
		img->reverseHL = true;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LT;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_T;
		}
	}
	else if (-(PI_DIV8 + PI_DIV4) > radian && radian >= -(PI_DIV8 + PI_DIV4 * 2.0f))
	{
		i = 6;
		img->reverseHL = false;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LB;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_B;
		}

	}
	else if (-(PI_DIV8) > radian && radian >= -(PI_DIV8 + PI_DIV4))
	{
		i = 7;
		img->reverseHL = false;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LB;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
			img->pivot = OFFSET_B;
		}

	}
	else
	{
		i = 8;
		img->reverseHL = true;
		if (gunstate == GUNSTATE::MAGNUM)
		{
			img->pivot = OFFSET_LT;
		}
		else if (gunstate == GUNSTATE::RIFLE)
		{
		}

	}

	
	//if (INPUT->KeyDown('R'))
	//{
	//	cout << "°¢µµ : " << i << endl;
	//}
}
void Gun::Shot()
{
}
bool Gun::Empty()
{
	Bullet* temp = magazine[0];
	int  i = 0;
	while (temp->isFire)
	{
		temp = temp + i;
		i++;
		if (i > maxBullet - 1)
		{
			return true;
		}
	}
	return false;
}
void Gun::Reload()
{
	for (int i = 0; i <= bulletCnt; i++)
	{
		if (magazine[i]->isFire and !magazine[i]->isFlying)
		{
			magazine[i]->col->SetWorldPos(firePos->GetWorldPos());
			magazine[i]->isFire = false;
		}
		else break;
	}
	bulletCnt = 0;
}
void Gun::Update()
{

	//Vector2 dir = INPUT->GetMouseWorldPos() - col->GetWorldPos();
	//TargetDir(dir, img, gunstate);

	if (img->reverseHL)
	{
		firePos->SetLocalPos(Vector2(col->scale.x, -col->scale.y / 2));
	}
	else
	{
		firePos->SetLocalPos(Vector2(col->scale.x, col->scale.y / 2));
	}

	col->Update();
	img->Update();
	firePos->Update();

	for (int i = 0; i < magazine.size(); i++)
	{
		magazine[i]->Update();
	}

}
void Gun::Render()
{
	
	for (int i = 0; i < magazine.size(); i++)
	{
		magazine[i]->Render();
	}
	if (active)
	{
		col->Render();
		img->Render();
		firePos->Render();
	}
	
}
