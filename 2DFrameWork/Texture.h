#pragma once
class Texture :public Singleton<Texture>
{
private:
    //    key  ,  value
    map<wstring, ID3D11ShaderResourceView*> textureList;
public:
    ~Texture();
    ID3D11ShaderResourceView* LoadTexture(wstring file);
    bool DeleteTexture(wstring file);


};

