#pragma once
#define MAXBULLET 50
enum class BossState
{
	IDLE = 0,
	WALK = 1,
	DASH = 2,
	ATTK = 3,
	JUMP = 4,
	CHRG = 5,
	INTRO = 8,
	INTRO2= 9,
	DIED = 10
};
class Boss : public Character
{
private:
	ObCircle* targetPoint[8];

	ObImage* intro;
	ObImage* nextIntro;
	ObImage* idle;
	ObImage* walk;
	ObImage* dash;
	ObImage* jump;
	ObImage* die;

	ObRect* hpGage;

	eGun* egun;

private:

	float attkTime = 0.0f;
	float jumpTime = 0.0f;
	float timer = 0.0f;

	float JCoolTime = 5.0f;
	float RCoolTime = 5.0f;

	bool isJump = false;
	bool isCharge = false;
	bool canMove = false;

	float startTime;
	
public:
	BossState state;
	Bullet* jumpShotBull[8];
	Bullet* trippleShot[3][8];

	Bullet* rotateBull[8];

	Gun* getGun()
	{
		return this->egun;
	}

public:
	Boss();
	Boss(int num);
	~Boss();

	void TrippleShot();
	void JumpShot(Vector2 firepos);
	void RotateShot(float theta);

	void Intro();
	void Intro2();
	void Idle();
	void Walk();
	void Dash();
	void Jump();
	void Attk();
	void Died();

	void SetState(int i) { this->state = (BossState)i; };

	virtual void Update();
	virtual void Render();
};

