#include "stdafx.h"

Blank::Blank()
{
	col = new ObRect();
	col->scale = Vector2(12, 12) * 3;
	col->isFilled = false;

	img = new ObImage(L"Blank.png");
	img->scale = Vector2(12, 12) * 3;
	img->SetParentRT(*col);

	range = new ObCircle();
	range->scale = Vector2(30, 30);

	abilityAct = false;
	abilityTimer = 0.0f;

	IsObtain = false;
}

Blank::~Blank()
{
	SafeDelete(col);
	SafeDelete(img);
}

void Blank::Active(GameObject* obj)
{
	if (range->Intersect(obj))
	{
		obj->visible = false;
	}
}


void Blank::Update()
{
	if (IsObtain)
	{
		col->space = SPACE::SCREEN;
		img->space = SPACE::SCREEN;
		
	}
	else
	{
		col->space = SPACE::WORLD;
		img->space = SPACE::WORLD;
	}

	range->Update();
	col->Update();
	img->Update();
}

void Blank::Render()
{
	range->Render();
	col->Render();
	img->Render();
}
