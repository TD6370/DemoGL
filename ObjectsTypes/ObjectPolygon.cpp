#include "..\ModelData.h"
#include "..\GeometryLib.h"

#include "ObjectPolygon.h"

void ObjectPolygon::FillPlanes()
{
	int indVert = 0;
	int indexPlane = 0;

	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle, 
		Size);

	//while (indVert < ModelPtr->Vertices.size())
	while (indVert < GetVertices().size())
	{
		vec3 vertexNormA = GetVertices()[indVert++];
		vec3 vertexNormB = GetVertices()[indVert++];
		vec3 vertexNormC = GetVertices()[indVert++];

		//glm::vec3 posA = vec3(trans * vec4(vertexNormA, 1));
		//glm::vec3 posB = vec3(trans * vec4(vertexNormB, 1));
		//glm::vec3 poCB = vec3(trans * vec4(vertexNormC, 1));

		Plane plane(vertexNormA, vertexNormB, vertexNormC, trans);
		plane.Index = indexPlane++;
		//Planes->push_back(plane);
		Planes.push_back(std::make_unique<Plane>(plane));
	}
}

void ObjectPolygon::InitData()
{
	ObjectPhysic::InitData();

	ActionObjectCurrent = Lock;
	//FillPlanes();
}
