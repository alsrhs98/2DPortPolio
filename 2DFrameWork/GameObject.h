#pragma once
class ObLine;
enum class SPACE
{
	WORLD,
	SCREEN, // ����� 0,0��
};
enum class COLLIDER 
{
	NONE,
	RECT,
	CIRCLE, // ����� 0,0��
};
class GameObject
{
	//static member
private:
	static ObLine*			axisObject;

	static ID3D11Buffer*	WVPBuffer; 
	static ID3D11Buffer*	colorBuffer;
protected:
	static Shader*			basicShader;
	static Shader*			imageShader;
	static Shader *			tileMapShader;
public:

	//static member function
public:
	static void CreateStaticMember();
	static void DeleteStaticMember();

	//member
private:
	Matrix		S, R, T, R2, RT, Pi, WVP;
	Matrix*		P;
	Vector2		position;
protected:
	Matrix		W;
public:
	Vector2		pivot;
	Vector2		scale;
	float		rotation;
	float		rotation2;
	bool		axis;
	bool        visible;
	bool        isFilled;
	Color       color;
	SPACE		space;
	COLLIDER	collider;
	bool        colEnable;
	//member function
public:
	GameObject();
	virtual ~GameObject() {};

	virtual void Update();
	virtual void Render();

	bool Intersect(Vector2 coord);
	bool IntersectScreenMouse(Vector2 coord);
	bool Intersect(GameObject* GO);

	//getter setter
public:
	
	//��ġ �޾ƿ���
	Vector2 GetLocalPos()	{ return position;}
	Vector2 GetWorldPos()	{ return Vector2(RT._41, RT._42); }
	Vector2 GetWorldPivot() { return Vector2(W._41, W._42); }//
	//��ġ�� �����Ҷ� =
	void	SetLocalPos(Vector2 location)	{ position = location; }
	void	SetLocalPosX(float locationX)	{ position.x = locationX; }
	void	SetLocalPosY(float locationY)	{ position.y = locationY; }
	void	SetWorldPos(Vector2 worldPos);
	void	SetWorldPosX(float worldPosX);
	void	SetWorldPosY(float worldPosY);
	//�̵���ų�� +=
	void	MoveLocalPos(Vector2 Velocity)	{ position += Velocity; }
	void	MoveWorldPos(Vector2 Velocity);
	//������� ����
	Vector2 GetRight()	{ return Vector2(RT._11, RT._12); }
	Vector2 GetUp()		{ return Vector2(RT._21, RT._22); }
	//�θ���� �����
	void	SetParentRT( GameObject& src)	{ P = &src.RT; }
	void	SetParentT( GameObject& src)	{ P = &src.T; }
};

