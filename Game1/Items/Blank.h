#pragma once
//��ź�� �浹�� ��ź �����
class Blank : public Item
{

public:
	ObCircle* range;

	bool abilityAct;
	float abilityTimer;

public:
	Blank();
	~Blank();

	void Active(GameObject* obj);
	virtual void Update();
	virtual void Render();
};

