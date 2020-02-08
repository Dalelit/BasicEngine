cbuffer sceneConstObj {
	float4 ambientColor;	// background light
	float4 lightDirection;	// scene directional light - direction
	float4 lightColor;		// scene directional light - color
	float4 pointLightPosition;
	float4 pointLightColor;
	float4 cameraPosition;
};
