// ƒpƒ‰ƒ[ƒ^
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

float4 main(VS_OUTPUT input) : SV_Target0
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}