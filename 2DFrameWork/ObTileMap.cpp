#include "framework.h"

using namespace std;

void Tile::ClearCost()
{
    //����� ���Ѱ����� �ʱ�ȭ
    F = G = H = INT_MAX;
    P = nullptr;
    isFind = false;
}

void Tile::ClacH(Int2 DestIdx)
{
    //���� ������������ �������� ����ض�
    int tempX = abs(idx.x - DestIdx.x) * 10;
    int tempY = abs(idx.y - DestIdx.y) * 10;

    H = tempX + tempY;
}

void Tile::ClacF()
{
    F = G + H;
}

ObTileMap::ObTileMap()
{
    tileSize.x = 20;
    tileSize.y = 20;
    file = "map1.txt";

    vertices = new VertexTile[tileSize.x * tileSize.y * 6];

    tileImages[0] = new ObImage(L"Tile.png");
    tileImages[0]->maxFrame = Int2(8, 6);
    tileImages[1] = new ObImage(L"Dungeon_Tileset.png");
    tileImages[1]->maxFrame = Int2(10, 10);
    tileImages[2] = nullptr;
    tileImages[3] = nullptr;

	mtree = new MAPTREE();
	mtree->LB = Int2(0, 0);
	mtree->RT = tileSize;
	mtree->level = 1;
	mtree->parent = nullptr;
	mtree->connPoint = Int2(0, 0);


    //����
    for (int i = 0; i < tileSize.y; i++)
    {
        //����
        for (int j = 0; j < tileSize.x; j++)
        {
            //Ÿ����ǥ ( tileSize.x * y��ǥ + x��ǥ)
            //������ ��ǥ ( tileSize.x * y��ǥ + x��ǥ) * 6
            int tileIdx = tileSize.x * i + j;
            //0
            vertices[tileIdx * 6].position.x = 0.0f + j;
            vertices[tileIdx * 6].position.y = 0.0f + i;
            vertices[tileIdx * 6].uv = Vector2(0.0f, 1.0f);
            //1                             
            vertices[tileIdx * 6 + 1].position.x = 0.0f + j;
            vertices[tileIdx * 6 + 1].position.y = 1.0f + i;
            vertices[tileIdx * 6 + 1].uv = Vector2(0.0f, 0.0f);
            //2                             
            vertices[tileIdx * 6 + 2].position.x = 1.0f + j;
            vertices[tileIdx * 6 + 2].position.y = 0.0f + i;
            vertices[tileIdx * 6 + 2].uv = Vector2(1.0f, 1.0f);
            //3
            vertices[tileIdx * 6 + 3].position.x = 1.0f + j;
            vertices[tileIdx * 6 + 3].position.y = 1.0f + i;
            vertices[tileIdx * 6 + 3].uv = Vector2(1.0f, 0.0f);
            //4
            vertices[tileIdx * 6 + 4].position.x = 1.0f + j;
            vertices[tileIdx * 6 + 4].position.y = 0.0f + i;
            vertices[tileIdx * 6 + 4].uv = Vector2(1.0f, 1.0f);
            //5
            vertices[tileIdx * 6 + 5].position.x = 0.0f + j;
            vertices[tileIdx * 6 + 5].position.y = 1.0f + i;
            vertices[tileIdx * 6 + 5].uv = Vector2(0.0f, 0.0f);
        }
    }
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexTile) * tileSize.x * tileSize.y * 6;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        Check(hr);
    }

}

ObTileMap::~ObTileMap()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);
	SafeDelete(mtree);
    for (int i=0; i<4; i++)
        SafeDelete(tileImages[i]);
}

void ObTileMap::Render()
{
    if (!visible)return;
    GameObject::Render();

    for (int i = 0; i < 4; i++)
    {
        if (tileImages[i])
        {
            D3D->GetDC()->PSSetShaderResources(i, 1, &tileImages[i]->SRV);
            D3D->GetDC()->PSSetSamplers(i, 1, &tileImages[i]->sampler);
        }
    }

    tileMapShader->Set();

    UINT stride = sizeof(VertexTile);
    UINT offset = 0;

    D3D->GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    D3D->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D->GetDC()->Draw(tileSize.x * tileSize.y * 6, 0);
}

void ObTileMap::SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx, int TileState, Color color)
{
    int tileIdx = tileSize.x * TileIdx.y + TileIdx.x;
    vertices[tileIdx * 6 + 0].uv.x = FrameIdx.x / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 1].uv.x = FrameIdx.x / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 5].uv.x = FrameIdx.x / (float)tileImages[ImgIdx]->maxFrame.x;

    vertices[tileIdx * 6 + 2].uv.x = (FrameIdx.x + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 3].uv.x = (FrameIdx.x + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.x;
    vertices[tileIdx * 6 + 4].uv.x = (FrameIdx.x + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.x;

    vertices[tileIdx * 6 + 3].uv.y = FrameIdx.y / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 1].uv.y = FrameIdx.y / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 5].uv.y = FrameIdx.y / (float)tileImages[ImgIdx]->maxFrame.y;

    vertices[tileIdx * 6 + 2].uv.y = (FrameIdx.y + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 0].uv.y = (FrameIdx.y + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.y;
    vertices[tileIdx * 6 + 4].uv.y = (FrameIdx.y + 1.0f) / (float)tileImages[ImgIdx]->maxFrame.y;
    
    for (int i = 0; i < 6; i++)
    {
        vertices[tileIdx * 6 + i].tileMapIdx = ImgIdx;
        vertices[tileIdx * 6 + i].color = color;
        vertices[tileIdx * 6 + i].tileState = TileState;
    }
    
    
    D3D->GetDC()->UpdateSubresource
    (vertexBuffer, 0, NULL, vertices, 0, 0);
}

int ObTileMap::GetTileState(Int2 TileIdx)
{
    int tileIdx = tileSize.x * TileIdx.y + TileIdx.x;
    return vertices[tileIdx * 6].tileState;
}

Vector2 ObTileMap::GetTilePosition(Int2 TileIdx)
{
    int tileIdx = tileSize.x * TileIdx.y + TileIdx.x;
    return Vector2(vertices[tileIdx * 6].position.x, vertices[tileIdx * 6].position.y);
}

bool ObTileMap::WorldPosToTileIdx(Vector2 WPos, Int2 & TileIdx)
{
    WPos -= GetWorldPos();
    Vector2 tileCoord;
    tileCoord.x = WPos.x / scale.x;
    tileCoord.y = WPos.y / scale.y;
    if ((tileCoord.x < 0) or (tileCoord.y < 0 ) or
        (tileCoord.x >= tileSize.x) or (tileCoord.y >= tileSize.y))
    {
        return false;
    }
    TileIdx.x = tileCoord.x;
    TileIdx.y = tileCoord.y;
    return true;
}

void ObTileMap::ResizeTile(Int2 TileSize)
{
    VertexTile* Vertices = new VertexTile[TileSize.x * TileSize.y * 6];
    //Init
    for (int i = 0; i < TileSize.y; i++)
    {
        //����
        for (int j = 0; j < TileSize.x; j++)
        {
            //Ÿ����ǥ ( tileSize.x * y��ǥ + x��ǥ)
            //������ ��ǥ ( tileSize.x * y��ǥ + x��ǥ) * 6
            int tileIdx = TileSize.x * i + j;
            //0
            Vertices[tileIdx * 6].position.x = 0.0f + j;
            Vertices[tileIdx * 6].position.y = 0.0f + i;
            Vertices[tileIdx * 6].uv = Vector2(0.0f, 1.0f);
            //1                             
            Vertices[tileIdx * 6 + 1].position.x = 0.0f + j;
            Vertices[tileIdx * 6 + 1].position.y = 1.0f + i;
            Vertices[tileIdx * 6 + 1].uv = Vector2(0.0f, 0.0f);
            //2                             
            Vertices[tileIdx * 6 + 2].position.x = 1.0f + j;
            Vertices[tileIdx * 6 + 2].position.y = 0.0f + i;
            Vertices[tileIdx * 6 + 2].uv = Vector2(1.0f, 1.0f);
            //3
            Vertices[tileIdx * 6 + 3].position.x = 1.0f + j;
            Vertices[tileIdx * 6 + 3].position.y = 1.0f + i;
            Vertices[tileIdx * 6 + 3].uv = Vector2(1.0f, 0.0f);
            //4
            Vertices[tileIdx * 6 + 4].position.x = 1.0f + j;
            Vertices[tileIdx * 6 + 4].position.y = 0.0f + i;
            Vertices[tileIdx * 6 + 4].uv = Vector2(1.0f, 1.0f);
            //5
            Vertices[tileIdx * 6 + 5].position.x = 0.0f + j;
            Vertices[tileIdx * 6 + 5].position.y = 1.0f + i;
            Vertices[tileIdx * 6 + 5].uv = Vector2(0.0f, 0.0f);
        }
    }
    
    //Copy
    Int2 Min = Int2(min(TileSize.x, tileSize.x), min(TileSize.y, tileSize.y));
    for (int i = 0; i < Min.y; i++)
    {
        for (int j = 0; j < Min.x; j++)
        {
            int SrcIdx = tileSize.x * i + j;
            int DestIdx = TileSize.x * i + j;
            for (int k = 0; k < 6; k++)
            {
                Vertices[DestIdx * 6 + k] = vertices[SrcIdx * 6 + k];
            }
        }
    }
    SafeDeleteArray(vertices);
    vertices = Vertices;
    tileSize = TileSize;
    SafeRelease(vertexBuffer);
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexTile) * tileSize.x * tileSize.y * 6;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        Check(hr);
    }
}

void ObTileMap::Save()
{
    ofstream fout;
    string path = "../Contents/TileMap/" + file;
    fout.open(path.c_str(), ios::out);
    if (fout.is_open())
    {
        for (int i = 0; i < 4; i++)
        {
            fout << i << " ";
            if (tileImages[i])
            {
                string imgFile = "";
                imgFile.assign(tileImages[i]->file.begin(), tileImages[i]->file.end());
                fout << imgFile << " ";
                fout << tileImages[i]->maxFrame.x << " " << tileImages[i]->maxFrame.y << endl;
            }
            else
            {
                fout << "N" << endl;
            }
        }
        fout << "TileSize " << tileSize.x << " " << tileSize.y << endl;
        fout << "TileScale " << scale.x << " " << scale.y << endl;
        fout << "TilePosition " << GetWorldPos().x << " " << GetWorldPos().y << endl;
        
        for (int i = 0; i < tileSize.y; i++)
        {
            //����
            for (int j = 0; j < tileSize.x; j++)
            {
                int tileIdx = tileSize.x * i + j;
                fout << j << " " << i <<" "
                    << vertices[tileIdx * 6 + 1].uv.x << " " << vertices[tileIdx * 6 + 1].uv.y << " "
                    << vertices[tileIdx * 6 + 2].uv.x << " " << vertices[tileIdx * 6 + 2].uv.y << " "
                    << vertices[tileIdx * 6].color.x << " " << vertices[tileIdx * 6].color.y << " "
                    << vertices[tileIdx * 6].color.z << " " << vertices[tileIdx * 6].color.w << " "
                    << vertices[tileIdx * 6].tileMapIdx << " " << vertices[tileIdx * 6].tileState << endl;
            }
        }
        fout.close();
    }
}

void ObTileMap::Load()
{
    ifstream fin;
    string path = "../Contents/TileMap/" + file;
    fin.open(path.c_str(), ios::in);
    string temp;
    if (fin.is_open())
    {
        for (int i = 0; i < 4; i++)
        {
            SafeDelete(tileImages[i]);
            int idx; string Imgfile;
            fin >> idx;
            fin >> Imgfile;
            if (Imgfile != "N")
            {
                wstring wImgFile = L"";
                wImgFile.assign(Imgfile.begin(), Imgfile.end());
                tileImages[i] = new ObImage(wImgFile);
                fin >> tileImages[i]->maxFrame.x >> tileImages[i]->maxFrame.y;
            }
        }
        Int2 TileSize;
        fin>>temp >> TileSize.x >> TileSize.y;
        ResizeTile(TileSize);
        fin>>temp >> scale.x >> scale.y ;
        Vector2 pos;
        fin >> temp >> pos.x >> pos.y;
        SetWorldPos(pos);
      
        for (int i = 0; i < tileSize.y; i++)
        {
            //����
            for (int j = 0; j < tileSize.x; j++)
            {
                int temp; Vector2 MinUV, MaxUV; Color color; float tileMapIdx, tileMapState;
                fin >> temp >> temp >> MinUV.x >> MinUV.y >> MaxUV.x >> MaxUV.y
                    >> color.x >> color.y >> color.z >> color.w >> tileMapIdx >> tileMapState;

                int tileIdx = tileSize.x * i + j;

                vertices[tileIdx * 6].uv = Vector2(MinUV.x, MaxUV.y);
                vertices[tileIdx * 6 + 1].uv = Vector2(MinUV.x, MinUV.y);
                vertices[tileIdx * 6 + 2].uv = Vector2(MaxUV.x, MaxUV.y);
                vertices[tileIdx * 6 + 3].uv = Vector2(MaxUV.x, MinUV.y);
                vertices[tileIdx * 6 + 4].uv = Vector2(MaxUV.x, MaxUV.y);
                vertices[tileIdx * 6 + 5].uv = Vector2(MinUV.x, MinUV.y);
                for (int k = 0; k < 6; k++)
                {
                    vertices[tileIdx * 6 + k].color = color;
                    vertices[tileIdx * 6 + k].tileMapIdx = tileMapIdx;
                    vertices[tileIdx * 6 + k].tileState = tileMapState;
                }
            }
        }
        D3D->GetDC()->UpdateSubresource
        (vertexBuffer, 0, NULL, vertices, 0, 0);

        fin.close();
    }
}

void ObTileMap::CreateTileCost()
{
    for (int i = 0; i < Tiles.size(); i++)
    {
        Tiles[i].clear();
    }
    Tiles.clear();
    Tiles.resize(tileSize.x);
    for (int i = 0; i < Tiles.size(); i++)
    {
        Tiles[i].resize(tileSize.y);
    }
    Vector2 half = scale * 0.5f;
    Update();
    for (int i = 0; i < tileSize.x; i++)
    {
        for (int j = 0; j < tileSize.y; j++)
        {
            Tiles[i][j].idx = Int2(i, j);
            Tiles[i][j].state = GetTileState(Tiles[i][j].idx);
            Tiles[i][j].Pos.x = i * scale.x + GetWorldPos().x + half.x;
            Tiles[i][j].Pos.y = j * scale.y + GetWorldPos().y + half.y;
        }
    }
}

bool ObTileMap::PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way)
{
	//cout << "2. �н� ���ε� ����" << endl;
    //���߿� �ϳ��� ���̸� �� �� �ִ±��� ����.
    if (Tiles[dest.x][dest.y].state == TILE_WALL ||
        Tiles[sour.x][sour.y].state == TILE_WALL)
    {
        return false;
    }
    //������ �ִ� ���� ���� ����.
    way.clear();
    //����� ������ ������
    if (sour == dest)
    {
        return false;//���ڸ� ���߱�
    }
    //Ÿ�� ��� �ʱ�ȭ
    for (int i = 0; i < tileSize.x; i++)
    {
        for (int j = 0; j < tileSize.y; j++)
        {
            Tiles[i][j].ClearCost();
        }
    }
    //�켱���� ť
    /*Tile* a, b;
    if (a > b)
    {

    }
    priority_queue<char> List2;*/

    //F���� ������ �������ִ� ����Ʈ
    priority_queue<PTile, vector<PTile>, compare> List;
    //����Ʈ�� ������� �߰�
    Tile* pTemp = &Tiles[sour.x][sour.y];
    pTemp->G = 0;//����� �������� 0
    pTemp->ClacH(dest);//���������� ������ �����
    pTemp->ClacF(); //�ѿ����� �����
    List.push({ pTemp ,pTemp->F });
    //���������� ����� ���涧 ���� �ݺ�
    while (1)
    {
        //�����ߵ� F���� ���̻� ������
        if (List.empty())
        {
            return false;
        }

        //Ž����� �޾ƿ���
        PTile Temp = List.top();
        //Ž����Ͽ��� ����
        Temp.first->isFind = true;
        //������ ��������
        List.pop();

        //�������� �������̸� ����(��ã�� ��������)
        if (Temp.first->idx == dest)
        {
            break;
        }

        //���� Ÿ�� ���˻�
        vector<Int2> LoopIdx;

        //����Ÿ���� �����Ҷ�
        if (Temp.first->idx.x > 0)
        {
            LoopIdx.push_back(Int2(Temp.first->idx.x - 1,
                Temp.first->idx.y));
        }
        //����Ÿ���� �����Ҷ�
        if (Temp.first->idx.y > 0)
        {
            LoopIdx.push_back(Int2(Temp.first->idx.x,
                Temp.first->idx.y - 1));
        }
        //������Ÿ���� �����Ҷ�
        if (Temp.first->idx.x < tileSize.x - 1)
        {
            LoopIdx.push_back(Int2(Temp.first->idx.x + 1,
                Temp.first->idx.y));
        }
        //�Ʒ���Ÿ���� �����Ҷ�
        if (Temp.first->idx.y < tileSize.y - 1)
        {
            LoopIdx.push_back(Int2(Temp.first->idx.x,
                Temp.first->idx.y + 1));
        }
        //�����¿�Ÿ�� ���˻�
        for (int i = 0; i < LoopIdx.size(); i++)
        {
            Tile* loop =
                &Tiles[LoopIdx[i].x][LoopIdx[i].y];
            //���� �ƴҶ�
            if (loop->state != TILE_WALL)
            {
                //������ �����
                loop->ClacH(dest);
                //���� �������ִ� ����� Ŭ��
                if (loop->G > Temp.first->G + 10)
                {
                    //��밻��
                    loop->G = Temp.first->G + 10;
                    loop->ClacF();
                    //�����κ��� �������� ǥ��
                    loop->P = Temp.first;
                    //�� Ÿ���� ã���� ���� Ÿ���̸� ����Ʈ�� �߰�
                    if (!loop->isFind)
                    {
                        List.push({ loop,loop->F });
                    }
                }
            }
        }
        LoopIdx.clear();

        //�밢��4�� 




    }
    //�������� ������ �Ǿ�����
    Tile* p = &Tiles[dest.x][dest.y];
    //dest 4,1 -> 4,2 -> 4,3 -> 3,3 ->2,3-> 1,3 ->
    while (1)
    {
        way.emplace_back(p);
        p = p->P;
        if (p == nullptr)
        {
            break;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////
void ObTileMap::DivideArea(MAPTREE* mtree)
{
	static int i = 1;

	int rowScale = mtree->RT.x - mtree->LB.x;
	int colScale = mtree->RT.y - mtree->LB.y;

	if (rowScale < MAXTILENUM or colScale < MAXTILENUM)
	{
		return;
	}

	float divPercent = RANDOM->Int(4, 6) * 0.1;

	// ������ ����
	Int2 pointIndex;

	//�����ڸ��� (���η� ���� rightX ,  ,y)
	if (rowScale / colScale >= 1)
	{
		//������ �б���
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
	//�����ڸ��� (���η� ���� y1, y2, x)
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

	}

	//cout<<"R : " << mtree->right->connPoint.x << ", " << mtree->right->connPoint.y << endl;
	//cout<<"L : " << mtree->left->connPoint.x << ", " << mtree->left->connPoint.y << endl;
	//����Ʈ�� ����

	DivideArea(mtree->right);
	DivideArea(mtree->left);
}
void ObTileMap::bfs(MAPTREE* mt, int i)
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
void ObTileMap::MakeRoom(vector<MAPTREE> & mtreeList)
{
	//cout << "�� ���� : " << mtreeList.size() << endl;
	for (int i = 0; i < mtreeList.size(); i++)
	{
		MAPTREE temp = mtreeList[i];

		// ���� ����� �� ������ ��ġ ���
		//LB
		int randX1 = RANDOM->Int(temp.LB.x + 1, temp.LB.x + 2);
		int randY1 = RANDOM->Int(temp.LB.y + 2, temp.LB.y + 3);
		//RT
		int randX2 = RANDOM->Int(temp.RT.x - 3, temp.RT.x - 2);
		int randY2 = RANDOM->Int(temp.RT.y - 3, temp.RT.y - 3);
		//mtreeList LB, RT ������Ʈ
		mtreeList[i].LB = Int2(randX1, randY1);
		mtreeList[i].RT = Int2(randX2, randY2);


		//cout <<i+1<<". "<< temp.LB.x << "," << temp.LB.y << "->" << temp.RT.x << "," << temp.RT.y << endl;
	}
}
//void Main::ConnectTile(Int2 start, Int2 end, ObTileMap* map)
void ObTileMap::Connecting(vector<MAPTREE*> connList)
{
	//static int i = 0;
	vector<MAPTREE*>::iterator iter;

	iter = connList.end();
	MAPTREE* leftN = *(iter - 2);
	MAPTREE * rightN = *(iter - 1);

	int min = 0;
	int max = 0;
	Int2 randomFloor = RandomTile(Int2(1, 1), Int2(4, 3));
	if (leftN->connPoint.x == rightN->connPoint.x)
	{
		min = min(leftN->connPoint.y, rightN->connPoint.y);
		max = max(leftN->connPoint.y, rightN->connPoint.y);
		for (int i = min; i < max; i++)
		{
			SetTile(Int2(leftN->connPoint.x, i), randomFloor,1,TILE_FLOOR);
			Update();
		}
	}
	else if (leftN->connPoint.y == rightN->connPoint.y)
	{
		min = min(leftN->connPoint.x, rightN->connPoint.x);
		max = max(leftN->connPoint.x, rightN->connPoint.x);
		for (int i = min; i < max; i++)
		{
			SetTile(Int2(i, leftN->connPoint.y), randomFloor, 1, TILE_FLOOR);
			Update();
		}
	}

	connList.pop_back();
	connList.pop_back();
	//i++;
	//cout << i << "��° ���� �Ϸ�" << endl;
	if (connList.size() <= 1)return;
	Connecting(connList);

}
void ObTileMap::MakeConnPoint(MAPTREE * leftNode, MAPTREE * rightNode)
{
	Int2 connector[2];
	if (leftNode->haveSubTree() and rightNode->haveSubTree())
	{
		//cout << "����Ʈ�� ���� : " << endl;

		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL)//�� ��尡 ���κ��� �Ǿ� ������,
		{
			if (leftNode->right->rc == RC::COL) // 1) left����� right�� ���� ���ҷ� �Ǿ� ������,
			{
				if (rightNode->right->rc == RC::COL) // 1 - 1) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					leftNode->connPoint = leftNode->left->midPoint();
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = leftNode->connPoint.y;
				}
				else								 // 1 - 2) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					rightNode->connPoint = rightNode->right->midPoint();
					leftNode->connPoint.x = leftNode->midPoint().x;
					leftNode->connPoint.y = rightNode->connPoint.y;
				}
			}
			else							   // 2) left����� right�� ���� ���ҷ� �Ǿ� ������, 
			{
				if (rightNode->right->rc == RC::COL) // 2 - 1) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					leftNode->connPoint = leftNode->right->midPoint();
					rightNode->connPoint.x = rightNode->left->midPoint().x;
					rightNode->connPoint.y = leftNode->connPoint.y;
				}
				else                               // 2 - 2) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					leftNode->connPoint = leftNode->right->midPoint();
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = leftNode->connPoint.y;
				}
			}
		}
		else //�� ��尡 ���κ��� �Ǿ� ������,
		{
			if (leftNode->right->rc == RC::COL) // 1) left����� right�� ���� ���ҷ� �Ǿ� ������,
			{
				if (rightNode->right->rc == RC::COL) // 1 - 1) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = rightNode->right->midPoint().y;
					leftNode->connPoint.y = leftNode->midPoint().y + 3;
					leftNode->connPoint.x = rightNode->connPoint.x;
				}
				else								 // 1 - 2) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					leftNode->connPoint = leftNode->right->midPoint();
					rightNode->connPoint.y = rightNode->left->midPoint().y;
					rightNode->connPoint.x = leftNode->connPoint.x;
				}
			}
			else							   // 2) left����� right�� ���� ���ҷ� �Ǿ� ������, 
			{
				if (rightNode->right->rc == RC::COL) // 2 - 1) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					rightNode->connPoint.x = rightNode->right->midPoint().x;
					rightNode->connPoint.y = rightNode->right->midPoint().y;
					leftNode->connPoint.y = leftNode->right->midPoint().y + 3;
					leftNode->connPoint.x = rightNode->connPoint.x;
				}
				else                               // 2 - 2) right����� right�� ���� ���ҷ� �Ǿ� ������,
				{
					rightNode->connPoint = rightNode->left->midPoint();
					leftNode->connPoint.y = leftNode->right->midPoint().y;
					leftNode->connPoint.x = rightNode->connPoint.x;
				}
			}
		}

	}
	// ���� ��忡�� SubTree �� ���� ���
	// ���� ��忡 ����Ʈ���� ���� ���
	else if (leftNode->haveSubTree() and !rightNode->haveSubTree())
	{
		//cout << "���ʳ�忡 �� ���� : " << endl;

		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL) // 1) ���κ����� ����
		{
			if (leftNode->right->rc == RC::COL) //1 - 1) leftNode ���� ���� ����
			{
				leftNode->connPoint = leftNode->right->midPoint();
				rightNode->connPoint.x = rightNode->midPoint().x;
				rightNode->connPoint.y = leftNode->connPoint.y;
			}
			else //1 - 2) leftNode ���� ���� ����
			{
				leftNode->connPoint = leftNode->right->midPoint();
				rightNode->connPoint.x = rightNode->midPoint().x;
				rightNode->connPoint.y = leftNode->connPoint.y;
			}
		}
		else													  // 2) ���κ����� ����
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
	// ������ ��忡 ����Ʈ���� ���� ���
	else if (!leftNode->haveSubTree() and rightNode->haveSubTree()) 
	{
		//cout << "�����ʳ�忡 �� ���� : " << endl;

		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL) // 1) ���κ����� ����
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
		else													  // 2) ���κ����� ����
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
	// �� ��� ��� ����Ʈ���� ������,
	else
	{
		//cout << "��� ���� : " << endl;
		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL) // 1) ���κ����� ����
		{
			int Y = leftNode->midPoint().y;

			//leftNode->connPoint = Int2(leftNode->midPoint().x, Y);
			//rightNode->connPoint = Int2(rightNode->midPoint().x, Y);

			for (int x = leftNode->midPoint().x; x < rightNode->midPoint().x; x++)
			{
				connector[0] = Int2(x, Y);
				connector[1] = Int2(x + 1, Y);

				if (GetTileState(connector[1]) == TILE_NONE and GetTileState(connector[0]) == TILE_FLOOR)
				{
					leftNode->connPoint = connector[0];
				}

				if (GetTileState(connector[0]) == TILE_NONE and GetTileState(connector[1]) == TILE_FLOOR)
				{
					rightNode->connPoint = connector[1];
					break;
				}

			}
		}
		else
		{
			int X = leftNode->midPoint().x;

			//leftNode->connPoint = Int2(X, leftNode->midPoint().y);
			//rightNode->connPoint = Int2(X, rightNode->midPoint().y);
			for (int y = leftNode->midPoint().y; y < rightNode->midPoint().y; y++)
			{
				connector[0] = Int2(X, y);
				connector[1] = Int2(X , y+1);

				if (GetTileState(connector[1]) == TILE_NONE and GetTileState(connector[0]) == TILE_FLOOR)
				{
					leftNode->connPoint = connector[0];
				}

				if (GetTileState(connector[0]) == TILE_NONE and GetTileState(connector[1]) == TILE_FLOOR)
				{
					rightNode->connPoint = connector[1];
					break;
				}

			}
		}
	}

	{

	if (leftNode->level <= 1 and rightNode->level <= 1)
	{
		MAPTREE* temp1 = leftNode;
		MAPTREE* temp2 = rightNode;
		if (leftNode->rc == RC::COL and rightNode->rc == RC::COL)
		{
			temp1 = leftNode;
			while (temp1->right != NULL)
			{
				temp1 = temp1->right;
			}
			temp2 = rightNode;
			while (temp2->right != NULL)
			{
				temp2 = temp2->right;
			}
			
			if (temp1->rc == RC::COL)
			{
				if (temp2->rc == RC::COL)
				{
					leftNode->connPoint = temp1->midPoint();
					temp2 = temp2->parent->left;
					rightNode->connPoint.x = temp2->midPoint().x;
					rightNode->connPoint.y = temp1->midPoint().y;
				}
				else
				{
					rightNode->connPoint = temp2->midPoint();
					leftNode->connPoint.x = temp1->midPoint().x;
					leftNode->connPoint.y = rightNode->connPoint.y;
				}
			}
			else
			{
				if (temp2->rc == RC::COL) 
				{
					leftNode->connPoint = temp1->midPoint();
					rightNode->connPoint.x = temp2->midPoint().x;
					rightNode->connPoint.y = temp1->midPoint().y;
				}
				else
				{
					leftNode->connPoint = temp1->midPoint();
					temp2 = temp2->parent->left;
					rightNode->connPoint.x = temp2->midPoint().x;
					rightNode->connPoint.y = temp1->midPoint().y;
				}
			}


			
		}
		else
		{
			while (temp1->right != NULL)
			{
				temp1 = temp1->right;
			}
			leftNode->connPoint.y = temp1->midPoint().y;

			while (temp2->left != NULL)
			{
				temp2 = temp2->left;
			}
			rightNode->connPoint.y = temp2->LB.y + 3;

			if (temp1->rc == RC::COL)
			{
				leftNode->connPoint.x = temp1->midPoint().x;
				rightNode->connPoint.x = leftNode->connPoint.x;
			}
			else
			{
				leftNode->connPoint.x = temp1->RT.x - 4;
				rightNode->connPoint.x = leftNode->connPoint.x;
			}

			//cout << "L : " << leftNode->connPoint.x << ", " << leftNode->connPoint.y << endl;
			//cout << "R : " << rightNode->connPoint.x << ", " << rightNode->connPoint.y << endl;
		}
		
	}
	}

	//cout << "L : " << leftNode->connPoint.x << ", " << leftNode->connPoint.y << endl;
	//cout << "R : " << rightNode->connPoint.x << ", " << rightNode->connPoint.y << endl;
	
}
void ObTileMap::MakeConnPoint2(MAPTREE* leftNode, MAPTREE* rightNode)
{
	//cout << "�Լ� ����" << endl;
	MAPTREE* tempL = leftNode;
	MAPTREE* tempR = rightNode;

	Int2 connector[2];

	int roadCnt  = 0;
	int temp;

	//cout << "���۳��->";
	while (tempL->right != NULL)
	{
		//cout << "�ڽĳ��->";
		tempL = tempL->right;
	}

	//cout <<"������ ���" << endl;
	Int2 size = tempL->size();
	connector[0].x = tempL->RT.x;
	connector[0].y = tempL->RT.y-1;
	while (GetTileState(connector[0]) != TILE_FLOOR)
	{
		connector[0].x -= 1;
		connector[0].y -= 1;
	}
	SetTile(connector[0], Int2(4, 4), 1, TILE_FLOOR);
	//cout << connector[0].x << ", " << connector[0].y << endl;
	//���� �����ΰ�

	if (tempL->rc == RC::COL)
	{
		temp = 99;
		//cout << "���� ����" << endl;
		for (int y = tempL->midPoint().y+1; y < tempL->midPoint().y+size.y/3; y++)
		{
			connector[0].y = y;
			connector[1].y = y;
			roadCnt = 0;
			if (GetTileState(Int2(connector[0].x+1, y)) == TILE_FLOOR)
			{
				connector[0].x++;
				continue;
			}
			for (int x = connector[0].x + 1; x < tileSize.x; x++)
			{
				roadCnt++;
				connector[1].x = x;
				if (GetTileState(connector[1]) == TILE_FLOOR)
				{
					if (temp > roadCnt)
					{
						temp = roadCnt;
						leftNode->connPoint = connector[0];
						rightNode->connPoint = connector[1];
					}
				}
				else continue;
			}
		}
	}
	//���� �����ΰ�
	else
	{
		temp = 99;
		//cout << "���� ����" << endl;
		
		for (int x = tempL->midPoint().x-3; x < tempL->midPoint().x + size.x / 3; x++)
		{
			connector[0].x = x;
			connector[1].x = x;
			roadCnt = 0;
			if (GetTileState(Int2(x, connector[0].y)) == TILE_FLOOR)
			{
				connector[0].y++;
				continue;
			}
			
			for (int y = connector[0].y + 2; y < tileSize.y; y++)
			{
				roadCnt++;
				connector[1].y = y;
				if (GetTileState(connector[1]) == TILE_FLOOR)
				{
					if (temp > roadCnt)
					{
						temp = roadCnt;
						leftNode->connPoint = connector[0];
						rightNode->connPoint = connector[1];
					}
				}
				else continue;
			}
			
		}
		//cout<<"R : " << rightNode->connPoint.x << ", "<<rightNode->connPoint.y << endl;
		//cout<<"L : " << leftNode->connPoint.x << ", "<<leftNode->connPoint.y << endl;
	}

	if (rightNode->connPoint.x == 0 or leftNode->connPoint.x == 0)
	{
		
		temp = 99;
		if (leftNode->rc == RC::COL)
		{
			for (int y = leftNode->LB.y+size.y/3; y < leftNode->RT.y-size.y/3; y++)
			{
				connector[0].y = y;
				connector[1].y = y;
				roadCnt = 0;
				for (int x = leftNode->RT.x - size.x / 3; x <rightNode->midPoint().x; x++)
				{
					connector[1].x = x;
					roadCnt++;
					if (GetTileState(connector[1]) == TILE_FLOOR)
					{
						if (roadCnt < temp)
						{
							temp = roadCnt;
							rightNode->connPoint = connector[1];
							leftNode->connPoint = connector[0];
							break;
						}
					}

					
				}
		
			}
		}
		else
		{
			for (int x = leftNode->LB.x + size.x / 3; x < leftNode->RT.x - 2; x++)
			{
				connector[0].x = x;
				connector[1].x = x;
				roadCnt = 0;
				for (int y = leftNode->RT.y - size.y / 3; y < rightNode->midPoint().y; y++)
				{
					connector[1].y = y;
					roadCnt++;
					if (GetTileState(connector[1]) == TILE_FLOOR)
					{
						if (roadCnt < temp)
						{
							temp = roadCnt;
							rightNode->connPoint = connector[1];
							leftNode->connPoint = connector[0];
							break;
						}
					}


				}
			}
		}
	}
	
	

	//cout << "�Լ� ��" << endl;
	


}
void ObTileMap::MakeWall()
{
	//       x  y
	Int2 mat[2][2];

	for (int i = 0; i <tileSize.x-1; i++)
	{
		for (int j = 0; j < tileSize.y-1; j++)
		{
			//  x  y
			mat[0][0] = Int2(i, j);
			mat[0][1] = Int2(i, j+1);
			mat[1][0] = Int2(i+1, j);
			mat[1][1] = Int2(i+1, j+1);
			//���� Ÿ�� 
			Int2 randomUpWall	 = RandomTile(Int2(1,0),Int2(4,0));
			Int2 randomDownWall	 = RandomTile(Int2(1,4),Int2(4,4));
			Int2 randomLeftWall	 = RandomTile(Int2(0,0),Int2(0,3));
			Int2 randomRightWall = RandomTile(Int2(5,0),Int2(5,3));

			//�Ʒ� FLOOR ���� NONE
			if (GetTileState(mat[0][0]) == TILE_FLOOR and GetTileState(mat[1][0]) == TILE_FLOOR
				and GetTileState(mat[1][1]) != TILE_FLOOR and GetTileState(mat[0][1]) != TILE_FLOOR)
			{
				
				SetTile(mat[1][1], randomUpWall, 1, TILE_WALL);
				SetTile(mat[0][1], randomUpWall, 1, TILE_WALL);
			}
			else if (GetTileState(mat[0][0]) != TILE_FLOOR and GetTileState(mat[1][0]) != TILE_FLOOR
				and GetTileState(mat[1][1]) == TILE_FLOOR and GetTileState(mat[0][1]) == TILE_FLOOR)
			{
				SetTile(mat[0][0], randomDownWall, 1, TILE_WALL);
				SetTile(mat[1][0], randomDownWall, 1, TILE_WALL);
			}
			//���� FLOOR ������ NONE
			if (GetTileState(mat[0][1]) == TILE_FLOOR and GetTileState(mat[0][0]) == TILE_FLOOR
				and GetTileState(mat[1][1]) != TILE_FLOOR and GetTileState(mat[1][0]) != TILE_FLOOR)
			{
				SetTile(mat[1][1], randomRightWall, 1, TILE_WALL);
				SetTile(mat[1][0], randomRightWall, 1, TILE_WALL);
			}
			else if (GetTileState(mat[0][1]) != TILE_FLOOR and GetTileState(mat[0][0]) != TILE_FLOOR
				and GetTileState(mat[1][1]) == TILE_FLOOR and GetTileState(mat[1][0]) == TILE_FLOOR)
			{
				SetTile(mat[0][1], randomLeftWall, 1, TILE_WALL);
				SetTile(mat[0][0], randomLeftWall, 1, TILE_WALL);
			}

		}

	}
	for (int i = 0; i < tileSize.x - 1; i++)
	{
		for (int j = 0; j < tileSize.y - 1; j++)
		{
			//  x  y
			mat[0][0] = Int2(i, j);
			mat[0][1] = Int2(i, j + 1);
			mat[1][0] = Int2(i + 1, j);
			mat[1][1] = Int2(i + 1, j + 1);
			// 
			if (GetTileState(mat[0][0]) == TILE_WALL and GetTileState(mat[1][1]) == TILE_WALL)
			{
				if (GetTileState(mat[0][1]) == TILE_FLOOR)
				{
					SetTile(mat[1][0], LDCORNER, 1, TILE_WALL);
				}
				else if(GetTileState(mat[1][0]) == TILE_FLOOR)
				{
					SetTile(mat[0][1], RUCORNER, 1, TILE_WALL);
				}
			}
			else if (GetTileState(mat[0][1]) == TILE_WALL and GetTileState(mat[1][0]) == TILE_WALL)
			{
				if (GetTileState(mat[0][0]) == TILE_FLOOR)
				{
					SetTile(mat[1][1], RDCORNER, 1, TILE_WALL);
				}
				else if (GetTileState(mat[1][1]) == TILE_FLOOR)
				{
					SetTile(mat[0][0], LUCORNER, 1, TILE_WALL);
				}
			}
			
		}
	}

	for (int i = 0; i < tileSize.x - 1; i++)
	{
		for (int j = 0; j < tileSize.y - 1; j++)
		{
			//  x  y
			mat[0][0] = Int2(i, j);
			mat[0][1] = Int2(i, j + 1);
			mat[1][0] = Int2(i + 1, j);
			mat[1][1] = Int2(i + 1, j + 1);
			// 
			if (GetTileState(mat[0][0]) == TILE_WALL and 
				GetTileState(mat[1][0]) == TILE_WALL and
				GetTileState(mat[1][1]) == TILE_WALL)
			{
				if (GetTileState(mat[0][1]) == TILE_NONE)
				{
					SetTile(mat[1][0], UPWALL, 1, TILE_WALL);
				}
				
			}
			else if (GetTileState(mat[0][1]) == TILE_WALL and 
					 GetTileState(mat[1][0]) == TILE_WALL and 
					 GetTileState(mat[1][1]) == TILE_WALL  )
			{
				if (GetTileState(mat[0][0]) == TILE_NONE)
				{
					SetTile(mat[1][1],Int2(5,5) , 1, TILE_WALL);
				}
				
			}
			else if (GetTileState(mat[0][1]) == TILE_WALL and
					 GetTileState(mat[0][0]) == TILE_WALL and
					 GetTileState(mat[1][1]) == TILE_WALL)
			{
				if (GetTileState(mat[1][0]) == TILE_NONE)
				{
					SetTile(mat[0][1], Int2(0, 5), 1, TILE_WALL);
				}
			}
			
		}
	}



}
void ObTileMap::MakePiller(MAPTREE* room)
{
	for (int x = room->LB.x+4; x < room->RT.x-4; x+=3)
	{
		for (int y = room->LB.y+4; y <room->RT.y-4 ; y+=4)
		{
			int nan = RANDOM->Int(0, 5);
			if (GetTileState(Int2(x-1, y+1)) == TILE_FLOOR
				and GetTileState(Int2(x + 3, y - 1)) == TILE_FLOOR 
				and GetTileState(Int2(x + 3, y + 1)) == TILE_FLOOR
				and GetTileState(Int2(x - 1, y - 1)) == TILE_FLOOR
				and (nan==2))
			{
				SetPiller(Int2(x, y));
			}
		}
	}

}
void ObTileMap::MakeDoor(MAPTREE* room)
{
	Int2 colmat[3];
	Int2 rowmat[3];
	Door temp;
	for (int x =room->LB.x-1 ; x < room->RT.x+1; x++)
	{
		if (x > room->LB.x + 1 and x < room->RT.x - 1) continue;
			
		for (int y = room->LB.y - 1; y < room->RT.y + 2; y++)
		{
			
			colmat[0] = Int2(x, y);
			colmat[1] = Int2(x, y + 1);
			colmat[2] = Int2(x, y + 2);

			rowmat[0] = Int2(x, y);
			rowmat[1] = Int2(x + 1, y);
			rowmat[2] = Int2(x + 2, y);

			if (GetTileState(colmat[0]) == TILE_WALL and
				GetTileState(colmat[1]) == TILE_FLOOR and
				GetTileState(colmat[2]) == TILE_WALL)
			{
				temp.isCol = true;
				temp.isCollidable = false;
				temp.positon = colmat[1];
				DoorPos.push_back(temp);
				break;
			}

			if (GetTileState(rowmat[0]) == TILE_WALL and
				GetTileState(rowmat[1]) == TILE_FLOOR and
				GetTileState(rowmat[2]) == TILE_WALL)
			{
				temp.isCol = false;
				temp.isCollidable = false;
				temp.positon = rowmat[1];
				DoorPos.push_back(temp);
				break;
			}
		}
	}
}
void ObTileMap::ControlDoor(bool isOpen)
{
	Door* temp;
	for (int i = 0; i < DoorPos.size(); i++)
	{
		temp = &DoorPos[i];
		if (isOpen) // ���� ��������,
		{
			timer = 0;
			//cout << "������" << endl;
			SetTile(temp->positon, Int2(1, 1), 1, TILE_FLOOR);
		}
		else //���� ��������,
		{
			timer += 1.0f * DELTA;
			if (timer > 2.5f)
			{
				if (temp->isCol)//����
				{
					SetTile(temp->positon, Int2(6, 5), 1, TILE_WALL);
				}
				else //����
				{
					SetTile(temp->positon, Int2(6, 3), 1, TILE_WALL);
				}
			}
			
		}
	}
}
void ObTileMap::MakeResponRoom()
{
	MAPTREE temp;
	for (int i = 0; i < mtreeList.size()-1; i++)
	{
		for (int j = i+1; j < mtreeList.size(); j++)
		{
			if (mtreeList[i].tileWidth() > mtreeList[j].tileWidth())
			{
				temp = mtreeList[i];
				mtreeList[i] = mtreeList[j];
				mtreeList[j] = temp;
			}
		}
	}
	for (int i = 0; i < mtreeList.size(); i++)
	{
		Vector2 pos  = GetTilePosition(mtreeList[i].midPoint());
		//cout <<i+1<<" : " << pos.x << ", "<< pos.y << endl;
	}

}
Int2 ObTileMap::RandomTile(Int2 start, Int2 end)
{
	int x = RANDOM->Int(start.x, end.x);
	int y = RANDOM->Int(start.y, end.y);
	return {x,y};
}

void ObTileMap::SetPiller(Int2 point)
{
	Int2 temp;
	SetTile(Int2(point.x,point.y),	 Int2(0,5), 1, TILE_WALL);
	SetTile(Int2(point.x+1,point.y), Int2(1,5), 1, TILE_WALL);
	SetTile(Int2(point.x+2,point.y), Int2(2,5), 1, TILE_WALL);
	SetTile(Int2(point.x+3,point.y), Int2(3,5), 1, TILE_WALL);

	for (int i = 0; i < 4; i++)
	{
		for (int y = 1; y < 2; y++)
		{
			Int2 randomUpWall = RandomTile(Int2(1, 0), Int2(4, 0));
			SetTile(Int2(point.x+i,point.y-y), randomUpWall, 1, TILE_WALL);
		}
		
	}
	/*temp.x = point.x;
	temp.y -= 1;*/
	/*for (int x = 0; x < 4; x++)
	{
		temp.x = point.x + x;
		Int2 randomUpWall = RandomTile(Int2(1, 0), Int2(4, 0));
		for (int y = 0; y < 2; y++)
		{	
			temp.y = point.y - y;
			SetTile(temp, randomUpWall, 1, TILE_WALL);
		}
	}*/

	
}

void ObTileMap::AutomaticMap(MAPTREE* mtree)
{
	//�� ����
	ResizeTile(Int2(90, 80));
	
	mtree->LB = Int2(0, 0);
	mtree->RT = tileSize;

	for (int i = 0; i < tileSize.x; i++)
	{
		for (int j = 0; j < tileSize.y; j++)
		{
			SetTile(Int2(i, j), Int2(8, 7), 1, TILE_NONE);
		}
	}
	//���� ������
	DivideArea(mtree);
	cout << "���� ������ �Ϸ�" << endl;
	//�� ������ ��� ã��
	preorder(mtree);

	//������ ��带 �̿��Ͽ� �� ����
	MakeRoom(mtreeList);

	//bfs ��ȸ�� ����
	int depth = MaxDepth(mtree) + 1;
	for (int i = 0; i < depth; i++)
	{
		bfs(mtree, i);
	}

	//tree�� level ���� �� max depth ã��
	for (int i = 0; i < mtreeList.size(); i++)
	{
		MAPTREE *temp = &mtreeList[i];
		cout << temp->num << "->";
		//���� ��ġ�� �� �� �׸���
		for (int i = temp->LB.y; i < temp->RT.y; i++)
		{
			for (int j = temp->LB.x; j < temp->RT.x; j++)
			{
				Int2 setTilePoint = Int2(j, i);
				Int2 randomFloor = RandomTile(Int2(1,1),Int2(4,3));
				SetTile(setTilePoint, randomFloor, 1, TILE_FLOOR);
			}
		}
		
		
	}
	cout << "�� �׸��� �Ϸ�" << endl;
	
	
	//���� ����Ʈ ����
	for (int i = connList.size() - 1; i >= 1; i -= 2)
	{
		//cout << "LEVEL : " << connList[i]->level << "  ";
		int next = i - 1;
		if (next < 1)break;
		//MakeConnPoint(connList[next], connList[i]);
		MakeConnPoint2(connList[next], connList[i]);
	}
	cout << "��������Ʈ ���� �Ϸ�" << endl;
	Connecting(connList);
	cout << "���� �Ϸ�" << endl;
	MakeResponRoom();
	MakeWall();
	for (int i = 1; i < mtreeList.size(); i++)
	{
		MAPTREE* temp = &mtreeList[i];
		
		{
			MakePiller(temp);
			MakeDoor(temp);
		}
	}
	
	
	ControlDoor(true);

	cout << "�� �Ϸ�" << endl;
	Update();
}

void ObTileMap::AutoBossMap()
{
	ResizeTile(Int2(40, 30));

	for (int x = 0; x < tileSize.x; x++)
	{
		for (int y = 0; y < tileSize.y; y++)
		{
			Int2 setTilePoint = Int2(x, y);
			Int2 randomFloor = RandomTile(Int2(1, 1), Int2(4, 3));
			SetTile(setTilePoint, randomFloor, 1, TILE_FLOOR);
		}
	}
	MakeWall();
	cout << "�� ���� �Ϸ�" << endl;
    Update();
}


