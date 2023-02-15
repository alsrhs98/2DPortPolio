#pragma once
#define MAXBULLET 8
enum HAVINGSTATE
{
	ENEMY = 0 ,
	PLAYER = 1
};
enum class BULLETSTATE
{
	RELOAD,
	FLYING,
	DISAPP
};

class Bullet
{
private:
	int Damage;
	int havingState;
	float speed;
	float accel;

public:
	ObCircle* col;
	ObImage* img;

	bool isFire;
	bool isFlying;
	bool isReload;

	Vector2 dir;
	Vector2 firepos;

	
public:
	Bullet();
	Bullet(int i);
	~Bullet();

	void SetBullet(int damage, int speed)
	{
		this->Damage = damage;
		this->speed = speed;
	}

	void Shot();
	void colMap(ObTileMap* map);
	void Update();
	void Render();

	bool colOb(GameObject* ob);


	int GetDamage() { return this->Damage; };
};

