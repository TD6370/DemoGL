
#include "ObjectBlock.h"

//#include "..\CreatorModelData.h"
//#include "..\TransformModel.h"
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "ObjectDynamic.h"
#include "..\Serialize\SceneSerialize.h"
#include "../GeomertyShapes/ShapeHexagon.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../CreatorModelData.h"
#include "../Components/RenderComponent.h"

void ObjectBlock::InitData()
{
	Shape = new ShapeHexagon();

	ActionObjectCurrent = Stay;
	IsGravity = true;

	ObjectPhysic::InitData();

	IsTextureRepeat = true;

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
	Move.y = PlaneDownPosition.y + ModelPtr->MeshData.RadiusCollider;
	NewPostranslate = Move;

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

	if (EngineData->Inputs->Key == KeyUpTopVertex || EngineData->Inputs->Key == KeyDownTopVertex) {
		
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

	Render->ResetMem_Vertex();

	//-- Param transform
	int indexUpdate = IndexVertexTransform;

	//-- target transform
	vec3 newPos = Target;

	vec3 offsetPosScene = newPos - Postranslate;
	vec3 vertOffset = vec3(offsetPosScene.x, 0, offsetPosScene.z);

	//-- set transform
	if (MeshData.Vertices.size() != 0) {

		vec3 vertA = Shape->GetBottom(indexUpdate);

		vertA = vec3(vertOffset.x, vertA.y, vertOffset.z);
		Shape->SetBottom(indexUpdate, vertA);

		vec3 vertB = Shape->GetTop(indexUpdate);
		vertB = vec3(vertOffset.x, vertB.y, vertOffset.z);
		Shape->SetTop(indexUpdate, vertB);

		SaveNewPosition();

		GetShapeHexagon()->CalculateTextureUV(false);
	}
}

vector<ObjectFiledsSpecific> ObjectBlock::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();
	BlockFileds* filedsName = new BlockFileds;
	vector<ObjectFiledsSpecific> result;

	for(auto vertex : MeshData.Vertices)
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

	Render->ResetMem_Vertex();
	MeshData.Vertices.clear();
	//TextureUV.clear();

	for (auto fieldValue : filedsSpecific)
	{
		if(fieldValue.FieldName == filedsName->Vertex)
			MeshData.Vertices.push_back(serializer->StrToVec3(fieldValue.Value));

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
