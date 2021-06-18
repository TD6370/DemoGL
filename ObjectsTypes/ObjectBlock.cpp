
#include "ObjectBlock.h"

//#include "..\CreatorModelData.h"
//#include "..\TransformModel.h"
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "ObjectDynamic.h"
#include "..\Serialize\SceneSerialize.h"
#include "../GeomertyShapes/ShapeHexagon.h"
#include "../GeomertyShapes/ShapeBase.h"

void ObjectBlock::InitData()
{
	/*if (IsSquareModel) {
		Shape = new ShapeHexagon();
	}*/
	Shape = new ShapeHexagon();

	ActionObjectCurrent = Stay;
	IsGravity = true;
	//Vertices = ModelPtr->Vertices;

	ObjectPhysic::InitData();

	IsTextureRepeat = true;
	//CalculateTextureUV(true);
	GetShapeHexagon()->CalculateTextureUV(true);
}

void ObjectBlock::LockPolygonResult() {
	SaveNewPosition();
}

void ObjectBlock::LockResult() {
	SaveNewPosition();
}

void ObjectBlock::SaveNewPosition()
{
	ActionObjectCurrent = Stay;
	Move = Postranslate;
	//Set position polygon
	Move.y = PlaneDownPosition.y + ModelPtr->RadiusCollider;
	NewPostranslate = Move;

	//Storage->Clusters->SaveClusterObject(Index);
	SaveToCluster();

	Postranslate = NewPostranslate;
}

void ObjectBlock::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
			case Starting:
				GenStartPosition();
				break;
			case Stay:
				TestGravity();
				break;
			default:
				break;
		}
	}
	ObjectData::ActionBase();
}

void ObjectBlock::ControlsEvents() {

	if (!IsSelected)
		return;

	if (Storage->Inputs->Key == KeyUpTopVertex || Storage->Inputs->Key == KeyDownTopVertex) {
		
		GetShapeHexagon()->ResizeVerticaleWall(KeyUpTopVertex, KeyDownTopVertex);
	}
}

void ObjectBlock::TestGravity()
{
	if (Postranslate == Move)
		return;
	CheckIsLock();
}

void ObjectBlock::SelectedEvent() {

	ObjectPhysic::SelectedEvent();
	GetShapeHexagon()->SelectVertexBlock();
}

void ObjectBlock::MeshTransform() {

	int indCursor = Storage->SceneData->IndexCursorRayObj;
	std::shared_ptr<ObjectData> CusrorObj = Storage->GetObjectPrt(indCursor);
	
	//-- Param transform
	int indexUpdate = IndexVertexTransform;
	
	//-- Calculate transform
	vec3 newPos = CusrorObj->Postranslate;
	vec3 offsetPosScene = newPos - Postranslate;
	vec3 vertOffset = vec3(offsetPosScene.x, 0, offsetPosScene.z);

	//-- set transform
	if (Vertices.size() != 0) {

		//vec3 vertA =  GetBottom(indexUpdate);
		vec3 vertA = Shape->GetBottom(indexUpdate);

		vertA = vec3(vertOffset.x, vertA.y, vertOffset.z);
		//Shape->SetBottom(this, indexUpdate, vertA);
		Shape->SetBottom(indexUpdate, vertA);

		vec3 vertB = Shape->GetTop(indexUpdate);
		vertB = vec3(vertOffset.x, vertB.y, vertOffset.z);
		//Shape->SetTop(this, indexUpdate, vertB);
		Shape->SetTop(indexUpdate, vertB);

		SaveNewPosition();

		//CalculateTextureUV(false);
		GetShapeHexagon()->CalculateTextureUV(false);
	}
}

vector<ObjectFiledsSpecific> ObjectBlock::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();
	BlockFileds* filedsName = new BlockFileds;
	vector<ObjectFiledsSpecific> result;

	for(auto vertex : Vertices)
	{
		result.push_back({ filedsName->Vertex ,serializer->Vec3Str(vertex) });
	}
	// --- ver1.UV
	/*for (auto uv : TextureUV)
	{
		result.push_back({ filedsName->UV,serializer->Vec2Str(uv) });
	}*/
	return result;
}

void ObjectBlock::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (filedsSpecific.size() == 0)
		return;

	SceneSerialize* serializer = new SceneSerialize();
	BlockFileds* filedsName = new BlockFileds;

	Vertices.clear();
	//TextureUV.clear();

	for (auto fieldValue : filedsSpecific)
	{
		if(fieldValue.FieldName == filedsName->Vertex)
			Vertices.push_back(serializer->StrToVec3(fieldValue.Value));

		// --- ver1.UV
		//if (fieldValue.FieldName == filedsName->UV)
		//	TextureUV.push_back(serializer->StrToVec2(fieldValue.Value));
	}

	//ObjectPhysic::FillPlanesCube();
	//GetShapeHexagon()->FillPlanesCube(this);
	//FillPlanesCube(this);

	//++++++++++++++++++++++++++++++++ #????#
	ObjectPhysic* objPhysic = static_cast<ObjectPhysic*>(this);
	Shape->FillVertextBox();

	// --- ver2.UV
	GetShapeHexagon()->CalculateTextureUV(false); 
}
