#include "ShapeBase.h"

ShapeBase::ShapeBase() {

}

ShapeBase::~ShapeBase() {

}

vec3 ShapeBase::GetVertexWorldPosition(ObjectData* obj, int indVertex)
{
	std::vector<vec3 > verticesModel = obj->GetVertices();
	if (verticesModel.size() - 1 < indVertex)
		return vec3(-1);

	vec3 posNorm = verticesModel[indVertex];

	return ToWorldPosition(obj, posNorm);
}

vec3 ShapeBase::ToWorldPosition(ObjectData* obj, vec3 pos) {
	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		obj->Postranslate,
		obj->TranslateAngle,
		obj->Size);
	glm::vec3 worldPos = vec3(trans * vec4(pos, 1));
	return worldPos;
}

string ShapeBase::GetKeySectorPolygon(ObjectData* obj, bool isNewPosition) {
	vec3 pos = vec3();
	if (isNewPosition)
		pos = obj->NewPostranslate;
	else
		pos = obj->Postranslate;

	int x_sector = pos.x / obj->Storage->Clusters->SectorSizePlane;
	int z_sector = pos.z / obj->Storage->Clusters->SectorSizePlane;
	return std::to_string(x_sector) + "_" + std::to_string(z_sector);
}


void ShapeBase::FillPlanes(ObjectData* obj)
{
	int indVert = 0;
	int indexPlane = 0;

	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		obj->Postranslate,
		obj->TranslateAngle,
		obj->Size);

	while (indVert < obj->GetVertices().size())
	{
		vec3 vertexNormA = obj->GetVertices()[indVert++];
		vec3 vertexNormB = obj->GetVertices()[indVert++];
		vec3 vertexNormC = obj->GetVertices()[indVert++];

		Plane plane(vertexNormA, vertexNormB, vertexNormC, trans);
		plane.Index = indexPlane++;
		Planes.push_back(std::make_unique<Plane>(plane));
	}
}


void ShapeBase::FillVertextBox(ObjectPhysic* obj)
{
	std::vector<vec3> CubeVectors;
	int indexPlaneT = 0;
	int indexPlaneB = 0;
	int indVert = 0;

	obj->BottomVectors.clear();
	obj->TopVectors.clear();

	while (indVert < obj->GetVertices().size())
	{
		vec3 vertexNext = obj->GetVertices()[indVert];
		indVert++;
		bool isExist = std::find(CubeVectors.begin(), CubeVectors.end(), vertexNext) != CubeVectors.end();
		if (isExist)
			continue;

		CubeVectors.push_back(vertexNext);
		if (vertexNext.y < 0) {

			//TODO:	BottomVectors -> Shape
			obj->BottomVectors.insert(std::pair<int, vec3>(indexPlaneB, vertexNext));
			indexPlaneB++;
		}
		else {
			obj->TopVectors.insert(std::pair<int, vec3>(indexPlaneT, vertexNext));
			indexPlaneT++;
		}
	}
}
