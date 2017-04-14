// PX2SceneNodeCtrl.cpp

#include "PX2SceneNodeCtrl.hpp"
#include "PX2Edit.hpp"
#include "PX2GeoObjFactory.hpp"
#include "PX2Project.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Time.hpp"
#include "PX2Renderer.hpp"
#include "PX2Picker.hpp"
#include "PX2Canvas.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2TriMesh.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2Canvas.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2Canvas.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SceneNodeCtrl::SceneNodeCtrl()
{
	mOriginScale = 1.0f;

	GeoObjFactory factory;
	mCtrl_P_Translate = factory.CreateTranslateCtrl_P();
	mCtrl_P_Rolate = factory.CreateRolateCtrl_P();
	mCtrl_P_Scale = factory.CreateScaleCtrl_P();
	mCtrl_O_Translate = factory.CreateTranslateCtrl_O();
	mCtrl_O_Rolate = factory.CreateRolateCtrl_O();
	mCtrl_O_Scale = factory.CreateScaleCtrl_O();
	NodePtr nodeEmpty = new0 Node();

	mCtrlsGroup = new0 SwitchNode();
	mCtrlsGroup->WorldTransformIsCurrent = true;
	mOriginScale = mCtrlsGroup->WorldTransform.GetUniformScale();
	mCtrlsGroup->AttachChild(mCtrl_P_Translate);
	mCtrlsGroup->AttachChild(mCtrl_P_Rolate);
	mCtrlsGroup->AttachChild(mCtrl_P_Scale);
	mCtrlsGroup->AttachChild(mCtrl_O_Translate);
	mCtrlsGroup->AttachChild(mCtrl_O_Rolate);
	mCtrlsGroup->AttachChild(mCtrl_O_Scale);
	mCtrlsGroup->AttachChild(nodeEmpty);

	mLookType = LT_PERSPECTIVE;
	mCtrlType = CT_SELECT;
	mDragType = DT_NONE;

	mAxisMode = AM_WORLD;
	mDirX = AVector::UNIT_X;
	mDirY = AVector::UNIT_Y;
	mDirZ = AVector::UNIT_Z;
}
//----------------------------------------------------------------------------
SceneNodeCtrl::~SceneNodeCtrl()
{
	PX2_EW.GoOut(this);
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::SetAxisMode(SceneNodeCtrl::AxisMode mode)
{
	mAxisMode = mode;

	UpdateCtrl();
	UpdateCtrlTrans();
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::SetLookType(SceneNodeCtrl::LookType type)
{
	mLookType = type;

	UpdateCtrl();
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::SetCtrlType(SceneNodeCtrl::CtrlType type)
{
	mCtrlType = type;

	UpdateCtrl();
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::SetDragType(DragType type)
{
	mDragType = type;
}
//----------------------------------------------------------------------------
bool SceneNodeCtrl::IsDragNone() const
{
	return mDragType == DT_NONE;
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::OnLeftDown(Camera *camera, const APoint &viewPortPos,
	const Sizef &size)
{
	Edit::EditType editType = PX2_EDIT.GetEditType();
	if (Edit::ET_SCENE != editType) return;

	DragType dt = GetDragType(camera, viewPortPos, size);
	SetDragType(dt);
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::OnMouseWheel(Camera *camera, float wheelDelta, 
	const Sizef &size)
{
	PX2_UNUSED(size);
	PX2_UNUSED(wheelDelta);

	float rmax = camera->GetRMax();
	APoint camPosition = camera->GetPosition();
	APoint ctrlPosition = GetPosition();
	AVector diff = ctrlPosition - camPosition;
	float diffLength = diff.Length();

	if (mLookType != LT_PERSPECTIVE)
	{
		if (0.0f != rmax)
		{
			mCtrlsGroup->WorldTransform.SetUniformScale(rmax*0.11f);
			mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
		}
	}
	else
	{
		float scale = diffLength*0.04f;
		if (scale == 0.0f) scale = 0.0001f;
		if (scale < 1.0f) scale = 1.0f;
		mCtrlsGroup->WorldTransform.SetUniformScale(scale);
		mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
	}
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::OnMotionCalDragType(Camera *camera, APoint posNow,
	const Sizef &size)
{
	// 光标移动更新
	if (DT_NONE == mDragType)
	{
		GeoObjFactory factory;

		DragType dt = GetDragType(camera, posNow, size);
		Movable *ctrlMov = 0;
		Float4 colorYellowAlpha = Float4(1.0f, 1.0f, 0.0f, 0.3f);

		if (DT_X == dt)
		{
			ctrlMov = GetCurrentCtrlX();
			factory.UpdateCtrlColor(ctrlMov, Float4::YELLOW);
		}
		else if (DT_Y == dt)
		{
			ctrlMov = GetCurrentCtrlY();
			factory.UpdateCtrlColor(ctrlMov, Float4::YELLOW);
		}
		else if (DT_Z == dt)
		{
			ctrlMov = GetCurrentCtrlZ();
			factory.UpdateCtrlColor(ctrlMov, Float4::YELLOW);
		}
		else if (DT_XY == dt)
		{
			factory.UpdateCtrlColor1(GetCurrentCtrlXY(), colorYellowAlpha);
			factory.UpdateCtrlColor1(GetCurrentCtrlYZ(), Float4::ZERO);
			factory.UpdateCtrlColor1(GetCurrentCtrlXZ(), Float4::ZERO);
		}
		else if (DT_YZ == dt)
		{
			factory.UpdateCtrlColor1(GetCurrentCtrlYZ(), colorYellowAlpha);
			factory.UpdateCtrlColor1(GetCurrentCtrlXY(), Float4::ZERO);
			factory.UpdateCtrlColor1(GetCurrentCtrlXZ(), Float4::ZERO);
		}
		else if (DT_XZ == dt)
		{
			factory.UpdateCtrlColor1(GetCurrentCtrlXZ(), colorYellowAlpha);
			factory.UpdateCtrlColor1(GetCurrentCtrlXY(), Float4::ZERO);
			factory.UpdateCtrlColor1(GetCurrentCtrlYZ(), Float4::ZERO);
		}
		else if (DT_XYZ == dt)
		{
			factory.UpdateCtrlColor1(GetCurrentCtrlXYZ(), Float4::YELLOW);
		}
		else if (DT_NONE == dt)
		{
			factory.UpdateCtrlColor(GetCurrentCtrlX(), Float4::RED);
			factory.UpdateCtrlColor(GetCurrentCtrlY(), Float4::GREEN);
			factory.UpdateCtrlColor(GetCurrentCtrlZ(), Float4::BLUE);
			factory.UpdateCtrlColor1(GetCurrentCtrlXY(), Float4::ZERO);
			factory.UpdateCtrlColor1(GetCurrentCtrlYZ(), Float4::ZERO);
			factory.UpdateCtrlColor1(GetCurrentCtrlXZ(), Float4::ZERO);
			factory.UpdateCtrlColor1(GetCurrentCtrlXYZ(), Float4::WHITE);
		}

		if (DT_NONE == dt)
		{
			RenderWindow::SetCursorType(RenderWindow::CT_CURSOR_ARROW);
		}
		else
		{
			RenderWindow::SetCursorType(RenderWindow::CT_CURSOR_SIZING);
		}
	}
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::OnDraging(Camera *camera, APoint posNow, APoint posBefore, 
	const Sizef &size)
{
	if (DT_NONE == mDragType) return;

	int numObjs = PX2_SELECTM_E->GetNumObjects();
	if (0 == numObjs) return;

	// get pickPoint with the plane
	TriMesh *meshHelp = PX2_GR.GetXYPlane();
	if (DT_X == mDragType)
	{
		if (LT_PERSPECTIVE == mLookType || LT_TOP == mLookType)
			meshHelp = PX2_GR.GetXYPlane();
		else if (LT_FRONT == mLookType)
			meshHelp = PX2_GR.GetXZPlane();
	}
	else if (DT_Y == mDragType)
	{
		meshHelp = PX2_GR.GetXYPlane();
	}
	else if (DT_Z == mDragType)
	{
		AVector cameraDir = camera->GetDVector();
		cameraDir.Normalize();
		float dotVal = Mathf::FAbs(cameraDir.Dot(AVector::UNIT_X));
		if (dotVal > 0.7f)
		{
			meshHelp = PX2_GR.GetYZPlane();
		}
		else
		{
			meshHelp = PX2_GR.GetXZPlane();
		}
	}
	else if (DT_XY == mDragType)
	{
		meshHelp = PX2_GR.GetXYPlane();
	}
	else if (DT_YZ == mDragType)
	{
		meshHelp = PX2_GR.GetYZPlane();
	}
	else if (DT_XZ == mDragType)
	{
		meshHelp = PX2_GR.GetXZPlane();
	}
	else if (DT_XYZ == mDragType)
	{
		meshHelp = PX2_GR.GetXYPlane();
	}
	meshHelp->WorldTransform.SetTranslate(GetPosition());
	HMatrix matRot;
	matRot.SetColumn(0, mDirX);
	matRot.SetColumn(1, mDirY);
	matRot.SetColumn(2, mDirZ);
	AVector::Orthonormalize(mDirX, mDirY, mDirZ);
	AVector vec;
	vec.Set(0.0f, 0.0f, 0.0f, 1.0f);
	matRot.SetColumn(3, vec);
	meshHelp->WorldTransform.SetRotate(matRot);

	// get pick ray
	APoint rayOrigin_Now;
	AVector rayDir_Now;
	camera->GetPickRay(posNow.X(), posNow.Z(), size, rayOrigin_Now, rayDir_Now);

	APoint rayOrigin_Before;
	AVector rayDir_Before;
	camera->GetPickRay(posBefore.X(), posBefore.Z(), size, rayOrigin_Before, rayDir_Before);

	// pick
	Picker pickerNow;
	pickerNow.Execute(meshHelp, rayOrigin_Now, rayDir_Now, 0.0f, Mathf::MAX_REAL);
	float lengthNow = pickerNow.GetClosestToZero().T;
	APoint positionNow(rayOrigin_Now + rayDir_Now*lengthNow);

	Picker pickerOrigin;
	pickerOrigin.Execute(meshHelp, rayOrigin_Before, rayDir_Before, 0.0f, Mathf::MAX_REAL);
	float lengthBefore = pickerOrigin.GetClosestToZero().T;
	APoint positionBefore(rayOrigin_Before + rayDir_Before*lengthBefore);

	if (pickerNow.Records.empty() || pickerOrigin.Records.empty())
		return;

	AVector transMoved = positionNow - positionBefore;
	AVector transDir = transMoved;
	transDir.Normalize();

	float transValue = 0.0f;
	float transValue1 = 0.0f;
	AVector transVec;
	AVector rolateVec;

	AVector dirX = mDirX;
	AVector dirY = mDirY;
	AVector dirZ = mDirZ;

	if (DT_X == mDragType)
	{
		transValue = transMoved.Dot(dirX);
		transVec = dirX * transValue;

		rolateVec.X() = transMoved.Length() *(1.0f - Mathf::FAbs(transDir.Dot(dirX)));

		AVector vec = transDir.Cross(dirX);
		rolateVec.X() *= Mathf::Sign(vec.Z());
	}
	else if (DT_Y == mDragType)
	{
		transValue = transMoved.Dot(dirY);
		transVec = dirY * transValue;

		rolateVec.Y() = transMoved.Length() *(1.0f - Mathf::FAbs(transDir.Dot(dirY)));

		AVector vec = transDir.Cross(dirY);
		rolateVec.Y() *= Mathf::Sign(vec.Z());
	}
	else if (DT_Z == mDragType)
	{
		transValue = transMoved.Dot(dirZ);
		transVec = dirZ * transValue;

		rolateVec.Z() = transMoved.Length() *(1.0f - Mathf::FAbs(transDir.Dot(dirZ)));

		rolateVec.Z() *= Mathf::Sign(posNow.X() - posBefore.X());
	}
	else if (DT_XY == mDragType)
	{
		transValue = transMoved.Dot(dirX);
		transValue1 = transMoved.Dot(dirY);
		transVec = dirX * transValue + dirY * transValue1;
	}
	else if (DT_YZ == mDragType)
	{
		transValue = transMoved.Dot(dirY);
		transValue1 = transMoved.Dot(dirZ);
		transVec = dirY * transValue + dirZ * transValue1;
	}
	else if (DT_XZ == mDragType)
	{
		transValue = transMoved.Dot(dirX);
		transValue1 = transMoved.Dot(dirZ);
		transVec = dirX * transValue + dirZ * transValue1;
	}
	else if (DT_XYZ == mDragType)
	{
		float transValue0 = Mathf::FAbs(transMoved.Dot(dirX));
		float transValue1 = Mathf::FAbs(transMoved.Dot(dirY));
		float transValue2 = Mathf::FAbs(transMoved.Dot(dirZ));

		float trans = (transValue0 + transValue1 + transValue2) / 3.0f;
		trans *= Mathf::Sign(transMoved.Y());

		transVec = AVector(trans, trans, trans);
	}

	if (CT_SCALE == mCtrlType)
		transVec *= 0.5f;

	//HMatrix parentMat = mParentRotateMat.Inverse();
	//AVector localTransVec = parentMat * transVec;

	Transform invers = mParentWorldTrans.InverseTransform();
	AVector localTransVec = invers * transVec;

	if (CT_TRANSLATE == mCtrlType)
	{
		PX2_SELECTM_E->Translate(localTransVec);

		UpdateCtrlTrans();
	}
	else if (CT_ROLATE == mCtrlType)
	{
		AVector degree2Radius = rolateVec;
		degree2Radius[0] = degree2Radius[0] * Mathf::DEG_TO_RAD * 10.0f;
		degree2Radius[1] = degree2Radius[1] * Mathf::DEG_TO_RAD * 10.0f;
		degree2Radius[2] = degree2Radius[2] * Mathf::DEG_TO_RAD * 10.0f;

		PX2_SELECTM_E->AddRolate(degree2Radius);
	}
	else if (CT_SCALE == mCtrlType)
	{
		PX2_SELECTM_E->AddScale(localTransVec);
	}

	Object *obj = PX2_SELECTM_E->GetFirstObject();
	if (obj)
	{
		Event *ent = EditorEventSpace::CreateEventX(
			EditorEventSpace::ObjectTransformChanged);
		ent->SetData<Object*>(obj);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
	}

	mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlX()
{
	int index = mCtrlsGroup->GetActiveChild();
	Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

	if (!node)
		return 0;

	return node->GetChild(0);
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlY()
{
	int index = mCtrlsGroup->GetActiveChild();
	Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

	if (!node)
		return 0;

	return node->GetChild(1);
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlZ()
{
	int index = mCtrlsGroup->GetActiveChild();
	Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

	if (!node)
		return 0;

	return node->GetChild(2);
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlXYZ()
{
	if (mCtrlType == CT_SCALE)
	{
		int index = mCtrlsGroup->GetActiveChild();
		Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

		if (node)
		{
			return node->GetChild(3);
		}

	}

	return 0;
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlXY()
{
	int index = mCtrlsGroup->GetActiveChild();
	Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

	if (node)
	{
		return node->GetChild(3);
	}

	return 0;
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlYZ()
{
	int index = mCtrlsGroup->GetActiveChild();
	Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

	if (node)
	{
		return node->GetChild(4);
	}

	return 0;
}
//----------------------------------------------------------------------------
Movable *SceneNodeCtrl::GetCurrentCtrlXZ()
{
	int index = mCtrlsGroup->GetActiveChild();
	Node *node = DynamicCast<Node>(mCtrlsGroup->GetChild(index));

	if (node)
	{
		return node->GetChild(5);
	}

	return 0;
}
//----------------------------------------------------------------------------
APoint SceneNodeCtrl::GetPosition()
{
	return mCtrlsGroup->WorldTransform.GetTranslate();
}
//----------------------------------------------------------------------------
AVector SceneNodeCtrl::GetXDir()
{
	HPoint vector;
	mCtrlsGroup->WorldTransform.GetRotate().GetRow(0, vector);
	return AVector(vector.X(), vector.Y(), vector.Z());
}
//----------------------------------------------------------------------------
AVector SceneNodeCtrl::GetYDir()
{
	HPoint vector;
	mCtrlsGroup->WorldTransform.GetRotate().GetRow(1, vector);
	return AVector(vector.X(), vector.Y(), vector.Z());
}
//----------------------------------------------------------------------------
AVector SceneNodeCtrl::GetZDir()
{
	HPoint vector;
	mCtrlsGroup->WorldTransform.GetRotate().GetRow(2, vector);
	return AVector(vector.X(), vector.Y(), vector.Z());
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::OnEvent(Event *event)
{
	if (EditES::IsEqual(event, EditES::AddSelect))
	{
		UpdateCtrl();
		UpdateCtrlTrans();
	}
	else if (EditES::IsEqual(event, EditES::RemoveSelect))
	{
		UpdateCtrl();
		UpdateCtrlTrans();
	}
	else if (EditES::IsEqual(event, EditES::RemoveAllSelects))
	{
		UpdateCtrl();
	}
	else if (EditES::IsEqual(event, EditES::UnDo) ||
		EditES::IsEqual(event, EditES::ReDo))
	{
		UpdateCtrlTrans();
		UpdateCtrl();
	}
	else if (EditorEventSpace::IsEqual(event,
		EditorEventSpace::ObjectTransformChanged))
	{
		UpdateCtrlTrans();
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::SetEditAxisMode))
	{
		Edit::EditAxisMode mode = PX2_EDIT.GetEdtiAxisMode();

		if (Edit::EAM_WORLD == mode)
		{
			SetAxisMode(AM_WORLD);
		}
		else if (Edit::EAM_LOCAL == mode)
		{
			SetAxisMode(AM_LOCAL);
		}
		else if (Edit::EAM_PARENT == mode)
		{
			SetAxisMode(AM_PARENT);
		}
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::SetEditMode))
	{
		Edit::EditMode mode = PX2_EDIT.GetEditMode();

		if (Edit::EM_SELECT == mode)
		{
			SetCtrlType(SceneNodeCtrl::CT_SELECT);
		}
		else if (Edit::EM_TRANSLATE == mode)
		{
			SetCtrlType(SceneNodeCtrl::CT_TRANSLATE);
		}
		else if (Edit::EM_ROTATE == mode)
		{
			SetCtrlType(SceneNodeCtrl::CT_ROLATE);
		}
		else if (Edit::EM_SCALE == mode)
		{
			SetCtrlType(SceneNodeCtrl::CT_SCALE);
		}
	}
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::UpdateCtrl()
{
	int numObjects = PX2_SELECTM_E->GetNumObjects();

	bool isObjectsHasMovable = false;
	for (int i = 0; i < numObjects; i++)
	{
		Object *obj = PX2_SELECTM_E->GetObjectAt(i);
		Movable *mov = DynamicCast<Movable>(obj);
		if (mov)
		{
			isObjectsHasMovable = true;
		}
	}

	if (isObjectsHasMovable)
	{	
		if (mCtrlType == CT_SELECT)
		{
			mCtrlsGroup->SetActiveChild(6);
		}
		else if (mCtrlType == CT_TRANSLATE)
		{
			if (mLookType == LT_PERSPECTIVE)
			{
				mCtrlsGroup->SetActiveChild(0);
				GetCurrentCtrlX()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlY()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlZ()->Culling = Movable::CULL_DYNAMIC;
			}
			else
			{
				mCtrlsGroup->SetActiveChild(3);
				GetCurrentCtrlX()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlY()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlZ()->Culling = Movable::CULL_DYNAMIC;

				if (mLookType == LT_TOP)
				{
					GetCurrentCtrlZ()->Culling = Movable::CULL_ALWAYS;
				}
				else if (mLookType == LT_FRONT)
				{
					GetCurrentCtrlY()->Culling = Movable::CULL_ALWAYS;
				}
				else if (mLookType == LT_LEFT)
				{
					GetCurrentCtrlX()->Culling = Movable::CULL_ALWAYS;
				}
			}
		}
		else if (mCtrlType == CT_ROLATE)
		{
			if (mLookType == LT_PERSPECTIVE)
			{
				mCtrlsGroup->SetActiveChild(1);
				GetCurrentCtrlX()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlY()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlZ()->Culling = Movable::CULL_DYNAMIC;
			}
			else
			{
				mCtrlsGroup->SetActiveChild(4);
				GetCurrentCtrlX()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlY()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlZ()->Culling = Movable::CULL_DYNAMIC;

				if (mLookType == LT_TOP)
				{
					GetCurrentCtrlZ()->Culling = Movable::CULL_ALWAYS;
				}
				else if (mLookType == LT_FRONT)
				{
					GetCurrentCtrlY()->Culling = Movable::CULL_ALWAYS;
				}
				else if (mLookType == LT_LEFT)
				{
					GetCurrentCtrlX()->Culling = Movable::CULL_ALWAYS;
				}
			}

			mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
		}
		else if (mCtrlType == CT_SCALE)
		{
			if (mLookType == LT_PERSPECTIVE)
			{
				mCtrlsGroup->SetActiveChild(2);
				GetCurrentCtrlX()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlY()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlZ()->Culling = Movable::CULL_DYNAMIC;
			}
			else
			{
				mCtrlsGroup->SetActiveChild(5);
				GetCurrentCtrlX()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlY()->Culling = Movable::CULL_DYNAMIC;
				GetCurrentCtrlZ()->Culling = Movable::CULL_DYNAMIC;

				if (mLookType == LT_TOP)
				{
					GetCurrentCtrlZ()->Culling = Movable::CULL_ALWAYS;
				}
				else if (mLookType == LT_FRONT)
				{
					GetCurrentCtrlY()->Culling = Movable::CULL_ALWAYS;
				}
				else if (mLookType == LT_LEFT)
				{
					GetCurrentCtrlX()->Culling = Movable::CULL_ALWAYS;
				}
			}
		}
	}
	else
	{
		mCtrlsGroup->SetActiveChild(6);
	}

	mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
}
//----------------------------------------------------------------------------
void SceneNodeCtrl::UpdateCtrlTrans()
{
	int numObjects = PX2_SELECTM_E->GetNumObjects();

	bool isObjectsHasMovable = false;

	for (int i = 0; i < numObjects; i++)
	{
		Object *obj = PX2_SELECTM_E->GetObjectAt(i);
		Movable *mov = DynamicCast<Movable>(obj);
		if (mov)
		{
			isObjectsHasMovable = true;
		}
	}

	mParentRotateMat.MakeIdentity();

	if (!isObjectsHasMovable)
	{
		mCtrlsGroup->SetActiveChild(6);
	}
	else
	{
		if (1 == numObjects)
		{
			Movable *mov = DynamicCast<Movable>(PX2_SELECTM_E->GetFirstObject());
			if (mov)
			{
				APoint movTrans = mov->WorldTransform.GetTranslate();

				if (AM_WORLD == mAxisMode)
				{
					mCtrlsGroup->WorldTransform.SetRotate(0.0f, 0.0f, 0.0f);
					mDirX = AVector::UNIT_X;
					mDirY = AVector::UNIT_Y;
					mDirZ = AVector::UNIT_Z;
					mCtrlsGroup->WorldTransform.SetTranslate(movTrans);
				}
				else if (AM_LOCAL == mAxisMode)
				{
					mov->WorldTransform.GetRotate().GetColumn(0, mDirX);
					mov->WorldTransform.GetRotate().GetColumn(1, mDirY);
					mov->WorldTransform.GetRotate().GetColumn(2, mDirZ);
					mDirX.Normalize();
					mDirY.Normalize();
					mDirZ.Normalize();
					HMatrix matRot;
					matRot.SetColumn(0, mDirX);
					matRot.SetColumn(1, mDirY);
					matRot.SetColumn(2, mDirZ);
					AVector vec;
					vec.Set(0.0f, 0.0f, 0.0f, 1.0f);
					matRot.SetColumn(3, vec);
					AVector::Orthonormalize(mDirX, mDirY, mDirZ);

					mCtrlsGroup->WorldTransform.SetTranslate(movTrans);
					mCtrlsGroup->WorldTransform.SetRotate(matRot);
					mCtrlsGroup->WorldTransform.SetUniformScale(1.0f);
				}
				else if (AM_PARENT == mAxisMode)
				{
					Node *parent = DynamicCast<Node>(mov->GetParent());
					if (parent)
					{
						parent->WorldTransform.GetRotate().GetColumn(0, mDirX);
						parent->WorldTransform.GetRotate().GetColumn(1, mDirY);
						parent->WorldTransform.GetRotate().GetColumn(2, mDirZ);
						mDirX.Normalize();
						mDirY.Normalize();
						mDirZ.Normalize();
						HMatrix matRot;
						matRot.SetColumn(0, mDirX);
						matRot.SetColumn(1, mDirY);
						matRot.SetColumn(2, mDirZ);
						AVector vec;
						vec.Set(0.0f, 0.0f, 0.0f, 1.0f);
						matRot.SetColumn(3, vec);
						AVector::Orthonormalize(mDirX, mDirY, mDirZ);

						mCtrlsGroup->WorldTransform.SetTranslate(movTrans);
						mCtrlsGroup->WorldTransform.SetRotate(matRot);
					}
				}

				Movable *movParent = mov->GetParent();
				if (movParent)
				{
					HMatrix parRotMat = movParent->WorldTransform.GetRotate();
					AVector mDirX;
					AVector mDirY;
					AVector mDirZ;
					parRotMat.GetColumn(0, mDirX);
					parRotMat.GetColumn(1, mDirY);
					parRotMat.GetColumn(2, mDirZ);
					mDirX.Normalize();
					mDirY.Normalize();
					mDirZ.Normalize();
					mParentRotateMat.SetColumn(0, mDirX);
					mParentRotateMat.SetColumn(1, mDirY);
					mParentRotateMat.SetColumn(2, mDirZ);

					mParentWorldTrans = movParent->WorldTransform;
				}
			}
			else
			{
				mCtrlsGroup->SetActiveChild(6);
			}
		}
		else
		{
			APoint pos;

			for (int i = 0; i < numObjects; i++)
			{
				Object *obj = PX2_SELECTM_E->GetObjectAt(i);
				Movable *mov = DynamicCast<Movable>(obj);

				if (mov)
				{
					pos += mov->WorldTransform.GetTranslate();
				}
			}

			pos /= (float)numObjects;

			mCtrlsGroup->WorldTransform.SetRotate(0.0f, 0.0f, 0.0f);
			mDirX = AVector::UNIT_X;
			mDirY = AVector::UNIT_Y;
			mDirZ = AVector::UNIT_Z;

			mCtrlsGroup->WorldTransform.SetTranslate(pos);
		}
	}

	mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
}
//----------------------------------------------------------------------------
SceneNodeCtrl::DragType SceneNodeCtrl::GetDragType(Camera *camera,
	const APoint &viewPortPos, const Sizef &size)
{
	APoint origin;
	AVector direction;
	camera->GetPickRay(viewPortPos.X(), viewPortPos.Z(), size, origin, direction);

	Picker picker;

	Movable *ctrlX = GetCurrentCtrlX();
	Movable *ctrlY = GetCurrentCtrlY();
	Movable *ctrlZ = GetCurrentCtrlZ();
	Movable *ctrlXYZ = GetCurrentCtrlXYZ();
	Movable *ctrlXY = GetCurrentCtrlXY();
	Movable *ctrlYZ = GetCurrentCtrlYZ();
	Movable *ctrlXZ = GetCurrentCtrlXZ();

	bool xDrag = false;
	float xDragT = Mathf::MAX_REAL;

	bool yDrag = false;
	float yDragT = Mathf::MAX_REAL;

	bool zDrag = false;
	float zDragT = Mathf::MAX_REAL;

	bool xyzDrag = false;
	float xyzDragT = Mathf::MAX_REAL;

	bool xyDrag = false;
	float xyDragT = Mathf::MAX_REAL;

	bool yzDrag = false;
	float yzDragT = Mathf::MAX_REAL;

	bool xzDrag = false;
	float xzDragT = Mathf::MAX_REAL;

	GetCtrlsGroup()->Update(Time::GetTimeInSeconds(), false);

	picker.Execute(ctrlX, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		xDrag = true;
		xDragT = picker.GetClosestNonnegative().T;
	}

	picker.Execute(ctrlY, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		yDrag = true;
		yDragT = picker.GetClosestNonnegative().T;
	}

	picker.Execute(ctrlZ, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		zDrag = true;
		zDragT = picker.GetClosestNonnegative().T;
	}

	picker.Execute(ctrlXYZ, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		xyzDrag = true;
		xyzDragT = picker.GetClosestNonnegative().T;
	}

	picker.Execute(ctrlXY, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		xyDrag = true;
		xyDragT = picker.GetClosestNonnegative().T;
	}

	picker.Execute(ctrlYZ, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		yzDrag = true;
		yzDragT = picker.GetClosestNonnegative().T;
	}

	picker.Execute(ctrlXZ, origin, direction, -Mathf::MAX_REAL, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		xzDrag = true;
		xzDragT = picker.GetClosestNonnegative().T;
	}

	if (xDrag)
		return DT_X;
	else if (yDrag)
		return DT_Y;
	else if (zDrag)
		return DT_Z;
	else if (xyzDrag)
		return DT_XYZ;
	else if (xyDrag)
		return DT_XY;
	else if (yzDrag)
		return DT_YZ;
	else if (xzDrag)
		return DT_XZ;

	return DT_NONE;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
BoundCtrl::BoundCtrl()
{
	GeoObjFactory factory;
	mCtrlsGroup = new0 SwitchNode();
	mCtrlsGroup->WorldTransformIsCurrent = true;
	mBoundNode = factory.CreateBound();
	mCtrlsGroup->AttachChild(mBoundNode);
	mCtrlsGroup->AttachChild(new0 Node());
}
//----------------------------------------------------------------------------
BoundCtrl::~BoundCtrl()
{
}
//----------------------------------------------------------------------------
void BoundCtrl::OnEvent(Event *event)
{
	if (GraphicsES::IsEqual(event, EditES::AddSelect))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, EditES::RemoveSelect))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, EditES::RemoveAllSelects))
	{
		UpdateCtrl();
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::ObjectTransformChanged))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, EditES::UnDo) ||
		GraphicsES::IsEqual(event, EditES::ReDo))
	{
		UpdateCtrl();
	}
	//else if (GraphicsES::IsEqual(event, GraphicsES::BoundChanged))
	//{
	//	UpdateCtrl();
	//}
}
//----------------------------------------------------------------------------
void BoundCtrl::UpdateCtrl()
{
	mCtrlsGroup->SetActiveChild(1);

	int numMovs = 0;
	APoint center;
	APoint boundCenter;
	float radius = 0.0f;
	PX2_EDIT.GetSelectionCenterRadius(numMovs, center, boundCenter, radius);

	if (numMovs > 0)
	{
		mCtrlsGroup->SetActiveChild(0);

		if (0.0f == radius)
		{
			radius = 1.0f;

			mCtrlsGroup->WorldTransform.SetUniformScale(radius);
			mCtrlsGroup->WorldTransform.SetTranslate(center);
		}
		else
		{
			mCtrlsGroup->WorldTransform.SetUniformScale(radius);
			mCtrlsGroup->WorldTransform.SetTranslate(boundCenter);
		}
	}
	else
	{
		mCtrlsGroup->SetActiveChild(1);
	}

	mCtrlsGroup->Update(Time::GetTimeInSeconds(), false);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CameraCtrl
//----------------------------------------------------------------------------
CameraCtrl::CameraCtrl()
{
	mCtrlNode = new0 Node();
	mCtrlNode->WorldTransformIsCurrent = true;

	GeoObjFactory factory;
	mPoly = factory.CreateCameraCtrl();
	mCtrlNode->AttachChild(mPoly);
}
//----------------------------------------------------------------------------
CameraCtrl::~CameraCtrl()
{
	PX2_EW.GoOut(this);
}
//----------------------------------------------------------------------------
void CameraCtrl::OnEvent(Event *event)
{
	if (GraphicsES::IsEqual(event, EditES::AddSelect))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, EditES::RemoveSelect))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, EditES::RemoveAllSelects))
	{
		UpdateCtrl();
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::ObjectTransformChanged))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, EditES::UnDo) ||
		GraphicsES::IsEqual(event, EditES::ReDo))
	{
		UpdateCtrl();
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::CameraNodeChanged))
	{
		UpdateCtrl();
	}
}
//----------------------------------------------------------------------------
Node *CameraCtrl::GetNode()
{
	return mCtrlNode;
}
//----------------------------------------------------------------------------
void CameraCtrl::UpdateCtrl()
{
	int numObjscts = PX2_SELECTM_E->GetNumObjects();
	if (1 == numObjscts)
	{
		Object *firstSelObj = PX2_SELECTM_E->GetFirstObject();
		CameraNode *camNode = DynamicCast<CameraNode>(firstSelObj);
		if (camNode)
		{
			mCtrlNode->WorldTransform = camNode->WorldTransform;
			mCtrlNode->WorldTransform.MakeUnitScale();

			GeoObjFactory factory;
			factory.UpdateCameraCtrl(camNode, mPoly);

			mCtrlNode->Show(true);
		}
		else
		{
			mCtrlNode->Show(false);
		}
	}
	else
	{
		mCtrlNode->Show(false);
	}
}
//----------------------------------------------------------------------------
