struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float3 tangentL : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

cbuffer Camera : register(b1)
{
	float4 camPos;
};

SamplerState samp : register(s0);
Texture2D normalMap : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);
	
	//���f���̖@�������̂܂܎g���ꍇ
	//float3 N = normalize(pin.normal);

	//�@���}�b�v�������̖@�������o��
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;	//0�`1�̐F�f�[�^���A-1�`1�̃x�N�g���f�[�^�ɕϊ�
	N = normalize(N);


	float3 L = normalize(-pin.tangentL);

	float diffuse = dot(N, L);
	float ambient = 0.5f;

	float3 R = reflect(-L, N);

	//�s�N�Z������J�����֌������x�N�g��
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);

	float specular = pow(saturate(dot(R, V)), 30.0f);

	color.rgb = diffuse * 0.5f + ambient + specular;

	return color;
}