#include "stdafx.h"

void Character::LookDir(Vector2 dir, ObImage * img)
{
	float radian = Util::DirToRadian(dir);

	if (-PI_DIV8 <= radian && radian < PI_DIV8)
	{
		dirState = Dir_R;
	}
	else if (PI_DIV8 <= radian && radian < PI_DIV8 + PI_DIV4)
	{
		dirState = Dir_RT;
	}
	else if (PI_DIV8 + PI_DIV4 <= radian && radian < PI_DIV8 + PI_DIV4 * 2.0f)
	{
		dirState = Dir_T;
	}
	else if (PI_DIV8 + PI_DIV4 * 2.0f <= radian && radian < PI_DIV8 + PI_DIV4 * 3.0f)
	{
		dirState = Dir_LT;
	}
	else if (-(PI_DIV8 + PI_DIV4 * 2.0f) > radian && radian >= -(PI_DIV8 + PI_DIV4 * 3.0f))
	{
		dirState = Dir_LB;
	}
	else if (-(PI_DIV8 + PI_DIV4) > radian && radian >= -(PI_DIV8 + PI_DIV4 * 2.0f))
	{
		dirState = Dir_B;
	}
	else if (-(PI_DIV8) > radian && radian >= -(PI_DIV8 + PI_DIV4))
	{
		dirState = Dir_RB;
	}
	else
	{
		dirState = Dir_L;
	}
	img->frame.y = frameY[dirState];
}

bool Character::isBack()
{
	if (dirState == Dir_T or dirState == Dir_LT or dirState == Dir_RT)
	{
		//cout << "µÚ µ· »óÅÂ" << endl;
		return true;
	}
	else
	{
		//cout << "¾Õ ÀÎ »óÅÂ" << endl;
		return false;
	}
}

void Character::HandPos(Vector2 point, Vector2 target)
{
	hand->SetParentT(*col);
	Vector2 pos;
		pos.y = -col->scale.y * point.y;
	switch (dirState)
	{
	case  Dir_L  :
	case  Dir_LB :
		pos.x = col->scale.x / 2 * -point.x;
		hand->SetLocalPos(pos);
		hand->visible = true;
		break;
	case  Dir_R : 
	case  Dir_B :
	case  Dir_RB :
		pos.x = col->scale.x / 2 * point.x;
		hand->SetLocalPos(pos);
		hand->visible = true;
		break;
	case  Dir_RT :
		pos.x = col->scale.x / 2 * point.x;
		pos.y = 2;
		hand->SetLocalPos(pos);
		hand->visible = false;
		break;
	case  Dir_LT :
		pos.x = col->scale.x / 2 * -point.x;
		pos.y = 2;
		hand->SetLocalPos(pos);
		hand->visible = false;
		break;
	}

	Vector2 dir = target - hand->GetWorldPos();
	dir.Normalize();
	
	hand->rotation = Util::DirToRadian(dir);

}

void Character::Damaged(int damage)
{
	static bool damaging = false;
	isDamaging = true;

	if (damage != 0 and !damaging)
	{
		//cout << "¸÷ hp : "<<this->hp << endl;
		SetHp(GetHp() - 1 * damage);
		return;
	}
	damaging = isDamaging;
	
}





