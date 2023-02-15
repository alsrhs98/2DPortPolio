#include "stdafx.h"

IntroScene::IntroScene()
{
	introScene = new ObImage(L"intro10.png");
	introScene->scale = Vector2(720, 480)*2.0f;
	introScene->maxFrame = Int2(10, 1);

	playRect = new ObRect();
	playRect->scale = Vector2(100, 50);
	playRect->SetWorldPos(Vector2(-App.GetHalfWidth()*0.8, -App.GetHalfHeight()*0.7));
	playRect->isFilled = false;


	play = new ObImage(L"PLAY.png");
	play->scale = playRect->scale;
	play->SetParentRT(*playRect);


	title = new ObImage(L"enterthgGungeon.png");
	title->scale = Vector2(830, 280);



	
}

IntroScene::~IntroScene()
{
	SafeDelete(introScene);
	SafeDelete(playRect);
	SafeDelete(play);
	SafeDelete(title);

}

void IntroScene::Init()
{
}

void IntroScene::Release()
{
}

void IntroScene::Update()
{
	if (playRect->Intersect(INPUT->GetMouseWorldPos()) and
		INPUT->KeyPress(VK_LBUTTON))
	{
		playRect->color = Color(1, 1, 0, 0.5f);
		SCENE->ChangeScene("SC0", 0.5f);
	}
	else
	{
		playRect->color = Color(1, 1, 1,1);
	}

	if (INPUT->KeyDown('U'))
	{
	
		SCENE->ChangeScene("SC1", 0.5f);
	}
	if (ImGui::Button("Mapedit"))
	{

		SCENE->ChangeScene("Map", 0.5f);
	}

	introScene->ChangeAnim(ANISTATE::LOOP, 0.2f);
	play->Update();
	playRect->Update();
	title->Update();
	introScene->Update();
}

void IntroScene::LateUpdate()
{
}

void IntroScene::Render()
{
	introScene->Render();
	playRect->Render();
	play->Render();
	title->Render();
}

void IntroScene::ResizeScreen()
{
}
