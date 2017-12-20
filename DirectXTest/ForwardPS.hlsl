Texture2D baseColor : register(t0);
Texture2D normal : register(t1);
Texture2D height : register(t2);
Texture2D roughness : register(t3);
SamplerState sampleType;

cbuffer CameraParamsBuffer : register(b0) {
	float4 viewPos;
};

cbuffer EngineParamsBuffer : register(b1) {
	float4 time;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float2 uv : TEXTURE0;
	float3x3 tbnMat : TEXTURE1;
};

struct PSOutput {
	float4 color : SV_TARGET;
};

PSOutput main(PSInput i) {
	PSOutput o;

	float3 lightDir = normalize(-1.0f * float3(-1.0, -1.0, -1.5));
	float3 viewDir = normalize(viewPos - i.worldPos).xyz;
	
	/* Parallex Occlusion Mapping */
	float3 tanViewDir = normalize(mul(viewDir, i.tbnMat));
	/*float heightMap = 1.0f - height.Sample(sampleType, float2(i.uv.x, 1.0f - i.uv.y));
	float2 puv = float2(i.uv.x, 1.0f - i.uv.y) - (tanViewDir.xy / tanViewDir.z * heightMap * 0.005f);*/

	float layer = 20.0f;
	float layerDepth = 1.0f / layer;
	float currentDepth = 0.0f;
	float2 currentUV = float2(i.uv.x, 1.0f - i.uv.y);
	float currnetMap = 1.0f - height.Sample(sampleType, currentUV);
	float2 offset = tanViewDir.xy * 0.015f * layerDepth;

	while (currentDepth < currnetMap && currentDepth < 1.0f) {
		currentDepth += layerDepth;
		currentUV -= offset;
		currnetMap = 1.0f - height.Sample(sampleType, currentUV);
	}

	float2 prevUV = currentUV + offset;
	float prevLayerDepth = currentDepth - layerDepth;
	float afterDepth = currentDepth - currnetMap;
	float beforeDepth = (1.0f - height.Sample(sampleType, prevUV)) - prevLayerDepth;
	float weight = afterDepth / (afterDepth + beforeDepth);

	float2 puv = prevUV * weight + currentUV * (1.0f - weight);
	//puv = float2(i.uv.x, 1.0f - i.uv.y);

	
	/* Normal Mapping */
	float3 n = normal.Sample(sampleType, puv) * 2.0f - 1.0f;
	n = normalize(mul(i.tbnMat, n));

	float4 color = baseColor.Sample(sampleType, puv);

	float4 diffuse = color * saturate(dot(lightDir, n)) * 1.2f;

	//float3 r = reflect(-i.lightDir, n);
	float g = 1.0f - roughness.Sample(sampleType, puv);
	float3 h = normalize(lightDir + normalize(viewDir));
	float specular = pow(saturate(dot(h, n)), 16.0f + 16.0f * g) * 0.35f;

	float4 ambient = color * 0.45f + float4(0.27f, 0.7f, 1.0f, 0.0f) * 0.2f;

	o.color = diffuse + specular + ambient;

	return o;
}