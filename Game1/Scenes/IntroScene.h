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
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

