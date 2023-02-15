#pragma once
class IntroScene : public Scene
{
private:

	ObImage* introScene;


	ObRect* playRect;
	ObImage* play;


	ObRect* titleRect;
	ObImage* title;

public:
	IntroScene();
	~IntroScene();



	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

