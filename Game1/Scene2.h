#pragma once
class Scene2 : public Scene
{
private:
	bool	FadeOut;
	float	FadeIn;

	
	Boss* boss1;
	ObTileMap* map;


	//������ ��ġ����
	Vector2 respawnPoint[3];

	//����

	float	time = 0.0f;
	float	lateTime = 0.0f;

	bool isClear = false;
public:
	Player* pl;
public:
	Scene2();
	~Scene2();

	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
};