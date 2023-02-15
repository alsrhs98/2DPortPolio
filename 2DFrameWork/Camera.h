#pragma once
class Camera : public Singleton<Camera>
{
private:
    Viewport            viewport;       //뷰포트
    Matrix              V,P,VP;             //View x Proj
public:
    //카메라위치
    Vector2             position;
public:
    Camera();
    void          Set();
    void          ResizeScreen();
    //참조형리턴은  public과 같아서 const 를 붙여서 외부에서 쓰기가 안되도록 막음

    const Matrix& GetP() { return P; };
    const Matrix& GetVP() { return VP; };
};