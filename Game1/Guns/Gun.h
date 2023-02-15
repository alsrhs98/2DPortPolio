#pragma once
enum class GUNSTATE
{
	MAGNUM =1,
	RIFLE =2
};

class Gun
{
public:
	ObRect* col;
	ObImage* img;
	ObRect* firePos;
public:
	Gun();
	~Gun();

	GUNSTATE gunstate;

	bool active;
	
	bool button = false;

	int maxBullet;
	int bulletCnt = 0;

	vector<Bullet*>magazine;
	void TargetDir(Vector2 dir, ObImage* img, GUNSTATE gunstate);
	
	virtual bool Empty();
	virtual void Shot();
	virtual void Reload();
	virtual void Update();
	virtual void Render();
	
};

