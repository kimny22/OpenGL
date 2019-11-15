in vec3 Position;
in vec3 Normal;

struct LightInfo{
	vec4 Position;  
	vec3 Intensity;
};
uniform LightInfo Light[2];

struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};
uniform MaterialInfo Material;

out vec4 color;

vec3 phongModel(int n, vec4 P, vec3 N)
{
	vec3 L; 
	if (Light[n].Position.w == 0.0) 
		L = normalize(Light[n].Position.xyz);
	else 
		L = normalize((Light[n].Position - P).xyz);
	vec3 V = normalize(-P.xyz);
	vec3 R = reflect(-L, N);
	float lDotN = max (dot (L, N), 0.0);
	vec3 ambient = Light[n].Intensity * Material.Ka;
	vec3 diffuse = Light[n].Intensity * Material.Kd * lDotN;
	vec3 spec = vec3(0.0);
	if (lDotN >0.0)
		spec = Light[n].Intensity * Material.Ks * pow(max (dot(R,V), 0.0), Material.Shininess);

	return ambient+diffuse+spec;
}

void main(void)
{
	vec3 LightIntensity;
	for(int i=0; i<2; i++)
		LightIntensity += phongModel(i, vec4(Position, 1.0), Normal);
	color = vec4(LightIntensity, 1.0);
}