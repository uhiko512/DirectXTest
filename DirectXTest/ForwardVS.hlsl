cbuffer MVPConstantBuffer : register(b0) {
	float4x4 model;
	float4x4 view;
	float4x4 projection;
	float4x4 normalMat;
};

cbuffer CameraParamsBuffer : register(b1) {
	float4 viewPos;
};

struct VSInput  {
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 uv : TEXTURE0;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float2 uv : TEXTURE0;
	float3x3 tbnMat : TEXTURE1;
};


VSOutput main(VSInput i) {
	VSOutput o;

	o.pos = mul(projection, mul(view, mul(model, i.pos)));
	o.uv = i.uv;
	o.worldPos = mul(model, i.pos);
	
	float3 normal = normalize(mul(normalMat, float4(i.normal.xyz, 0.0f)));
	float3 tangent = normalize(mul(normalMat, float4(i.tangent.xyz, 0.0f)));
	float3 bitangent = normalize(cross(tangent, normal));
	o.tbnMat = transpose(float3x3(tangent, bitangent, normal));

	return o;
}