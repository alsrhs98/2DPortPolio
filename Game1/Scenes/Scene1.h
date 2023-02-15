#pragma once
class Scene1 : public Scene
{
private:
	bool	FadeOut;
	float	FadeIn;

	vector<Monster*> mon;

	Blank* blank[3];

	ObTileMap* map;

	//리스폰 위치지정
	Vector2 respawnPoint;

	vector<Vector2> enemyRespawnPoints;

	float	time = 0.0f;
	float	lateTime = 0.0f;

	int		enemyCnt;

	bool	roomClear = true;

	int     EnemyCnt();
	void	VisitCheck(ObRect* col, MAPTREE* room);
	void	enemyRespawnPoint(MAPTREE* room);
public:
	Player* pl;

	bool stageClear = false;

public:
	Scene1();
	~Scene1();

	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void ResizeScreen() override;

};


