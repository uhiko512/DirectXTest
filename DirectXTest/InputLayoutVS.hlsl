struct VSInput {
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 uv : TEXTURE0;
};

float4 main(VSInput i) : SV_POSITION {
	return i.pos;
}