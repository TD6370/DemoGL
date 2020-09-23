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

// ��������, ������� �������� ����������� ��� ���� �����.
uniform mat4 MVP;
uniform vec3 setColor; 
uniform mat4 VIEW;
uniform mat4 MODEL;
uniform float paramCase;
uniform mat4 transform;
uniform float fTime;
uniform vec3 setPosMouse;

void main()
{

	fragNormal = posNormal;
	//-------------------
	// ��������� ������� � ������� ������������: M * ���������
	//Position_worldspace = (MODEL * vec4(position,1)).xyz;

	//-- Light position
	//vec3 LightPosition_worldspace= setColor;
	//vec3 LightPosition_worldspace= vec3(sin(fTime),0.0f,cos(fTime));
	vec3 LightPosition_worldspace= vec3(sin(fTime)*2f,0.0f,cos(fTime)*2f);

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

	
	//gl_Position = vec4(position.x, position.y, position.z, 1.0);

	// ---------- �������� ������� ����� �������: MVP * position
	//gl_Position = MVP *vec4(position, 1.0);
	//bug
	//gl_Position = transform *vec4(position, 1.0);
	//gl_Position *= MVP;
	gl_Position = MVP * transform *vec4(position, 1.0);

	// ---- OUT 
	//fragmentColor = vertexColor;
	fragmentColor = setColor; 
	UV = vertexUV;
	fragParamCase = paramCase;
	fragPosMouse = setPosMouse;
	//fragPos= position;
	fragPos= vec3(gl_Position.x,gl_Position.y,gl_Position.z);
	fragTime = fTime;

	//gl_Position = vec4(vertexColor.x, vertexColor.y, vertexColor.z, 1.0);
}