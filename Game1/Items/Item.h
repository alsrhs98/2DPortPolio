#pragma once
class Item
{	
protected:
	ObImage* img;


public:
	ObRect* col;
	bool IsObtain;

public:

	Vector2 GetScale() { return col->scale; };

	Vector2 GetPos() { return col->GetWorldPos(); };

	void SetPos(Vector2 pos) { col->SetWorldPos(pos); };
	virtual void Update();
	virtual void Render();
	//virtual void Active();
};

