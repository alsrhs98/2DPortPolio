#pragma once

enum class MonsterState
{
    MOVE = 0,
    IDLE = 1,
    ATKK = 2,
    INTRO = 3,
    HIT = 4,
	DIED = 5 
};

class Monster : public Character
{
private:
    ObImage*        intro;
    ObImage *       idle;
    ObImage *       walk;
    ObImage*        hit;
    ObImage*        die;


	eGun*			egun;

	Vector2 lastPos;
	
    
	float           distance;
	float g;

	int gunidx;
    int Time[3] = { 3, 4, 5 };



    void Intro();
    void Idle();
    void Move();
    void Atkk();
    void Hit();
	void Died();

   
	

	//AI용 시간 변수
    float time = 0.0f;
    float introTime = 0.0f;
    float LookTime = 0.0f;
	float MoveTime = 0.0f;
	float AttkTime = 0.0f;
    float ATimer = 0.0f;


    MonsterState    monsterState;

public:

	//이동시킬 위치
	Vector2 MonDest;
	Vector2 MonSour;
	vector<Tile*>Mway;


    bool isIntro;
public:
    Monster();
    ~Monster();

    void Update();
    void Render();

	void Stepback();
	void Astar(ObTileMap* map);
	Gun* getGun();

	MonsterState GetState()
	{
		return this->monsterState;
	}

};

