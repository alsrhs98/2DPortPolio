#pragma once
enum class Inven
{
	inven1,
	inven2
};
enum class PlayerState
{
    IDLE,
    WALK,
    ROLL,
	DIED
};
struct HP
{
	ObRect* col;
	ObImage* halfHeart;
	ObImage* heart;
	
	HP();
	~HP();
	void full();
	void half();
	void none();
	
	void Update();
	void Render();
	
};

class Player : public Character
{
private:
	ObImage *	idle;
    ObImage *   walk;
    ObImage *   roll;
	ObImage *	die;
	
	

	ObRect*					reloadGage;
	Gun*					gun[2];
	vector<Blank*>          itemInven;


    PlayerState				playerState;

	bool       Reloading = false;


    float       rollTime;
	float       damagingTime = 1.0f;
	float		timer = 0.0f;
	float		renderTime;

    Vector2     lastPos;

	//인벤토리 관련
	Inven inven;
	Blank* blank;

	//Blanck 관련
	ObCircle* blankRange;
	bool      blankActive;

    void Idle();
    void Walk();
    void Roll();
	void Died();
    void Input();

	void GunChange();
	void GageRoad();
	void Blanking();

public:
	vector<HP*>playerHp;
	Gun* getGun(int gunidx) { return gun[gunidx]; };
public:
    Player();
    ~Player();

	void SettingHp(int hp);
	void SetItem(Blank* item);

    void Update();
    void Render();
    void Stepback();


   
};

