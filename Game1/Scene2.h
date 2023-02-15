#pragma once
class Scene2 : public Scene
{
private:
	bool	FadeOut;
	float	FadeIn;

	
	Boss* boss1;
	ObTileMap* map;


	//리스폰 위치지정
	Vector2 respawnPoint[3];

	//비율

	float	time = 0.0f;
	float	lateTime = 0.0f;

	bool isClear = false;
public:
	Player* pl;
public:
	Scene2();
	~Scene2();

	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void ResizeScreen() override;
};