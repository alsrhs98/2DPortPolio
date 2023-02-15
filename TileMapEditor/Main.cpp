#include "stdafx.h"
#include "Main.h"



void Main::Init()
{
	map = new ObTileMap();
	map->scale = Vector2(12.0f, 12.0f);
	map->SetWorldPos(Vector2(-App.GetHalfWidth(), -App.GetHalfHeight()));

	LIGHT->light.radius = 3000.0f;

	imgIdx = 0;
	tileSize = Int2(80, 80);
	tileColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	tileState = 0;

	mtree = new MAPTREE();
	mtree->LB = Int2(0, 0);
	mtree->RT = tileSize;
	mtree->level = 1;
	mtree->parent = nullptr;
	mtree->connPoint = Int2(0, 0);
}

void Main::Release()
{
	SafeDelete(map);
	//Delete(mtree);
	SafeDelete(mtree);
}
void Main::DivideArea(MAPTREE* mtree)
{
	static int i = 1;
	int rowScale = mtree->RT.x - mtree->LB.x;
	int colScale = mtree->RT.y - mtree->LB.y;

	if (rowScale < MAXTILENUM or colScale < MAXTILENUM)
	{
		return;
	}
	
	float divPercent = RANDOM->Int(4, 6) * 0.1;

	// 임의의 지점
	Int2 pointIndex;

	//세로자르기 (가로로 연결 rightX ,  ,y)
	if (rowScale / colScale >= 1)
	{
		//나누는 분기점
		pointIndex.x = mtree->LB.x + rowScale * divPercent;
		pointIndex.y = mtree->RT.y;

		mtree->left = new MAPTREE();
		mtree->left->LB = mtree->LB;
		mtree->left->RT = pointIndex;
		mtree->left->parent = mtree;

		mtree->right = new MAPTREE();
		mtree->right->LB = Int2(pointIndex.x, mtree->LB.y);
		mtree->right->RT = mtree->RT;
		mtree->right->parent = mtree;


		mtree->left->rc = RC::COL;
		mtree->right->rc = RC::COL;

	}
	//가로자르기 (세로로 연결 y1, y2, x)
	else
	{
		pointIndex.x = mtree->LB.x;
		pointIndex.y = mtree->LB.y + colScale * divPercent;
		
		mtree->left = new MAPTREE();
		mtree->left->LB = mtree->LB;
		mtree->left->RT = Int2(mtree->RT.x, pointIndex.y);
		mtree->left->parent = mtree;

		mtree->right = new MAPTREE();
		mtree->right->LB = pointIndex;
		mtree->right->RT = mtree->RT;
		mtree->right->parent = mtree;

		mtree->left->rc = RC::ROW;
		mtree->right->rc = RC::ROW;

		
		/*
		cout << "pointIndex : " << pointIndex.x << ", " << pointIndex.y << endl;
		cout << "가로 자르기" << endl;
		cout << "right : " << right->LB.x << "," << right->LB.y << " -> "
			<< right->RT.x << "," << right->RT.y << endl;
		cout << "left : " << left->LB.x << "," << left->LB.y << " -> "
			<< left->RT.x << "," << left->RT.y << endl;
			*/

	}
	
	//cout<<"R : " << mtree->right->connPoint.x << ", " << mtree->right->connPoint.y << endl;
	//cout<<"L : " << mtree->left->connPoint.x << ", " << mtree->left->connPoint.y << endl;
	//리스트에 저장

	DivideArea(mtree->right);
	DivideArea(mtree->left);
}
void Main::bfs(MAPTREE* mt, int i)
{
	if (mt->level == i)
	{
		connList.push_back(mt);
	}
	if (mt->left != NULL)
		bfs(mt->left, i);

	if (mt->right != NULL)
		bfs(mt->right, i);
}
void Main::MakeRoom(vector<MAPTREE>&mtreeList)
{
	cout << "방 갯수 : " << mtreeList.size() << endl;
	for (int i = 0; i < mtreeList.size(); i++)
	{
		MAPTREE temp = mtreeList[i];

		// 방을 만들어 줄 랜덤한 위치 계산
		//LB
		int randX1 = RANDOM->Int(temp.LB.x + 1, temp.LB.x + 2);
		int randY1 = RANDOM->Int(temp.LB.y + 1, temp.LB.y + 2);
		//RT
		int randX2 = RANDOM->Int(temp.RT.x - 3, temp.RT.x - 2);
		int randY2 = RANDOM->Int(temp.RT.y - 3, temp.RT.y - 2);
		//mtreeList LB, RT 업데이트
		mtreeList[i].LB = Int2(randX1, randY1);
		mtreeList[i].RT = Int2(randX2, randY2);
		

		//cout <<i+1<<". "<< temp.LB.x << "," << temp.LB.y << "->" << temp.RT.x << "," << temp.RT.y << endl;
	}
}
void Main::ConnectTile(Int2 start, Int2 end, ObTileMap* map)
{
	cout << "연결" << endl;
	

}

void Main::Connecting(vector<MAPTREE*> connList, ObTileMap* map)
{

	vector<MAPTREE*>::iterator iter;
	
	iter = connList.end();
	MAPTREE* leftN = *(iter - 2);
	MAPTREE* rightN = *(iter - 1);

	int min = 0;
	int max = 0;

	if (leftN->connPoint.x == rightN->connPoint.x)
	{
		min = min(leftN->connPoint.y, rightN->connPoint.y);
		max = max(leftN->connPoint.y, rightN->connPoint.y);
		for (int i = min; i < max; i++)
		{
			map->SetTile(Int2(leftN->connPoint.x, i), Int2(1, 2));
			map->Update();
		}
	}
	else if (leftN->connPoint.y == rightN->connPoint.y)
	{
		min = min(leftN->connPoint.x, rightN->connPoint.x);
		max = max(leftN->connPoint.x, rightN->connPoint.x);
		for (int i = min; i < max; i++)
		{
			map->SetTile(Int2( i, leftN->connPoint.y), Int2(1, 2));
			map->Update();
		}
	}

	connList.pop_back();
	connList.pop_back();
	if (connList.size() <= 1)return;
	Connecting(connList,map);

}

void Main::MakeConnPoint(MAPTREE* leftNode, MAPTREE* rightNode)
{
	if (leftNode->haveSubTree() and rightNode->haveSubTree())
	{
		cout << "서브트리 있음 : " << endl;

		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL)//두 노드가 세로분할 되어 있을때,
		{
			if (leftNode->right->rc == RC::COL) // 1) left노드의 right가 세로 분할로 되어 있을때,
			{
				if (rightNode->right->rc == RC::COL) // 1 - 1) right노드의 right가 세로 분할로 되어 있을때,
				{
					leftNode->connPoint = leftNode->left->midPoint();
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = leftNode->connPoint.y;
				}
				else								 // 1 - 2) right노드의 right가 가로 분할로 되어 있을때,
				{
					rightNode->connPoint = rightNode->right->midPoint();
					leftNode->connPoint.x = leftNode->midPoint().x;
					leftNode->connPoint.y = rightNode->connPoint.y;
				}
			}
			else							   // 2) left노드의 right가 가로 분할로 되어 있을때, 
			{
				if (rightNode->right->rc==RC::COL) // 2 - 1) right노드의 right가 세로 분할로 되어 있을때,
				{
					leftNode->connPoint = leftNode->right->midPoint();
					rightNode->connPoint.x = rightNode->left->midPoint().x;
					rightNode->connPoint.y = leftNode->connPoint.y;
				}
				else                               // 2 - 2) right노드의 right가 가로 분할로 되어 있을때,
				{
					leftNode->connPoint = leftNode->right->midPoint();
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = leftNode->connPoint.y;
				}
			}
		}
		else //두 노드가 가로분할 되어 있을때,
		{
			if (leftNode->right->rc == RC::COL) // 1) left노드의 right가 세로 분할로 되어 있을때,
			{
				if (rightNode->right->rc == RC::COL) // 1 - 1) right노드의 right가 세로 분할로 되어 있을때,
				{
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = rightNode->right->midPoint().y;
					leftNode->connPoint.y = leftNode->midPoint().y + 3;
					leftNode->connPoint.x = rightNode->connPoint.x;
				}
				else								 // 1 - 2) right노드의 right가 가로 분할로 되어 있을때,
				{
					leftNode->connPoint = leftNode->right->midPoint();
					rightNode->connPoint.y = rightNode->left->midPoint().y;
					rightNode->connPoint.x = leftNode->connPoint.x;
				}
			}
			else							   // 2) left노드의 right가 가로 분할로 되어 있을때, 
			{
				if (rightNode->right->rc == RC::COL) // 2 - 1) right노드의 right가 세로 분할로 되어 있을때,
				{
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = rightNode->right->midPoint().y;
					leftNode->connPoint.y = leftNode->right->midPoint().y +3;
					leftNode->connPoint.x = rightNode->connPoint.x;
				}
				else                               // 2 - 2) right노드의 right가 가로 분할로 되어 있을때,
				{
					rightNode->connPoint = rightNode->left->midPoint();
					leftNode->connPoint.y = leftNode->right->midPoint().y;
					leftNode->connPoint.x = rightNode->connPoint.x;
				}
			}
		}

	}
	// 한쪽 노드에만 SubTree 가 있을 경우
	else if (leftNode->haveSubTree() and !rightNode->haveSubTree()) // 왼쪽 노드에 서브트리가 있을 경우
	{
		cout << "왼쪽노드에 만 있음 : " << endl;

		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL) // 1) 세로분할일 상태
		{
			if (leftNode->right->rc == RC::COL)
			{
				leftNode->connPoint = leftNode->right->midPoint();
				rightNode->connPoint.x = rightNode->midPoint().x;
				rightNode->connPoint.y = leftNode->connPoint.y;
			}
			else
			{
				leftNode->connPoint = leftNode->right->midPoint();
				rightNode->connPoint.x = rightNode->midPoint().x;
				rightNode->connPoint.y = leftNode->connPoint.y;
			}
		}
		else													  // 2) 가로분할일 상태
		{
			if (leftNode->right->rc == RC::COL)
			{
				leftNode->connPoint = leftNode->right->midPoint();
				rightNode->connPoint.x = leftNode->connPoint.x;
				rightNode->connPoint.y = rightNode->midPoint().y;
			}
			else
			{
				leftNode->connPoint = leftNode->right->midPoint();
				rightNode->connPoint.x = leftNode->connPoint.x;
				rightNode->connPoint.y = rightNode->midPoint().y;
			}
		}
	}
	else if (!leftNode->haveSubTree() and rightNode->haveSubTree()) // 오른쪽 노드에 서브트리가 있을 경우
	{
		cout << "오른쪽노드에 만 있음 : " << endl	;

		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL) // 1) 세로분할일 상태
		{
			if (rightNode->right->rc == RC::COL)
			{
				rightNode->connPoint = rightNode->left->midPoint();
				leftNode->connPoint.x = leftNode->midPoint().x;
				leftNode->connPoint.y = rightNode->connPoint.y;
			}
			else
			{
				rightNode->connPoint = rightNode->right->midPoint();
				leftNode->connPoint.x = leftNode->midPoint().x;
				leftNode->connPoint.y = rightNode->connPoint.y;
			}
		}
		else													  // 2) 가로분할일 상태
		{
			if (rightNode->right->rc == RC::COL)
			{
				rightNode->connPoint = rightNode->right->midPoint();
				leftNode->connPoint.x = rightNode->connPoint.x;
				leftNode->connPoint.y = leftNode->midPoint().y;
			}
			else
			{
				rightNode->connPoint = rightNode->left->midPoint();
				leftNode->connPoint.x = rightNode->connPoint.x;
				leftNode->connPoint.y = leftNode->midPoint().y;
			}
		}
	}
	// 두 노드 모두 서브트리가 없을때,
	else        
	{
		cout << "노드 없음 : " << endl;
		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL) // 1) 세로분할일 상태
		{
			int Y = leftNode->midPoint().y;

			leftNode->connPoint = Int2(leftNode->midPoint().x, Y);
			rightNode->connPoint = Int2(rightNode->midPoint().x, Y);

		}
		else
		{
			int X = leftNode->midPoint().x;

			leftNode->connPoint = Int2(X,leftNode->midPoint().y);
			rightNode->connPoint = Int2(X,rightNode->midPoint().y);
		}
	}
	if (leftNode->level == 1 and rightNode->level == 1)
	{
		MAPTREE* temp = leftNode;
		while (temp->right != NULL)
		{
			temp = temp->right;
		}
		leftNode->connPoint = temp->midPoint();

		rightNode->connPoint.y = leftNode->connPoint.y;
		temp = rightNode;
		while (temp->left != NULL)
		{
			temp = temp->left;
		}
		rightNode->connPoint.x = temp->midPoint().x;
		
	}
	cout << "L : " << leftNode->connPoint.x << ", " << leftNode->connPoint.y << endl;
	cout << "R : " << rightNode->connPoint.x << ", " << rightNode->connPoint.y << endl;
}

void Main::AutomaticMap()
{
	//맵 정리
	map->ResizeTile(Int2(80, 80));

	for (int i = 0; i < tileSize.x; i++)
	{
		for (int j = 0; j < tileSize.y; j++)
		{
			map->SetTile(Int2(i, j), Int2(3, 3), 0, TILE_NONE ,Color(0, 0, 0, 1));
		}
	}
	//영역 나누기
	DivideArea(mtree);
	
	//맨 마지막 노드 찾기
	preorder(mtree);

	//마지막 노드를 이용하여 방 생성
	MakeRoom(mtreeList);

	//bfs 순회로 정리
	int depth = MaxDepth(mtree)+1;
	for (int i = 0; i < depth; i++)
	{
		bfs(mtree, i);
	}
	
	//tree의 level 생성 및 max depth 찾기
	for (int i = 0; i < mtreeList.size(); i++)
	{
		MAPTREE temp = mtreeList[i];
 		//계산된 위치에 다 방 그리기
		for (int i = temp.LB.y; i < temp.RT.y; i++)
		{
			for (int j = temp.LB.x; j < temp.RT.x; j++)
			{
				Int2 setTilePoint = Int2(j, i);
				map->SetTile(setTilePoint, FLOOR, 0, 0);

			}
		}
	}
	//연결 포인트 생성
	for (int i = connList.size() - 1; i > 1; i-=2)
	{
		cout << "LEVEL : " << connList[i]->level<<"  ";
		int next = i - 1;
		if (next < 1)break;
		MakeConnPoint(connList[next],connList[i]);
	}
	Connecting(connList, map);

	map->Update();
}



void Main::Update()
{
	//CAM
	if (INPUT->KeyPress('W'))
	{
		CAM->position += UP * 300.0f * DELTA;
	}																		 
	if (INPUT->KeyPress('S'))
	{
		CAM->position += DOWN * 300.0f * DELTA;
	}
	if (INPUT->KeyPress('A'))
	{
		CAM->position += LEFT * 300.0f * DELTA;
	}
	if (INPUT->KeyPress('D'))
	{
		CAM->position += RIGHT * 300.0f * DELTA;
	}

	//Gui
	if (ImGui::Button("ErrorFileSystem?->Click me"))
	{
		ImGuiFileDialog::Instance()->Close();
	}
	 
	//TileSize
	if (ImGui::SliderInt2("TileSize", (int *)&tileSize, 1, 100))
	{
		map->ResizeTile(tileSize);
	}

	//TileScale
	ImGui::SliderFloat2("TileScale", (float *)&map->scale, 1.0f, 200.0f);

	//TilePos
	Vector2 pos = map->GetWorldPos();
	if (ImGui::SliderFloat2("TilePos", (float *)&pos, -1000.0f, 1000.0f))
	{
		map->SetWorldPos(pos);
	}

	//TileState
	ImGui::SliderInt("TileState", &tileState, TILE_NONE, TILE_SIZE);
	//TileColor
	ImGui::ColorEdit4("TileColor", (float *)&tileColor, ImGuiColorEditFlags_PickerHueWheel);

	
	//texture0~3	
	/*if (GUI->FileImGui("Texture0", "Texture0",
		".jpg,.png,.bmp,.dds,.tga", "../Contents/Images"))
	{
		string path = ImGuiFileDialog::Instance()->GetFilePathName();
		Util::Replace(&path, "\\", "/");
		size_t tok = path.find("/Images/") + 8;
		path = path.substr(tok, path.length());
		SafeDelete(map->tileImages[0]);
		wstring wImgFile = L"";
		wImgFile.assign(path.begin(), path.end());
		map->tileImages[0] = new ObImage(wImgFile);
	}
	ImGui::SameLine();
	if (GUI->FileImGui("Texture1", "Texture1",
		".jpg,.png,.bmp,.dds,.tga", "../Contents/Images"))
	{
		string path = ImGuiFileDialog::Instance()->GetFilePathName();
		Util::Replace(&path, "\\", "/");
		size_t tok = path.find("/Images/") + 8;
		path = path.substr(tok, path.length());
		SafeDelete(map->tileImages[1]);
		wstring wImgFile = L"";
		wImgFile.assign(path.begin(), path.end());
		map->tileImages[1] = new ObImage(wImgFile);
	}
	ImGui::SameLine();
	if (GUI->FileImGui("Texture2", "Texture2",
		".jpg,.png,.bmp,.dds,.tga", "../Contents/Images"))
	{
		string path = ImGuiFileDialog::Instance()->GetFilePathName();
		Util::Replace(&path, "\\", "/");
		size_t tok = path.find("/Images/") + 8;
		path = path.substr(tok, path.length());
		SafeDelete(map->tileImages[2]);
		wstring wImgFile = L"";
		wImgFile.assign(path.begin(), path.end());
		map->tileImages[2] = new ObImage(wImgFile);
	}
	ImGui::SameLine();*/
	for (int i = 0; i < 4; i++)
	{
		string str ="Texture" + to_string(i);
		if (GUI->FileImGui(str.c_str(), str.c_str(),
			".jpg,.png,.bmp,.dds,.tga", "../Contents/Images"))
		{
			string path = ImGuiFileDialog::Instance()->GetFilePathName();
			Util::Replace(&path, "\\", "/");
			size_t tok = path.find("/Images/") + 8;
			path = path.substr(tok, path.length());
			SafeDelete(map->tileImages[i]);
			wstring wImgFile = L"";
			wImgFile.assign(path.begin(), path.end());
			map->tileImages[i] = new ObImage(wImgFile);
		}
		if (i < 3)
		{
			ImGui::SameLine();
		}
	}
	
	//"감바스";
	//L"감바스";
	//ImgIdx
	if (ImGui::InputInt("ImgIdx", &imgIdx))
	{
		imgIdx = Util::saturate(imgIdx, 0, 3);

		if (not map->tileImages[imgIdx])
		{
			imgIdx = 0;
		}
	}
	//maxFrame
	ImGui::InputInt2("maxFrame", (int*)&map->tileImages[imgIdx]->maxFrame);
	
	//ImgButton
	Int2 MF = map->tileImages[imgIdx]->maxFrame;
	ImVec2 size;
	size.x = 300.0f / (float)MF.x;
	size.y = 300.0f / (float)MF.y;
	ImVec2 LT, RB;
	int index = 0;
	for (UINT i = 0; i < MF.y; i++)
	{
		for (UINT j = 0; j < MF.x; j++)
		{
			if (j != 0)
			{
				//같은줄에 배치
				ImGui::SameLine();
			}
			//텍스쳐 좌표
			LT.x = 1.0f / MF.x * j;
			LT.y = 1.0f / MF.y * i;
			RB.x = 1.0f / MF.x * (j + 1);
			RB.y = 1.0f / MF.y * (i + 1);

			ImGui::PushID(index);
			if (ImGui::ImageButton((void*)map->tileImages[imgIdx]->GetSRV()
				, size, LT, RB))
			{
				pickingIdx.x = j;
				pickingIdx.y = i;
			}
			index++;
			ImGui::PopID();
		}
	}
	
	//SaveLoad
	if (GUI->FileImGui("Save", "Save Map",
		".txt", "../Contents/TileMap"))
	{
		string path = ImGuiFileDialog::Instance()->GetFilePathName();
		Util::Replace(&path, "\\", "/");
		size_t tok = path.find("/TileMap/") + 9;
		path = path.substr(tok, path.length());
		map->file = path;
		map->Save();
	}
	ImGui::SameLine();
	if (GUI->FileImGui("Load", "Load Map",
		".txt", "../Contents/TileMap"))
	{

		string path = ImGuiFileDialog::Instance()->GetFilePathName();
		
		//path.find
		//path.length();
		//path.push_back('a');
		//path.find()
		Util::Replace(&path, "\\", "/");
		size_t tok = path.find("/TileMap/") + 9;
		path = path.substr(tok, path.length());
		map->file = path;
		map->Load();
		tileSize = map->GetTileSize();
	}
	if (ImGui::Button("Auto"))
	{
		AutomaticMap();
	}
	//Coord
	ImGui::Text("mouseIdx:: %d , %d", mouseIdx.x, mouseIdx.y);
	ImGui::Text("pickingIdx: %d , %d", pickingIdx.x, pickingIdx.y);
	ImGui::Text("imgIdx: %d ", imgIdx);

	//Brush
	ImVec2 min = ImGui::GetWindowPos();
	ImVec2 max;
	max.x = min.x + ImGui::GetWindowSize().x;
	max.y = min.y + ImGui::GetWindowSize().y;
	if (!ImGui::IsMouseHoveringRect(min, max))
	{
		if (INPUT->KeyPress(VK_LBUTTON))
		{
			if (map->WorldPosToTileIdx(INPUT->GetMouseWorldPos(), mouseIdx))
			{
				map->SetTile(mouseIdx, pickingIdx, imgIdx, tileState,tileColor);
			}
		}
	}

	map->Update();
}

void Main::LateUpdate()
{
}
void Main::Render()
{
	map->Render();
}

void Main::ResizeScreen()
{

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"TileMapEditor");
	App.SetInstance(instance);
	App.InitWidthHeight(1400.0f, 800.0f);
	Main * main = new Main();
	int wParam = (int)WIN->Run(main);
	WIN->DeleteSingleton();
	SafeDelete(main);
	return wParam;
}
