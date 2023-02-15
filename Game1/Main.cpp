#include "stdafx.h"
#include "Main.h"

void Main::Init()
{
	pl = new Player();
	pl->SetHp(5);

	mousePoint = new ObCircle();
	mousePoint->scale = Vector2(30, 30);
	mousePoint->visible = false;
	mouseImg = new ObImage(L"mouse.png");
	mouseImg->scale = mousePoint->scale;
	mouseImg->SetParentRT(*mousePoint);

	{
		IntroScene* temp = new IntroScene();
		SCENE->AddScene("INTRO", temp);
	}
	{
		Scene1 * temp = new Scene1();
		temp->pl = pl;
		SCENE->AddScene("SC0", temp);
	}

	{
		Scene2* sc2 = new Scene2();
		sc2->pl = pl;
		SCENE->AddScene("SC1", sc2);
	}
	{
		MapScene* map = new MapScene();
		SCENE->AddScene("Map", map);
	}

	SCENE->ChangeScene("INTRO");

}

void Main::Release()
{
	SafeDelete(pl);
	SafeDelete(mousePoint);
	SafeDelete(mouseImg);
	
}


void Main::Update()
{
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	
	mousePoint->SetWorldPos(INPUT->GetMouseWorldPos());
	
	if (pl->GetHp() == 0)
	{
		pl->isDying = true;
	}

	mousePoint->Update();
	mouseImg->Update();
	SCENE->Update();
}

void Main::LateUpdate()
{
	SCENE->LateUpdate();

}

void Main::Render()
{
	SCENE->Render();
	mousePoint->Render();
	mouseImg->Render();
}

void Main::ResizeScreen()
{

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"Game1");
	App.SetInstance(instance);
	App.InitWidthHeight(1400.0f, 700.0f);
	Main * main = new Main();
	int wParam = (int)WIN->Run(main);
	WIN->DeleteSingleton();
	SafeDelete(main);
	return wParam;
}