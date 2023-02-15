#pragma once
class eGun : public Gun
{
private :
	float shotTime = 0.0f;
	bool fire = false;

public:
	eGun();
	~eGun();

	virtual void Shot2(Vector2 target);	
};

