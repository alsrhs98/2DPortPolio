#include "framework.h"


Camera::Camera()
{
    //ù ī�޶� ��ġ
    position.x = 0.0f;
    position.y = 0.0f;
    //����Ʈ
    viewport.width = App.GetWidth();
    viewport.height = App.GetHeight();
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}


void Camera::Set()
{
    //��� ���̵����
    V = Matrix::CreateTranslation(-position.x, -position.y, 0.0f);
    //���� ��� (-1~ 1) ��ǥ�� ���� NDC
    P = Matrix::CreateOrthographic(App.GetWidth(), App.GetHeight(), 0.0f, 10.0f);
    VP = V * P;
}

void Camera::ResizeScreen()
{
    /*viewport.x = 100;
    viewport.y = 100;*/

    viewport.width = App.GetWidth();
    viewport.height = App.GetHeight();
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}
