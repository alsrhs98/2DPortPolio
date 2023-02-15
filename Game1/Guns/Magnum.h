#pragma once
class Magnum : public Gun
{
public:
	Magnum();
	~Magnum();

	Bullet* bullet[10];
	virtual void Shot();

};

