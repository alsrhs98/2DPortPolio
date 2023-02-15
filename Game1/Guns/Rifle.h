#pragma once
class Rifle : public Gun
{
public:
	Rifle();
	~Rifle();

	Bullet* bullet[30];

	float per;

	virtual void Shot();
};

