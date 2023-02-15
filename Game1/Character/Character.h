#pragma once

enum DirectionState
{
    Dir_L,
    Dir_R,
    Dir_T,
    Dir_B,
    Dir_LT,
    Dir_LB,
    Dir_RT,
    Dir_RB,
    Dir_D, //Áß¸³
};

class Character
{
protected:
    int             dirState;
    int             frameY[8];
	int				hp;

	
	Vector2         moveDir;
    Vector2         target;
   

    vector<Vector2> foot;

    ObImage*		hand; 
	
    
public:
	ObRect*         col;

	int				gunidx;

    bool			isBack();
	bool			isDying = false;
	bool			isDamaging = false;
    bool            collable = true;
public:
    Vector2 GetPos()
    {
        return col->GetWorldPos();
    }
    vector<Vector2>& GetFoot()
    {
        foot[0] = col->GetWorldPos() + Vector2(0.0f, -30.0f) + Vector2(-10.0f, 12.0f);
        foot[1] = col->GetWorldPos() + Vector2(0.0f, -30.0f) + Vector2(10.0f, 12.0f);
        foot[2] = col->GetWorldPos() + Vector2(0.0f, -30.0f) + Vector2(10.0f, -12.0f);
        foot[3] = col->GetWorldPos() + Vector2(0.0f, -30.0f) + Vector2(-10.0f, -12.0f);

        return foot;
    }
    
    void LookDir(Vector2 dir, ObImage* img);
    void SetPos(Vector2 pos)
    {
        col->SetWorldPos(pos);
    }
    void SetTarget(Vector2 target)
    {
        this->target = target;
    }
    void HandPos(Vector2 point, Vector2 target);
	void SetHp(int hp) { this->hp = hp; };
	void Damaged(int damage);
    
    int GetHp() { return hp; };
};

