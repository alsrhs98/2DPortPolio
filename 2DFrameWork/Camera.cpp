#include "framework.h"


Camera::Camera()
{
    //첫 카메라 위치
    position.x = 0.0f;
    position.y = 0.0f;
    //뷰포트
    viewport.width = App.GetWidth();
    viewport.height = App.GetHeight();
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}


void Camera::Set()
{
    //뷰는 역이동행렬
    V = Matrix::CreateTranslation(-position.x, -position.y, 0.0f);
    //투영 행렬 (-1~ 1) 좌표로 투영 NDC
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
