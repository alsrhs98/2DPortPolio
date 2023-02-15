#pragma once

enum class COLRECTCIR
{
    NONE,
    UPDOWN,
    LEFTRIGHT,
    EDGE
};

namespace Util
{
    //�浹Ÿ��
    struct RECT
    {
        Vector2 min;
        Vector2 max;
        RECT(Vector2 pivot, Vector2 scale);
    };
    struct CIRCLE
    {
        Vector2 pivot;
        float   radius;
        CIRCLE(Vector2 pivot, Vector2 scale);
    };
    //�浹�Լ�
    bool IntersectRectCoord(RECT& rc, Vector2 coord);
    bool IntersectCircleCoord(CIRCLE& cc, Vector2 coord);

    //�׸� �׸�(AABB)
    bool IntersectRectRect(RECT& rc1, RECT& rc2);
    //OBB
    bool IntersectRectRect(GameObject* ob1, GameObject* ob2);


    //�� ��
    bool IntersectCircleCircle(CIRCLE& cc1, CIRCLE& cc2);
    //�׸� ��
    bool IntersectRectCircle(RECT& rc, CIRCLE& cc);

    bool IntersectRectCircle(GameObject* rc, GameObject* cc, COLRECTCIR& result);


    float DirToRadian(Vector2 Dir);

    Vector2 Lerp(Vector2 from, Vector2 to, float g);

    //���ø� �Լ��� ����,���� �и� �Ұ�
    template<typename T>
    T saturate(T src, T min = 0, T max = 1)
    {
        if (src < min)
        {
            return min;
        }
        else if (src > max)
        {
            return max;
        }
        return src;
    }
    template<typename A>
    void Swap(A & a, A & b)
    {
        A temp = a;
        a = b;
        b = temp;
    }

    void Replace(string* str, string comp, string rep);
   

   /* template<typename A>
    void Shuffle(A & Arr, int Range, int ShuffleCount)
    {
        for (int i = 0; i < ShuffleCount; i++)
        {
            int sour = rand() % Range;
            int dest = rand() % Range;
            Swap(Arr[sour], Arr[dest]);
        }
    }*/
};

