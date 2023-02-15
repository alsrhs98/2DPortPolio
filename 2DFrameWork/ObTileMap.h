#pragma once
#define MAXTILENUM 29
#define  FLOOR		Int2(1, 1)
#define  UPWALL		Int2(1, 0)
#define  DOWNWALL	Int2(1, 2)
#define  LEFTWALL	Int2(0, 1)
#define  RIGHTWALL  Int2(2, 1)

#define  LDCORNER	Int2(5, 4)
#define  RDCORNER	Int2(5, 0)
#define  LUCORNER	Int2(0, 4)
#define  RUCORNER	Int2(0, 0)


enum class RC
{
	ROW,
	COL
};

struct MAPTREE
{

	int level;
	int num = 0;
	Int2 LB;
	Int2 RT;
	RC  rc;

	//bool visited;

	bool visited;
	Int2 connPoint;

	//Int2 tileSize() { return Int2(RT.x - LB.x, RT.y - LB.y); };
	MAPTREE* right;
	MAPTREE* left;
	MAPTREE* parent;

	Int2 size() { return { RT.x - LB.x,RT.y - LB.y }; };
	Int2 midPoint() { return{ (LB.x + RT.x) / 2, (LB.y + RT.y) / 2 }; };
	int  tileWidth(){ return (RT.x - LB.x) * (RT.y - LB.y); };
	void setLevel(int level) { this->level = level; };
	void setNum(int num) { this->num = num; };
	bool haveSubTree()
	{
		if (left != NULL and right != NULL)return true;
		else return false;
	}

	

};

class Tile
{
public:
    int         F, G, H;//타일 비용
    Int2        idx;    //타일 인덱스
    Tile*       P;      //나를 갱신시킨 타일
    bool        isFind; //검사한적이 있는가?
    int         state;
    Vector2     Pos;


    void ClearCost();         //비용 초기화
    void ClacH(Int2 DestIdx); //H계산해라
    void ClacF(); //F계산해라
};
//               타일주소,비교값


using PTile = pair<Tile*, int>;

struct compare
{
    //연산자 오버로딩
    bool operator()(PTile& a, PTile& b)
    {
        return a.second > b.second;
    }
};
struct Door
{
	bool isCol;
	bool isCollidable;
	Int2 positon;
};

enum TileState
{
    TILE_NONE,
    TILE_WALL,
    TILE_DOOR,
    TILE_TRAP,
    TILE_WATER,
	TILE_FLOOR,
	TILE_HALLWAY,
	TILE_SIZE
    //TILE_SAND,
};

class ObTileMap : public GameObject
{
private:
    VertexTile*             vertices;
    ID3D11Buffer *          vertexBuffer;
    Int2                    tileSize;   //10x10 , 4x4
    vector<vector<Tile>>    Tiles;
public:
    string                  file;

    ObImage *               tileImages[4];
    ObTileMap* leftTileMap;
    ObTileMap* rightTileMap;

public:
    ObTileMap();
    ~ObTileMap();


    void        Render()override;
    void        SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx = 0 ,
						int TileState = TILE_NONE , Color color = Color(0.5f, 0.5f, 0.5f, 0.5f));
    void        ResizeTile(Int2 TileSize);
    void        Save();
    void        Load();
    void        CreateTileCost();
	
	int         GetTileState(Int2 TileIdx);
    
	bool        WorldPosToTileIdx(Vector2 WPos, Int2 & TileIdx);
    bool        PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way);
    
	Vector2     GetTilePosition(Int2 TileIdx);
	Vector2     TileIdxToWorldPos(Int2 TileIdx)
	{
		return { (float)(TileIdx.x) * (scale.x),(float)(TileIdx.y) * (scale.y) };
	}
    
	Int2        GetTileSize() { return tileSize; };
///////////////////////////////////
private : 

	float timer = 0.0f;

	ObTileMap* rolPiller;
	ObTileMap* colPiller;

	Int2 dividePoint;

	vector<MAPTREE*> connList; // 연결포인트 저장 벡터
	vector<Tile*>connWay;


public:
	
	MAPTREE* mtree;
	
	vector<MAPTREE> mtreeList; //공간분할 저장 벡터
	vector<Door> DoorPos;


    //탐색 알고리즘
	void preorder(MAPTREE* mt)
	{
		static int i = 0;
		if (mt->parent != NULL)
		{
			int num = mt->parent->level + 1;
			mt->setLevel(num);
		}

		if (mt->left == NULL and mt->right == NULL)
		{
			mt->setNum(i);
			mtreeList.push_back(*mt);
			i++;
		}
		if (mt->left != NULL)
			preorder(mt->left);

		if (mt->right != NULL)
			preorder(mt->right);
	}
	vector<MAPTREE>responRoom;
	Int2 RandomTile(Int2 start, Int2 end);
	int dep = 0;
	
	void bfs(MAPTREE * mt, int i);
	void DivideArea(MAPTREE * mtree);
	void MakeRoom(vector<MAPTREE> & mtreeList);
	//void MakeConnPoint(vector<MAPTREE>&mtreeList, vector<Int2>& connList);
	//void ConnectTile(Int2 start, Int2 end);
	void Connecting(vector<MAPTREE*> connList);
	void MakeConnPoint(MAPTREE* leftNode ,  MAPTREE * right);
	void MakeConnPoint2(MAPTREE* leftNode, MAPTREE * right);
	void MakeWall();
	void MakeResponRoom();// 작은수 - > 큰수
	void SetPiller(Int2 point);
	void MakePiller(MAPTREE* room);
	void MakeDoor(MAPTREE* room);
	void ControlDoor(bool isOpen);
	void AutomaticMap(MAPTREE* mtree);
	void AutoBossMap();
	/////////////////////////////////////////////////////////
	void Delete(MAPTREE * mt)
	{
		if (mt->left != NULL)
		{
			Delete(mt->left);
			if (mt->left->left == NULL and mt->left->right == NULL)
			{
				SafeDelete(mt->left);
			}
		}
		if (mt->right != NULL)
		{
			Delete(mt->right);
			if (mt->right->left == NULL and mt->right->right == NULL)
			{
				SafeDelete(mt->right);
			}
		}
	}
	void FindMaxDepth(MAPTREE * mt, int depth)
	{
		if (mt == NULL) return;
		if (dep < depth) dep = depth;
		mt->setLevel(depth);
		if (mt->left != NULL) FindMaxDepth(mt->left, depth + 1);
		if (mt->left != NULL) FindMaxDepth(mt->right, depth + 1);

	}
	int MaxDepth(MAPTREE * mt)
	{
		FindMaxDepth(mt, 0);
		return dep;
	}
	//노드 관련 함수
	int i = 0;
	int NodeCnt(MAPTREE * mt)
	{
		int cnt = 0;
		if (mt != NULL)
		{
			cnt = 1 + NodeCnt(mt->left) + NodeCnt(mt->right);
			i++;
		}
		return cnt;
	};

	/////////////////////////////////////////////////////////////

	//플레이어 , 몬스터, 맵 상호작용 함수 모음
	//방에 플레이어가 왔는가?
	bool isPlayerVisiting(Int2 playerPos, MAPTREE* room)
	{
		if (playerPos.x > room->LB.x and playerPos.x< room->RT.x
			and playerPos.y> room->LB.y and playerPos.y < room->RT.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//bool doorOpen(Int2 playerPos, MAPTREE* room);

};

