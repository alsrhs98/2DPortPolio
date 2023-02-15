
struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : UV0;
};
// PI
struct PixelInput
{
    float4 position : SV_POSITION; //ȭ����ǥ�� ������
    float2 uv : UV0; //���ε� ��ǥ
    float4 color : COLOR0; //��������
};

//������� ũ��� 16byte����� �������Ѵ�.

cbuffer VS_WVP : register(b0) 
{
    matrix WVP;
}
cbuffer VS_COLOR : register(b1)
{
    //4 ���� 4����Ʈ
    float4 color;
}
cbuffer VS_UV : register(b2)
{
    float4 uv;
}
//�ȼ����̴� �������
cbuffer PS_LIGHT : register(b0)
{
	float2 ScreenPos; //��ũ����ǥ
	float Radius; //������ũ��
	float Select; //���°�
	float4 LightColor; //���� ��
	float4 OutColor; //�ܰ� ��
};



//���ؽ� ���̴�
//��ȯ��  �Լ��̸�(�Ű�����)
// VertexInput(in) ->  VS  -> PixelInput (out)
PixelInput VS(VertexInput input)
{
    //������ȯ�� ���� ������ ��
    PixelInput output;
    //output.Position * input.Position;
    //L-W
    output.position = mul(input.position, WVP);
    
    output.color = color;
    
    //���ε� ��ǥ ���
    output.uv = input.uv;
    
    [branch]
	if (output.uv.x == 0.0f)
        output.uv.x = uv.x;
    else
        output.uv.x = uv.z;
    
    [branch]
	if (output.uv.y == 0.0f)
        output.uv.y = uv.y;
    else
        output.uv.y = uv.w;
    
    
    
    return output;
}

//�ؽ��� �ڿ�  (srv) ���� ����
Texture2D Texture : register(t0);

//�����
SamplerState Sampler : register(s0);


//�ȼ����̴� ���� �Լ�
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET �� Ÿ���̵� ���� 
{
    float4 TextureColor =
    // ���ε� ��ǥ�� �ؽ��� �ε�
    Texture.Sample(Sampler, input.uv);
    
    //�о�� �׸������� �ȼ������� ���ǹ����� ��
    [flatten]
    if (TextureColor.r == 1.0f &&
        TextureColor.g == 0.0f &&
        TextureColor.b == 1.0f)
    {
        //�ش� �ȼ��� ��������ʴ´�.
        discard;
    }
    
    TextureColor = TextureColor + (input.color * 2.0f - 1.0f);
	TextureColor = saturate(TextureColor);
  
    
	float2 Minus = input.position.xy - ScreenPos;
	float dis = Minus.x * Minus.x + Minus.y * Minus.y;
	dis = sqrt(dis);
	if (Select == 0.0f)
	{
		if (dis > Radius)
		{
			TextureColor.rgb += (OutColor.rgb * 2.0f - 1.0f);
		}
		else
		{
			TextureColor.rgb += (LightColor.rgb * 2.0f - 1.0f);
		}
	}
    else
	{
		float temp = 1.0f - saturate(dis / Radius);
		TextureColor.rgb += (LightColor.rgb * 2.0f - 1.0f);
		TextureColor.rgb *= temp;
	}
	TextureColor = saturate(TextureColor);
    
	return TextureColor;

}