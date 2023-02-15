#include "stdafx.h"

MapScene::MapScene()
{
	map = new ObTileMap();
	map->scale = Vector2(7, 7);

	movedir = Vector2(0, 0);
}

MapScene::~MapScene()
{
	SafeDelete(map);
}

void MapScene::Init()
{
	map->AutomaticMap(map->mtree);
	map->Update();
}

void MapScene::Release()
{
}

void MapScene::Update()
{
	if (INPUT->KeyPress('W'))
	{
		movedir.y++;
	}
	else if (INPUT->KeyPress('S'))
	{
		movedir.y--;
	}
	else
	{
		movedir.y = 0;
	}
	if (INPUT->KeyPress('A'))
	{
		movedir.x--;
	}
	else if (INPUT->KeyPress('D'))
	{
		movedir.x++;
	}
	else
	{
		movedir.x = 0;
	}
	movedir.Normalize();


	CAM->position += movedir * 100*DELTA;
}

void MapScene::LateUpdate()
{
}

void MapScene::Render()
{
	map->Render();
}

void MapScene::ResizeScreen()
{
}
