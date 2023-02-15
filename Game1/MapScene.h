#pragma once
class MapScene : public Scene
{
	ObTileMap* map;

	Vector2 movedir;
public:

	MapScene();
	~MapScene();

	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

