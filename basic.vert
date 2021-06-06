#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 posNormal;

out vec3 fragmentColor;
out vec3 fragPos;
out vec2 UV;
out vec3 fragNormal;

out float cosTheta;
out float cosAlpha;
out float fragParamCase;
out vec3 fragPosMouse;
out float fragTime;
out float fragLightDist;

// ��������, ������� �������� ����������� ��� ���� �����.
uniform mat4 MVP;
uniform vec3 setColor; 
uniform mat4 VIEW;
uniform mat4 MODEL;
uniform float paramCase;
uniform mat4 transform;
uniform float fTime;
uniform vec3 setPosMouse;
uniform vec3 lightPos;
uniform vec3 filterVectors[10];

void main()
{
	vec3 resColor = setColor; 
	if(paramCase == 55){
		for(int i =0; i<10;i++)
		{
			vec3 posParam = filterVectors[i];
			if(position == posParam)
			{
				resColor = vec3(0,1,0);
			}
		}
	}

	fragNormal = posNormal;
	//-------------------
	// ��������� ������� � ������� ������������: M * ���������
	//vec3 Position_worldspace = (MODEL * vec4(position,1)).xyz;
	//------------------------
	gl_Position = MVP * transform *vec4(position, 1.0);

	//-- Light position
	vec3 LightPosition_worldspace = (inverse(transform) * vec4(lightPos, 1.0)).xyz;

	//------- dist light
	float distLight = distance(lightPos.xyz, (transform *vec4(position, 1.0)).xyz);
	distLight *= 0.05f;


	// ������ ������� ���� �� �������  ������ � ������������ ������
	// � ������������ ������, ������ ��������� �� ��������� (0,0,0)
	vec3 vertexPosition_cameraspace = (VIEW * MODEL * vec4(position,1)).xyz;
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// ������ ������� ���� �� ������� � ��������� ����� � ������������ ������.
	//������� M ���������, ��� ��� ��� � � ���� ������������ ���������.
	vec3 LightPosition_cameraspace = (VIEW * vec4(LightPosition_worldspace,1)).xyz;
	vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	// ������� ������� � ������������ ������
	// ����� �������� ���� � ��� ������, ����� ������� ������ �� �������� � ������.
	vec3 Normal_cameraspace = (VIEW * MODEL * vec4(posNormal,0)).xyz; 

	// ������� ��������� � ������������ ������
	vec3 n = normalize(Normal_cameraspace);
	// ����������� �����(�� ��������� � ��������� �����
	vec3 l = normalize(LightDirection_cameraspace);
	cosTheta = clamp(dot(n,l), 0,1);
	//-----------------
	// ������ �������(� ������� ������)
	vec3 E = normalize(EyeDirection_cameraspace);
	//����������� � ������� ����������� �������� ����
	vec3 R = reflect(-l,n);
	// ������� ���� ����� �������� ������� � �������� ��������� ���������� �� ���� ���� �����
	//  - ������� ���� �� ��������� -> 1
	//  -������� ����-�� � ������ ������� -> < 1
	cosAlpha = clamp( dot( E,R ), 0,1 );
	//--------------------------------------	


	// ---- OUT 
	//fragmentColor = vertexColor;
	fragmentColor = resColor; 
	UV = vertexUV;
	fragParamCase = paramCase;
	fragPosMouse = setPosMouse;
	fragPos= vec3(gl_Position.x,gl_Position.y,gl_Position.z);
	fragTime = fTime;

	fragLightDist = distLight; 
}