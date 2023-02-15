#pragma once

enum class ANISTATE
{
    STOP,      //정지
    LOOP,      //반복재생
    ONCE,      //한번재생
    R_LOOP,//역반복재생
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
    //그림파일에서 얻어온 텍스쳐자원
    ID3D11ShaderResourceView*   SRV;

    ID3D11SamplerState*         sampler;//s0
    D3D11_SAMPLER_DESC	        samplerDesc;

    ANISTATE                    aniState;
    float                       aniTime;
    float                       aniInterval; //재생간격
    bool                        aniXAxis;   //가로재생?
public:
    Vector4                     uv;
    Int2                        maxFrame;   //분할갯수
    Int2                        frame;      //현재재생중인 인덱스
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

