#pragma once
class Camera : public Singleton<Camera>
{
private:
    Viewport            viewport;       //����Ʈ
    Matrix              V,P,VP;             //View x Proj
public:
    //ī�޶���ġ
    Vector2             position;
public:
    Camera();
    void          Set();
    void          ResizeScreen();
    //������������  public�� ���Ƽ� const �� �ٿ��� �ܺο��� ���Ⱑ �ȵǵ��� ����

    const Matrix& GetP() { return P; };
    const Matrix& GetVP() { return VP; };
};