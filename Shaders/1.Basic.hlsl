
struct VertexInput
{
	float4 position : POSITION0; //�뵵
    float4 color : COLOR0;
};
// PI
struct PixelInput
{
    float4 position : SV_POSITION; //ȭ����ǥ�� ������
    float4 color : COLOR0;
};

//������Ʈ�� ũ�� ȸ�� �̵�
cbuffer VS_WVP : register(b0) //register(b0)
{
    matrix WVP;
}
cbuffer VS_COLOR : register(b1)
{
    //4 ���� 4����Ʈ
    float4 color;
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



PixelInput VS(VertexInput input)
{
    //������ȯ�� ���� ������ ��
    PixelInput output;
    
    output.position = mul(input.position, WVP);
	output.color = input.color + (color * 2.0f - 1.0f);
	saturate(output.color);
    
	return output;
}
//->pixel  PS   ->color
//�ȼ����̴� ���� �Լ�
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET �� Ÿ���̵� ���� 
{
    //����Ʈ���� ��ȯ�� ��������
    //��ũ����ǥ
    
	float4 OutputColor;
	OutputColor = saturate(input.color);
    
	float2 Minus = input.position.xy - ScreenPos;
	float dis = Minus.x * Minus.x + Minus.y * Minus.y;
	dis = sqrt(dis);
	if (Select == 0.0f)
	{
	if (dis > Radius)
	{
		OutputColor.rgb += (OutColor * 2.0f - 1.0f);
	}
	else
	{
		OutputColor.rgb += (LightColor * 2.0f - 1.0f);
	}
	}
	 else
	{
		float temp = 1.0f - saturate(dis / Radius);
		OutputColor.rgb += (LightColor.rgb * 2.0f - 1.0f);
		OutputColor.rgb *= temp;
	}
	OutputColor = saturate(OutputColor);
    
	return OutputColor;
}