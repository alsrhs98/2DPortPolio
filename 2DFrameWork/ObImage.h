#pragma once

enum class ANISTATE
{
    STOP,      //����
    LOOP,      //�ݺ����
    ONCE,      //�ѹ����
    R_LOOP,//���ݺ����
    R_ONCE,//
};
class ObImage : public GameObject
{
    friend class ObTileMap;
private:
    static ID3D11Buffer* vertexBuffer;
    static ID3D11Buffer* uvBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

private:
    //�׸����Ͽ��� ���� �ؽ����ڿ�
    ID3D11ShaderResourceView*   SRV;

    ID3D11SamplerState*         sampler;//s0
    D3D11_SAMPLER_DESC	        samplerDesc;

    ANISTATE                    aniState;
    float                       aniTime;
    float                       aniInterval; //�������
    bool                        aniXAxis;   //�������?
public:
    Vector4                     uv;
    Int2                        maxFrame;   //���Ұ���
    Int2                        frame;      //����������� �ε���
    bool                        reverseLR;
	bool                        reverseHL;
    wstring                     file;

private:
    void                        PlayAnim();
public:
    ObImage(wstring file);
    ~ObImage();


    void Render()override;
    void ChangeAnim(ANISTATE ani, float interval, bool XAxis = true);
    void ChangeSampler(D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_POINT,
        D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP);
    ID3D11ShaderResourceView* GetSRV()
    {
        return SRV;
    }
};

