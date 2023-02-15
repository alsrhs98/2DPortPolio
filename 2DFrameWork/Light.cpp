#include "framework.h"

Light::Light()
{
   
    light.screenPos =
        Vector2(App.GetHalfWidth(), App.GetHalfHeight());
    //���� ������
    light.radius = 300.0f;
    //������
    light.lightColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
    //���� ��
    light.outColor = Color(0.0f, 0.0f, 0.0f, 0.5f);
    //���� 0��
    light.select = 0;

    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(PointLight);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &lightBuffer);
        assert(SUCCEEDED(hr));
    }
    D3D->GetDC()->PSSetConstantBuffers(0, 1, &lightBuffer);
}

Light::~Light()
{
    SafeRelease(lightBuffer);
}

void Light::Set()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    D3D->GetDC()->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy_s(mappedResource.pData, sizeof(PointLight), &light, sizeof(PointLight));
    D3D->GetDC()->Unmap(lightBuffer, 0);
}

void Light::SetLightPos(Vector2 Wpos)
{
    //World To Screen
    Wpos -= CAM->position;
    Wpos.y *= -1.0f;
    Wpos += Vector2(App.GetHalfWidth(), App.GetHalfHeight());
    light.screenPos = Wpos;
}