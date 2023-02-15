#pragma once
#define MAXTILENUM 22
#define  UPWALL		Int2(1, 0)
#define  DOWNWALL	Int2(1, 2)
#define  LEFTWALL	Int2(0, 1)
#define  RIGHTWALL  Int2(2, 1)
#define  FLOOR		Int2(1, 1)

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

	bool visited = false;
	Int2 connPoint;

	//Int2 tileSize() { return Int2(RT.x - LB.x, RT.y - LB.y); };
	MAPTREE * right;
	MAPTREE *  left;
	MAPTREE * parent;
	
	Int2 midPoint() { return{(LB.x+RT.x)/2, (LB.y+RT.y)/2}; };
	void setLevel(int level) { this->level = level; };
	void setNum(int num) { this->num = num; };
	bool haveSubTree()
	{
		if (left != NULL and right != NULL)return true;
		else return false;
	}

};

class Main : public Scene
{
	
private:
	ObTileMap * map;

	Int2		tileSize;
	Int2		pickingIdx;
	int			imgIdx;
	Int2		mouseIdx;
	int			tileState;
	Color		tileColor;

	MAPTREE *  mtree;
	MAPTREE*   parent;
private:
	Int2 dividePoint;
	vector<MAPTREE> mtreeList; //공간분할 저장 벡터
	vector<MAPTREE*> connList; // 연결포인트 저장 벡터
	vector<Tile*>connWay;
	
	
public:
	
	//탐색 알고리즘
	void preorder(MAPTREE* mt)
	{
		if (mt->parent != NULL)
		{
			int num = mt->parent->level + 1;
			mt->setLevel(num);
		}
		
		if (mt->left == NULL and mt->right == NULL)
		{
			mtreeList.push_back(*mt);
			
		}
		if (mt->left != NULL)
			preorder(mt->left);
		 
		if (mt->right != NULL)
			preorder(mt->right);
	}
	void bfs(MAPTREE* mt, int i);
	
	void DivideArea(MAPTREE* mtree);
	void MakeRoom(vector<MAPTREE>&mtreeList);
	//void MakeConnPoint(vector<MAPTREE>&mtreeList, vector<Int2>& connList);
	void ConnectTile(Int2 start , Int2 end, ObTileMap* map);
	void Connecting(vector<MAPTREE*> connList, ObTileMap* map);

	void MakeConnPoint(MAPTREE* mt, MAPTREE* right);
	void AutomaticMap();

	void Delete(MAPTREE* mt)
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
	int dep = 0;
	void FindMaxDepth(MAPTREE* mt, int depth)
	{
		if (mt == NULL) return;
		if (dep < depth) dep = depth;
		mt->setLevel(depth);
		if (mt->left != NULL) FindMaxDepth(mt->left, depth + 1);
		if (mt->left != NULL) FindMaxDepth(mt->right, depth + 1);

	}
	int MaxDepth(MAPTREE* mt)
	{
		FindMaxDepth(mt, 0);
		return dep;
	}
	//노드 관련 함수
	int i = 0;
	int NodeCnt(MAPTREE* mt)
	{
		int cnt = 0;
		if (mt != NULL)
		{
			cnt = 1 + NodeCnt(mt->left) + NodeCnt(mt->right);
			i++;
		}
		return cnt;
	};

	

public:
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
