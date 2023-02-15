#include "framework.h"


Util::RECT::RECT(Vector2 pivot, Vector2 scale)
{
    min.x = pivot.x - scale.x * 0.5f;
    min.y = pivot.y - scale.y * 0.5f;
    max.x = pivot.x + scale.x * 0.5f;
    max.y = pivot.y + scale.y * 0.5f;
}

Util::CIRCLE::CIRCLE(Vector2 pivot, Vector2 scale)
{
    this->pivot = pivot;
    radius = scale.x * 0.5f;
}

bool Util::IntersectRectCoord(RECT& rc, Vector2 coord)
{
    if (rc.min.x < coord.x and coord.x < rc.max.x
        and rc.min.y < coord.y and coord.y < rc.max.y)
    {
        return true;
    }
    return false;
}

bool Util::IntersectRectRect(RECT& rc1, RECT& rc2)
{
    if (rc1.min.x < rc2.max.x and
        rc1.max.x > rc2.min.x and
        rc1.min.y < rc2.max.y and
        rc1.max.y > rc2.min.y)
    {
        return true;
    }
    return false;
}

bool Util::IntersectRectRect(GameObject* ob1, GameObject* ob2)
{
    //중심점
    Vector2 rc1Pivot = ob1->GetWorldPivot();
    Vector2 rc2Pivot = ob2->GetWorldPivot();
    //두 중심점간의 차이 벡터
    Vector2 Dis = rc2Pivot - rc1Pivot;

    //사각형마다 2개의 절반크기의 벡터
    Vector2 Rc1Up =
        ob1->GetUp() * ob1->scale.y * 0.5f;
    Vector2 Rc1Right =
        ob1->GetRight() * ob1->scale.x * 0.5f;

    Vector2 Rc2Up =
        ob2->GetUp() * ob2->scale.y * 0.5f;
    Vector2 Rc2Right =
        ob2->GetRight() * ob2->scale.x * 0.5f;

    //ob1의 right축 비교
    //       절대값(내적 a . b)
    float c = fabs(Dis.Dot(ob1->GetRight()));

    //ob2에서 두벡터가 투영된 길이
    float a = fabs(Rc2Up.Dot(ob1->GetRight()))
        + fabs(Rc2Right.Dot(ob1->GetRight()));
    //ob1에서 두벡터가 투영된 길이
    float b = ob1->scale.x * 0.5f;

    if (c > a + b)return false;

    //ob1의 Up축 비교
    //       절대값(내적 a . b)
    c = fabs(Dis.Dot(ob1->GetUp()));

    //ob2에서 두벡터가 투영된 길이
    a = fabs(Rc2Up.Dot(ob1->GetUp()))
        + fabs(Rc2Right.Dot(ob1->GetUp()));
    //ob1에서 두벡터가 투영된 길이
    b = ob1->scale.y * 0.5f;

    if (c > a + b)return false;

    //ob2의 Right축 비교
    //       절대값(내적 a . b)
    c = fabs(Dis.Dot(ob2->GetRight()));

    //ob1에서 두벡터가 투영된 길이
    a = fabs(Rc1Up.Dot(ob2->GetRight()))
        + fabs(Rc1Right.Dot(ob2->GetRight()));
    //ob2에서 두벡터가 투영된 길이
    b = ob2->scale.x * 0.5f;

    if (c > a + b)return false;

    //ob2의 Up축 비교
    //       절대값(내적 a . b)
    c = fabs(Dis.Dot(ob2->GetUp()));

    //ob1에서 두벡터가 투영된 길이
    a = fabs(Rc1Up.Dot(ob2->GetUp()))
        + fabs(Rc1Right.Dot(ob2->GetUp()));
    //ob2에서 두벡터가 투영된 길이
    b = ob2->scale.y * 0.5f;

    if (c > a + b)return false;

    return true;
}

bool Util::IntersectRectCircle(RECT& rc, CIRCLE& cc)
{
    //가로로 확장된 사각형과 원의 중점 충돌
    Vector2 RectPivot = (rc.max + rc.min) * 0.5f;
    Vector2 RectScale = rc.max - rc.min;
    RECT WRect(RectPivot, RectScale + Vector2(cc.radius * 2.0f, 0.0f));
    //ObRect
    if (IntersectRectCoord(WRect, cc.pivot))
    {
        return true;
    }
    RECT HRect(RectPivot, RectScale + Vector2(0.0f, cc.radius * 2.0f));
    if (IntersectRectCoord(HRect, cc.pivot))
    {
        return true;
    }
    Vector2 Edge[4];
    Edge[0] = rc.max;
    Edge[1] = rc.min;
    Edge[2] = Vector2(rc.min.x, rc.max.y);
    Edge[3] = Vector2(rc.max.x, rc.min.y);
    for (int i = 0; i < 4; i++)
    {
        if (IntersectCircleCoord(cc, Edge[i]))
        {
            return true;
        }
    }
    return false;
}

bool Util::IntersectRectCircle(GameObject* ob1, GameObject* ob2, COLRECTCIR& result)
{
    //가로로 확장된 사각형과 원의 중점 충돌
    Vector2 RectPivot = ob1->GetWorldPivot();
    Vector2 RectScale = ob1->scale;
    RECT WRect(RectPivot, RectScale + Vector2(ob2->scale.x, 0.0f));
    //ObRect
    if (IntersectRectCoord(WRect, ob2->GetWorldPivot()))
    {
        result = COLRECTCIR::LEFTRIGHT;
        return true;
    }
    RECT HRect(RectPivot, RectScale + Vector2(0.0f, ob2->scale.x));
    if (IntersectRectCoord(HRect, ob2->GetWorldPivot()))
    {
        result = COLRECTCIR::UPDOWN;
        return true;
    }
    Vector2 Edge[4];
    RECT rc = RECT(ob1->GetWorldPivot(), ob1->scale);
    Edge[0] = rc.max;
    Edge[1] = rc.min;
    Edge[2] = Vector2(rc.min.x, rc.max.y);
    Edge[3] = Vector2(rc.max.x, rc.min.y);

    CIRCLE cc = CIRCLE(ob2->GetWorldPivot(), ob2->scale);
    for (int i = 0; i < 4; i++)
    {
        if (IntersectCircleCoord(cc, Edge[i]))
        {
            result = COLRECTCIR::EDGE;
            return true;
        }
    }
    result = COLRECTCIR::NONE;
    return false;
}

bool Util::IntersectCircleCoord(CIRCLE& cc, Vector2 coord)
{
    Vector2 Distance = cc.pivot - coord;
    if (Distance.Length() < cc.radius)
    {
        return true;
    }
    return false;
}

bool Util::IntersectCircleCircle(CIRCLE& cc1, CIRCLE& cc2)
{
    Vector2 Distance = cc1.pivot - cc2.pivot;
    if (Distance.Length() < cc1.radius + cc2.radius)
    {
        return true;
    }
    return false;
}

float Util::DirToRadian(Vector2 Dir)
{
    //만약 단위벡터가 아닐수도있으니까
    Dir.Normalize();
    return atan2f(Dir.y, Dir.x);
}

Vector2 Util::Lerp(Vector2 from, Vector2 to, float g)

{
    // g(0) -> from
    // g(1) -> from + to - from -> to
    return  from + (to - from) * g;
}

void Util::Replace(string* str, string comp, string rep)
{
    string temp = *str;

    size_t start_pos = 0;
    while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
    {
        temp.replace(start_pos, comp.length(), rep);
        start_pos += rep.length();
    }
    *str = temp;
}
