#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2D diffuseTex2;
uniform sampler2D bumpTex2;
uniform sampler2D diffuseTex3;
uniform sampler2D bumpTex3;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex {
	vec3 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	float posY;
} IN;

vec4 getTexture(){
	if (IN.posY <= 275.0f) {
		return texture(diffuseTex, IN.texCoord);
	}
	if (IN.posY > 275.0f && IN.posY < 375.0f) {
		float diff = 100.0f;
		float nextF = (IN.posY - 275.0f) / diff;
		float currentF = 1.0f - nextF;

		vec4 newTex = vec4(0.0);
		newTex = (texture(diffuseTex, IN.texCoord)*currentF) + (texture(diffuseTex2, IN.texCoord)*nextF);
		return newTex;

	}
	if (IN.posY > 375.0f && IN.posY < 450.0f) {
		return texture(diffuseTex2, IN.texCoord);
	}
	if (IN.posY >= 450.0f && IN.posY < 550.0f) {
		float diff2 = 100.0f;
		float nextF2 = (IN.posY - 450.0f) / diff2;
		float currentF2 = 1.0f - nextF2;

		vec4 newTex2 = vec4(0.0);
		newTex2 = (texture(diffuseTex2, IN.texCoord)*currentF2) + (texture(diffuseTex3, IN.texCoord)*nextF2);
		return newTex2;
	}
	if (IN.posY >= 550.0f) {
		return texture(diffuseTex3, IN.texCoord);
	}
}

//vec3 getBump(){
//	if (IN.posY < 325.0f) {
//		return texture(bumpTex, IN.texCoord).rgb;
//	}
//	if (IN.posY > 325.0f && IN.posY < 500.0f) {
//		return texture(bumpTex2, IN.texCoord).rgb;
//	}
//	else {
//		return texture(bumpTex3, IN.texCoord).rgb;
//	}
//}

vec3 getBump(){
	if (IN.posY <= 275.0f) {
		return texture(bumpTex, IN.texCoord).rgb;
	}
	if (IN.posY > 275.0f && IN.posY < 375.0f) {
		float diff = 100.0f;
		float nextF = (IN.posY - 275.0f) / diff;
		float currentF = 1.0f - nextF;

		vec3 newTex = vec3(0.0);
		newTex = (texture(bumpTex, IN.texCoord).rgb*currentF) + (texture(bumpTex2, IN.texCoord).rgb*nextF);
		return newTex;

	}
	if (IN.posY > 375.0f && IN.posY < 450.0f) {
		return texture(bumpTex2, IN.texCoord).rgb;
	}
	if (IN.posY >= 450.0f && IN.posY < 550.0f) {
		float diff2 = 100.0f;
		float nextF2 = (IN.posY - 450.0f) / diff2;
		float currentF2 = 1.0f - nextF2;

		vec3 newTex2 = vec3(0.0);
		newTex2 = (texture(bumpTex2, IN.texCoord).rgb*currentF2) + (texture(bumpTex3, IN.texCoord).rgb*nextF2);
		return newTex2;
	}
	if (IN.posY >= 550.0f) {
		return texture(bumpTex3, IN.texCoord).rgb;
	}
}

out vec4 fragColour;

void main(void) {
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	mat3 TBN =	mat3 (normalize(IN.tangent),
				normalize(IN.binormal), normalize(IN.normal));

	vec4 diffuse = getTexture();
	vec3 bumpNormal = getBump();
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	float lambert = max(dot(incident, IN.normal), 0.0f);
	float distance = length(lightPos - IN.worldPos);
	float attenuation = 1.0 - clamp(distance / lightRadius, 0.0, 1.0);
	float specFactor = clamp(dot(halfDir, bumpNormal), 0.0, 1.0);

	specFactor = pow(specFactor, 60.0);
	vec3 surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert * attenuation;
	fragColour.rgb += (lightColour.rgb * specFactor)*attenuation*0.33;
	fragColour.rgb += surface * 0.1f; // ambient !
	fragColour.a = diffuse.a;
}