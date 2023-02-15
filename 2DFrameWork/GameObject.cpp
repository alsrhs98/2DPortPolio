#include "framework.h"

ObLine* GameObject::axisObject = nullptr;
ID3D11Buffer* GameObject::WVPBuffer = nullptr;
ID3D11Buffer* GameObject::colorBuffer = nullptr;
Shader* GameObject::basicShader = nullptr;
Shader* GameObject::imageShader = nullptr;
Shader * GameObject::tileMapShader = nullptr;

void GameObject::CreateStaticMember()
{
	//CreateConstantBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Matrix);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &WVPBuffer);
		assert(SUCCEEDED(hr));
	}
	D3D->GetDC()->VSSetConstantBuffers(0, 1, &WVPBuffer);
	//CreateConstantBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(Color);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &colorBuffer);
		assert(SUCCEEDED(hr));
	}
	

	D3D->GetDC()->VSSetConstantBuffers(1, 1, &colorBuffer);

	basicShader = new Shader(L"1.Basic");
	imageShader = new Shader(L"2.Image");
	tileMapShader = new Shader(L"3.TileMap");
	axisObject = new ObLine();
}

void GameObject::DeleteStaticMember()
{
	delete axisObject;
	delete basicShader;
	delete imageShader;
	delete tileMapShader;
	WVPBuffer->Release();
	colorBuffer->Release();
}


GameObject::GameObject()
{
	position.x = 0;
	position.y = 0;

	scale.x = 1.0f;
	scale.y = 1.0f;

	rotation = 0;
	rotation2 = 0;
	color = Color(0.5f, 0.5f, 0.5f, 0.5f);
	visible = true;
	isFilled = true;
	axis = false;
	P = nullptr;
	pivot = OFFSET_N;
	space = SPACE::WORLD;
	collider = COLLIDER::NONE;
	colEnable = true;
}

void GameObject::Update()
{
	Pi = Matrix::CreateTranslation(pivot.x, pivot.y, 0.0f);
	S = Matrix::CreateScale(scale.x, scale.y,1.0f);
	R = Matrix::CreateRotationZ(rotation);
	T = Matrix::CreateTranslation(position.x, position.y,0.0f);
	R2 = Matrix::CreateRotationZ(rotation2);

	RT = R * T * R2;
	
	//P의 주소가 있으면
	if (P)
	{
		RT *= *P;
	}
	W = Pi * S * RT;
}

void GameObject::Render()
{
	if (axis)
	{
		//right
		axisObject->SetWorldPos(GetWorldPos());
		axisObject->rotation = Util::DirToRadian(GetRight());
		axisObject->scale.x = scale.x * 2.0f;
		axisObject->color = Color(1, 0, 0);
		axisObject->Update();
		axisObject->Render();
		//up
		axisObject->rotation= Util::DirToRadian(GetUp());
		axisObject->scale.x = scale.y * 2.0f;
		axisObject->color = Color(0, 1, 0);
		axisObject->Update();
		axisObject->Render();
	}

	/*const int a = 5;
	a = 7;*/

	switch (space)
	{
		//CAM->GetP() = Matrix();
	case SPACE::WORLD:
		//  W * V * P
		WVP = W * CAM->GetVP();
		break;
	case SPACE::SCREEN:
		WVP = W * CAM->GetP();
		break;
	}
	//행우선에서 열우선구조로 변경
	WVP = WVP.Transpose();
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(WVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Matrix), &WVP, sizeof(Matrix));
		D3D->GetDC()->Unmap(WVPBuffer, 0);
	}
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Color), &color, sizeof(Color));
		D3D->GetDC()->Unmap(colorBuffer, 0);
	}
}

bool GameObject::Intersect(Vector2 coord)
{
	if (not colEnable) return false;

	if (collider == COLLIDER::RECT)
	{
		if (GetRight() == RIGHT)
		{
			Util::RECT rc1(GetWorldPivot(), scale);
			return IntersectRectCoord(rc1, coord);
		}
		else
		{
			Vector2 rc1Pivot = Vector2::Transform(pivot, S);
			Matrix	rc1Inverse = RT.Invert();
			Util::RECT rc1(rc1Pivot, scale);
			coord = Vector2::Transform(coord, rc1Inverse);
			return IntersectRectCoord(rc1, coord);
		}
	}
	else if (collider == COLLIDER::CIRCLE)
	{
		Util::CIRCLE cc1(GetWorldPivot(), scale);
		return Util::IntersectCircleCoord(cc1, coord);
	}
	return false;
}

bool GameObject::IntersectScreenMouse(Vector2 coord)
{
	coord.y = App.GetHalfHeight() - coord.y;
	coord.x = coord.x - App.GetHalfWidth();
	return Intersect(coord);
}

bool GameObject::Intersect(GameObject* GO)
{
	if (colEnable == false or GO->colEnable == false) return false;




	if (collider == COLLIDER::RECT)
	{
		if (GO->collider == COLLIDER::RECT)
		{
			if (GetRight() == RIGHT and GO->GetRight() == RIGHT)
			{
				Util::RECT rc1(GetWorldPivot(), scale);
				Util::RECT rc2(GO->GetWorldPivot(), GO->scale);
				return Util::IntersectRectRect(rc1, rc2);
			}
			else
			{
				return Util::IntersectRectRect(this, GO);
			}
		}
		else if (GO->collider == COLLIDER::CIRCLE)
		{
			if(GetRight() == RIGHT)
			{
				Util::RECT rc1(GetWorldPivot(), scale);
				Util::CIRCLE cc2(GO->GetWorldPivot(), GO->scale);
				return Util::IntersectRectCircle(rc1, cc2);
			}
			else
			{
				Vector2 rc1Pivot = Vector2::Transform(pivot, S);
				Matrix	rc1Inverse = RT.Invert();
				Vector2 cc2Pivot = GO->GetWorldPivot();
				cc2Pivot = Vector2::Transform(cc2Pivot, rc1Inverse);

				Util::RECT rc1(rc1Pivot, scale);
				Util::CIRCLE cc2(cc2Pivot, GO->scale);
				return Util::IntersectRectCircle(rc1, cc2);
			}
		}
	}
	else if (collider == COLLIDER::CIRCLE)
	{
		
		if (GO->collider == COLLIDER::RECT)
		{
			if (GO->GetRight() == RIGHT)
			{
				Util::CIRCLE cc1(GetWorldPivot(), scale);
				Util::RECT rc2(GO->GetWorldPivot(), GO->scale);
				return Util::IntersectRectCircle(rc2, cc1);
			}
			else
			{
				Vector2 rc1Pivot = Vector2::Transform(GO->pivot, GO->S);
				Matrix	rc1Inverse = GO->RT.Invert();
				Vector2 cc2Pivot = GetWorldPivot();
				cc2Pivot = Vector2::Transform(cc2Pivot, rc1Inverse);

				Util::RECT rc1(rc1Pivot, GO->scale);
				Util::CIRCLE cc2(cc2Pivot, scale);

				return Util::IntersectRectCircle(rc1, cc2);
			}
			
		}
		else if (GO->collider == COLLIDER::CIRCLE)
		{
			Util::CIRCLE cc1(GetWorldPivot(), scale);
			Util::CIRCLE cc2(GO->GetWorldPivot(), GO->scale);
			return Util::IntersectCircleCircle(cc1, cc2);
		}
	}
	return false;
}

void GameObject::SetWorldPos(Vector2 worldPos)
{
	if (!P)
	{
		position = worldPos;
	}
	else
	{
		Vector2 location = Vector2::Transform(worldPos, (*P).Invert());
		position = location;
	}
}
void GameObject::SetWorldPosX(float worldPosX)
{
	if (!P)
	{
		position.x = worldPosX;
	}
	else
	{
		Vector2 location = Vector2::Transform(Vector2(worldPosX,0), (*P).Invert());
		position.x = location.x;
	}
}
void GameObject::SetWorldPosY(float worldPosY)
{
	if (!P)
	{
		position.y = worldPosY;
	}
	else
	{
		Vector2 location = Vector2::Transform(Vector2(0, worldPosY), (*P).Invert());
		position.y = location.y;
	}
}
void GameObject::MoveWorldPos(Vector2 Velocity)
{
	if (!P)
	{
		position += Velocity;
	}
	else
	{
		Vector2 locVelocity = Vector2::TransformNormal(Velocity, (*P).Invert());
		position += locVelocity;
	}
}
