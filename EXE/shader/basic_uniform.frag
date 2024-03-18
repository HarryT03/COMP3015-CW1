#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding=0) uniform sampler2D cementTex;
layout (binding=1) uniform sampler2D brickTex;


layout (location = 0) out vec4 FragColor;

uniform struct LightInfo{
	vec3 La;
	vec3 L;
	vec4 Position;
} Light;

uniform struct SpotlightInfo{
	vec3 Position;
	vec3 La;
	vec3 L;
	vec3 Direction;
	float Exponent;
	float Cutoff;
} Spot;

uniform struct MaterialInfo{
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	float Shininess;
	int Choice;
} Material;

uniform struct FogInfo{
	float MaxDist;
	float MinDist;
	vec3 Colour;

} Fog;

const int levels=3;
const float scaleFactor=1.0/float(levels);

vec3 blinnphongAmbient(vec3 position, vec3 n){

		vec3 diffuse = vec3(0), spec = vec3(0);

		vec3 cementTexColour = texture(cementTex, TexCoord).rgb;
		vec3 brickTexColour = texture(brickTex, TexCoord).rgb;
		vec3 texColour;


		if (Material.Choice == 0){
			texColour = cementTexColour;
		}
		else if (Material.Choice == 1){
			texColour = brickTexColour;
		}
		else if (Material.Choice == 2){
			texColour = mix(cementTexColour, brickTexColour, texture(brickTex, TexCoord).a);
		}
		else if (Material.Choice == 3){
			texColour;
		}

		vec3 ambient = Light.La * texColour;
		vec3 s=normalize(vec3(Light.Position.xyz)-position);

		float sDotN=max(dot(s,n),0.0);
		diffuse = texColour * sDotN;

		if(sDotN>0.0){
			vec3 v = normalize(-position.xyz);
			vec3 h = normalize (s+v);
			spec=Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
			}
		return ambient+(diffuse+spec)*Light.L;
}

vec3 blinnphongSpot( vec3 position, vec3 n){

		vec3 diffuse = vec3(0), spec = vec3(0);
		vec3 ambient = Spot.La * Material.Ka;

		vec3 s = normalize(Spot.Position - position);

		float cosAng = dot(-s,normalize(Spot.Direction));
		float angle = acos(cosAng);

		float spotScale;

		if (angle >= 0.0 && angle < Spot.Cutoff){
			spotScale = pow(cosAng,Spot.Exponent);
			float sDotN=max(dot(s,n),0.0);
			diffuse = Material.Kd * floor(sDotN * levels) * scaleFactor;
			if(sDotN > 0.0){
				vec3 v=normalize(-position.xyz);
				vec3 h=normalize(s+v);
				spec=Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
			}

		}
		
		return ambient+spotScale*(diffuse+spec)*Spot.L;
}

void main() {
	
	float dist = abs(Position.z);
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor=clamp(fogFactor, 0.0, 1.0);

	vec3 shadeColour = vec3(0.0);
	
	shadeColour += blinnphongAmbient(Position, normalize(Normal));
	shadeColour += blinnphongSpot(Position, normalize(Normal));

	vec3 Colour = mix(Fog.Colour, shadeColour, fogFactor);
	
    FragColor = vec4(Colour,1.0);
	
	
}
