#include "PrecompileHeader.h"
#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineMaterial.h"

void GameEngineFBXAnimationInfo::Init(std::shared_ptr<GameEngineFBXMesh> _Mesh, std::shared_ptr<GameEngineFBXAnimation> _Animation, const std::string_view& _Name, int _Index)
{
	// GameENgineFBXAnimation의 행렬 정보가 완전해지는것은 
	// CalFbxExBoneFrameTransMatrix가 호출되고 난 후입니다.
	// 애니메이션의 행렬이 계산되는겁니다.

	_Animation->AnimationMatrixLoad(_Mesh, _Name, _Index);
	Aniamtion = _Animation;
	FBXAnimationData = Aniamtion->GetAnimationData(_Index);
	Start = static_cast<UINT>(FBXAnimationData->TimeStartCount);
	End = static_cast<UINT>(FBXAnimationData->TimeEndCount);
	Mesh = _Mesh;
	Aniamtion = _Animation;


	Start = 0;
	End = static_cast<unsigned int>(FBXAnimationData->TimeEndCount);
}


// 이걸 통해서 애니메이션을 진행시키고.
void GameEngineFBXAnimationInfo::Update(float _DeltaTime)
{
	if (true == ParentRenderer->Pause)
	{
		return;
	}

	CurFrameTime += _DeltaTime;
	PlayTime += _DeltaTime;
	//                      0.1
	// 1

	float CurInter = Inter;
	if (FrameTime.size() > CurFrame)
	{
		CurInter = FrameTime[CurFrame];
	}
	while (CurFrameTime >= CurInter)
	{
		// 여분의 시간이 남게되죠?
		// 여분의 시간이 중요합니다.
		CurFrameTime -= CurInter;
		++CurFrame;
		if (StartFunc.end() != StartFunc.find(CurFrame))
		{
			if (StartFunc[CurFrame].bStart == false && StartFunc[CurFrame].pFunc != nullptr)
			{
				StartFunc[CurFrame].pFunc();
				StartFunc[CurFrame].bStart = true;
			}
		}
		if (false == bOnceStart && CurFrame == 0)
		{
			bOnceStart = true;
			bOnceEnd = false;
		}

		if (CurFrame == (Frames.size() - 1)
			&& false == bOnceEnd)
		{
			bOnceEnd = true;
			bOnceStart = false;
			break;
		}

		if (CurFrame >= End)
		{
			if (true == Loop)
			{
				CurFrame = Start;	

				for (std::pair<const UINT, AnimStartFunc>& PairStartFunc : StartFunc)
				{
					PairStartFunc.second.bStart = false;
				}
			}
			else
			{
				CurFrame = End - 1;
				EndValue = true;
			}

		}
	}
	

	unsigned int NextFrame = CurFrame + 1;
	if (NextFrame >= End)
	{
		NextFrame = Start;
	}

	// mesh      subset
	std::vector<std::vector< std::shared_ptr<GameEngineRenderUnit>>>& Units = ParentRenderer->GetAllRenderUnit();
	std::vector<float4x4>& AnimationBoneMatrix = ParentRenderer->AnimationBoneMatrixs;

	std::vector<AnimationBoneData>& AnimationBoneData = ParentRenderer->AnimationBoneDatas;

	for (int i = 0; i < AnimationBoneMatrix.size(); i++)
	{
		if (0 == FBXAnimationData->AniFrameData.size())
		{
			continue;
		}

		Bone* BoneData = ParentRenderer->GetFBXMesh()->FindBone(i);

		if (true == FBXAnimationData->AniFrameData[i].BoneMatData.empty())
		{
			AnimationBoneMatrix[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
			return;
		}

		// 애니메이션 블랜드 등은 하나도 안들어가 있다.

		// CurFrame + 1

		FbxExBoneFrameData& CurData = FBXAnimationData->AniFrameData[i].BoneMatData[CurFrame];
		FbxExBoneFrameData& NextData = FBXAnimationData->AniFrameData[i].BoneMatData[NextFrame];

		AnimationBoneData[i].Scale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
		AnimationBoneData[i].RotQuaternion = float4::SLerpQuaternion(CurData.Q, NextData.Q, CurFrameTime);
		AnimationBoneData[i].Pos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

		size_t Size = sizeof(float4x4);

		float4x4 Mat = float4x4::Affine(AnimationBoneData[i].Scale, AnimationBoneData[i].RotQuaternion, AnimationBoneData[i].Pos);

		// 오프셋을 곱하지 않은 본행렬의 부모가 진짜 우리가 생각하는 월드이고
		ParentRenderer->AnimationBoneMatrixsNotOffset[i] = Mat;

		// 오프셋을 곱해주는 녀석은 버텍스에 곱해지기 위한 녀석이 된다.

		AnimationBoneMatrix[i] = BoneData->BonePos.Offset * Mat;
	}
}

void GameEngineFBXAnimationInfo::Reset()
{
	CurFrameTime = 0.0f;
	CurFrame = 0;
	PlayTime = 0.0f;
	EndValue = false;
	for (std::pair<const UINT, AnimStartFunc>& PairStartFunc : StartFunc)
	{
		PairStartFunc.second.bStart = false;
	}
}

bool GameEngineFBXAnimationInfo::IsEnd()
{
	return EndValue;
}


GameEngineFBXRenderer::GameEngineFBXRenderer()
{
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
}


void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("로드하지 않은 FBX 매쉬를 사용하려고 했습니다.");
	}

	FindFBXMesh->Initialize();

	const std::vector<Bone>& Bones = FindFBXMesh->GetAllBone();

	if (Bones.size() != AnimationBoneDatas.size())
	{
		AnimationBoneDatas.resize(Bones.size());

		for (size_t i = 0; i < Bones.size(); i++)
		{
			float4x4 Offset = Bones[i].BonePos.Offset;

			Offset.Decompose(AnimationBoneDatas[i].Scale, AnimationBoneDatas[i].RotQuaternion, AnimationBoneDatas[i].Pos);

			AnimationBoneDatas[i].RotEuler = AnimationBoneDatas[i].RotQuaternion.QuaternionToEulerDeg();
		}
	}

	// 너 몇개 가지고 있어.
	for (size_t UnitCount = 0; UnitCount < FindFBXMesh->GetRenderUnitCount(); UnitCount++)
	{
		SetFBXMesh(_Name, _Material, UnitCount);
	}
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	for (size_t SubSetCount = 0; SubSetCount < FindFBXMesh->GetSubSetCount(MeshIndex); SubSetCount++)
	{
		size_t SubSet = FindFBXMesh->GetSubSetCount(MeshIndex);

		SetFBXMesh(_Name, _Material, MeshIndex, SubSetCount);
	}
}

// SetFbxMesh를 해서 만들어진 랜더 유니트를 사용하게 하기 위해서 리턴해준다.
std::shared_ptr<GameEngineRenderUnit> GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name,
	std::string _Material,
	size_t _MeshIndex,
	size_t _SubSetIndex /*= 0*/)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("존재하지 않는 FBXMesh를 세팅했습니다.");
		return nullptr;
	}


	if (nullptr == FBXMesh && nullptr != FindFBXMesh)
	{
		FBXMesh = FindFBXMesh;
	}
	else if (nullptr != FBXMesh && FBXMesh != FindFBXMesh)
	{
		// 지금까지 만든거 다 날립니다.
	}

	FindFBXMesh->Initialize();

	// return nullptr;

	if (Unit.empty())
	{
		Unit.resize(FBXMesh->GetRenderUnitCount());
		for (size_t i = 0; i < Unit.size(); i++)
		{
			size_t Count = FBXMesh->GetSubSetCount(i);

			Unit[i].resize(Count);
			for (size_t j = 0; j < Count; j++)
			{
				Unit[i][j] = CreateRenderUnit();
			}
		}
	}

	std::shared_ptr<GameEngineRenderUnit> RenderUnit = Unit[_MeshIndex][_SubSetIndex];
	std::shared_ptr <GameEngineMesh> GetFBXMesh = FBXMesh->GetGameEngineMesh(_MeshIndex, _SubSetIndex);

	RenderUnit->SetMesh(GetFBXMesh);
	RenderUnit->SetMaterial(_Material);


	if (0 == AnimationBoneMatrixs.size())
	{
		size_t Count = FBXMesh->GetBoneCount();

		if (nullptr == FBXMesh)
		{
			MsgAssert("본이 존재하지 않는 매쉬에 애니메이션을 넣으려고 했습니다.");
		}

		AnimationBoneMatrixs.resize(Count);
		AnimationBoneMatrixsNotOffset.resize(Count);
		AnimationBoneDatas.resize(Count);
	}


	if (RenderUnit->ShaderResHelper.IsStructuredBuffer("ArrAniMationMatrix"))
	{
		GameEngineStructuredBufferSetter* AnimationBuffer = RenderUnit->ShaderResHelper.GetStructuredBufferSetter("ArrAniMationMatrix");

		AnimationBuffer->Res = FBXMesh->GetAnimationStructuredBuffer();

		if (nullptr == AnimationBuffer->Res)
		{
			MsgAssert("애니메이션용 스트럭처드 버퍼가 존재하지 않습니다.");
			return RenderUnit;
		}

		if (0 == AnimationBoneMatrixs.size())
		{
			return RenderUnit;
		}

		// 링크를 걸어준것.
		AnimationBuffer->SetData = &AnimationBoneMatrixs[0];
		AnimationBuffer->Size = sizeof(float4x4);
		AnimationBuffer->Count = AnimationBoneMatrixs.size();
	}


	if (RenderUnit->ShaderResHelper.IsTexture("DiffuseTexture"))
	{
		
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(_MeshIndex, _SubSetIndex);

		// 텍스쳐이름 확인용 test 코드 
		// 683 0 
		// 683 1 
		if (MatData.DifTextureName == "Floormark_Check.png")
		{
			int a = 0;
		}
		

		if (nullptr != GameEngineTexture::Find(MatData.DifTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("DiffuseTexture", MatData.DifTextureName);
		}
		else
		{
			UnTexturedUnit[{_MeshIndex, _SubSetIndex}] = RenderUnit;
		}
	}


	if (RenderUnit->ShaderResHelper.IsTexture("NormalTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(_MeshIndex, _SubSetIndex);

		if (nullptr != GameEngineTexture::Find(MatData.NorTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("NormalTexture", MatData.NorTextureName);
		}

		BaseValue.IsNormal = 1;
	}


	return RenderUnit;
}

void GameEngineFBXRenderer::SetAnimationStartFunc(const std::string_view& _Name, UINT _Index, std::function<void()> _Func)
{	
	std::string sUpperName = GameEngineString::ToUpper(_Name);
	if (Animations.end() == Animations.find(sUpperName))
	{
		MsgAssert("해당 이름의 애니메이션이 없습니다." + sUpperName);
		return;
	}
	std::shared_ptr< GameEngineFBXAnimationInfo> Info = Animations[sUpperName];
	//std::weak_ptr<GameEngineFBXAnimationInfo> AnimInfo = Animations[sUpperName];
	if ((Info->Start > _Index) || (_Index >= Info->End))
	{
		MsgAssert("설정하려는 인덱스가 " + sUpperName + "의 최대 프레임인 " + std::to_string(Info->Start)+","+ std::to_string(Info->End) + " 을(를) 넘었습니다");
		return;
	}
	Info->StartFunc[_Index].pFunc = _Func;
	Info->StartFunc[_Index].bStart = false;

}

void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, const AnimationCreateParams& _Params, int _Index)
{
	// 애니메이션 방식은 무조건 1개일것이라고 보고.
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	// 본을 가진 fbx가 세팅되어 있는지 검사한다.
	if (nullptr == GetFBXMesh())
	{
		MsgAssert("골격 FBX가 세팅되어 있지 않습니다.");
		return;
	}

	// 여기있는 함수를 외부에서 호출하면 됩니다.

	if (Animations.end() != Animations.find(UpperName))
	{
		MsgAssert("이미 존재하는 이름의 애니메이션입니다." + UpperName);
		return;
	}

	std::shared_ptr<GameEngineFBXAnimation> Animation = GameEngineFBXAnimation::Find(_AnimationFBXName);

	if (nullptr == Animation)
	{
		MsgAssert("GameEngineFBXAnimation이 존재하지 않습니다. " + std::string(_AnimationFBXName));
		return;
	}

	//GameEngineFile File;
	//File.SetPath(Animation->GetPath());
	//File.ChangeExtension(".AnimationFBX");

	//if (false == File.IsExists())
	//{
	//	Animation->Initialize();
	//}

	std::shared_ptr<GameEngineFBXAnimationInfo> NewAnimation = std::make_shared<GameEngineFBXAnimationInfo>();
	NewAnimation->Init(FBXMesh, Animation, _AnimationName, _Index);
	NewAnimation->ParentRenderer = this;
	NewAnimation->Inter = _Params.Inter;
	NewAnimation->Loop = _Params.Loop;
	NewAnimation->BlendIn = _Params.BlendIn;
	NewAnimation->BlendOut = _Params.BlendOut;
	NewAnimation->Reset();

	if (-1 != _Params.Start)
	{
		NewAnimation->Start = static_cast<unsigned int>(_Params.Start);
	}
	if (-1 != _Params.End)
	{
		NewAnimation->End = static_cast<unsigned int>(_Params.End);
	}

	BaseValue.IsAnimation = 1;
	Animations.insert(std::make_pair(UpperName, NewAnimation));

	Animation;
}



void GameEngineFBXRenderer::PauseSwtich()
{
	Pause = !Pause;
}

void GameEngineFBXRenderer::ChangeAnimation(const std::string& _AnimationName, bool _Force, float _BlendTime)
{
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>>::iterator FindIter = Animations.find(UpperName);

	if (Animations.end() == FindIter)
	{
		MsgAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (false == _Force && CurAnimation == FindIter->second)
	{
		return;
	}
	//change

	// prev
	CurBlendTime = 0.0f;
	BlendTime = 0.0f;
	if (nullptr != CurAnimation)
	{
		PrevAnimationBoneDatas = AnimationBoneDatas;
		if (_BlendTime < 0.0f)// 설정하지 않았다면 default
		{
			BlendTime = CurAnimation->BlendOut + FindIter->second->BlendIn;
		}
		else
		{
			BlendTime = _BlendTime;
		}
	}
	FindIter->second->Reset();
	CurAnimation = FindIter->second;
}

void GameEngineFBXRenderer::CalculateUnitPos()
{
	float4 f4MinPos = float4::ZERO;
	float4 f4MaxPos = float4::ZERO;
	float4 f4Scale = float4::ZERO;
	float4 ResultPos = float4::ZERO;
	float4 Quat;
	Quat.EulerDegToQuaternion();

	float4x4 RenderUnitMat = float4x4::Zero;

	for (size_t i = 0; i < Unit.size(); i++)
	{
		f4MinPos = FBXMesh->GetRenderUnit(i)->MinBoundBox;
		f4MaxPos = FBXMesh->GetRenderUnit(i)->MaxBoundBox;
		f4Scale = FBXMesh->GetRenderUnit(i)->BoundScaleBox;
		ResultPos = (f4MinPos + f4MaxPos) * 0.5f;

		RenderUnitMat.Compose(f4Scale, Quat, ResultPos);
		RenderUnitMat *= GetTransform()->GetWorldMatrixRef();
		//RenderUnitMat *= GetTransform()->GetLocalWorldMatrixRef();

		float4 Pos = float4(RenderUnitMat._30, RenderUnitMat._31, RenderUnitMat._32, RenderUnitMat._33);

		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			Unit[i][j]->SetUnitPos(Pos);
			Unit[i][j]->SetUnitScale(f4Scale);
		}
	}
}
void GameEngineFBXRenderer::UpdateBlend(float _DeltaTime)
{

	for (int i = 0; i < AnimationBoneMatrixs.size(); i++)
	{

		Bone* BoneData = GetFBXMesh()->FindBone(i);

		//if (true == FBXAnimationData->AniFrameData[i].BoneMatData.empty())
		//{
		//	AnimationBoneMatrixs[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
		//	return;
		//}

		// 애니메이션 블랜드 등은 하나도 안들어가 있다.

		//FbxExBoneFrameData& CurData = FBXAnimationData->AniFrameData[i].BoneMatData[CurFrame];
		float LerpTime = CurBlendTime / BlendTime;
		FbxExBoneFrameData& NextData = CurAnimation->FBXAnimationData->AniFrameData[i].BoneMatData[CurAnimation->CurFrame];

		AnimationBoneDatas[i].Scale = float4::Lerp(PrevAnimationBoneDatas[i].Scale, NextData.S, LerpTime);
		AnimationBoneDatas[i].RotQuaternion = float4::SLerpQuaternion(PrevAnimationBoneDatas[i].RotQuaternion, NextData.Q, LerpTime);
		AnimationBoneDatas[i].Pos = float4::Lerp(PrevAnimationBoneDatas[i].Pos, NextData.T, LerpTime);

		size_t Size = sizeof(float4x4);

		float4x4 Mat = float4x4::Affine(AnimationBoneDatas[i].Scale, AnimationBoneDatas[i].RotQuaternion, AnimationBoneDatas[i].Pos);

		AnimationBoneMatrixs[i] = BoneData->BonePos.Offset * Mat;
	}

}


void GameEngineFBXRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation)
	{
		return;
	}
	if (0.0f != BlendTime && BlendTime > CurBlendTime)
	{
		UpdateBlend(_DeltaTime);
		CurBlendTime += _DeltaTime;
		return;
	}
	CurAnimation->Update(_DeltaTime);

	const TransformData& TransFormData = GetTransform()->GetTransDataRef();

	for (size_t i = 0; i < AttachTransformValue.size(); i++)
	{
		AttachTransformInfo& Data = AttachTransformValue[i];

		// 오프셋이 곱해지지 않은 행렬이 실제 본의 위치를 담고 있는 행렬이다.
		// 버텍스는 오프셋을 기반으로 만들어져있기 때문이다.
		float4x4 Mat = AnimationBoneMatrixsNotOffset[Data.Index];
		Mat *= TransFormData.WorldMatrix;

		float4 Scale;
		float4 Rot;
		float4 Pos;

		Mat.Decompose(Scale, Rot, Pos);

		// 세팅할때 월드로 해줘야 한다. 
		// 정말 너무 다양한 상황이 있을수 있기 때문에 월드로 변경해서 넣어줘야 한다.
		Data.Transform->SetWorldRotation(Rot.QuaternionToEulerDeg() + Data.OffsetRot);
		Data.Transform->SetWorldPosition(Pos + Data.OffsetPos);
	}
}

float4 GameEngineFBXRenderer::GetMeshScale()
{
	float4 f4MinBox = float4::ZERO;
	float4 f4MaxBox = float4::ZERO;
	float4 ResultBox = float4::ZERO;
	for (size_t i = 0; i < FBXMesh->GetRenderUnitCount(); i++)
	{
		float4 f4TempMinBox = float4::ZERO;
		float4 f4TempMaxBox = float4::ZERO;
		f4TempMinBox = FBXMesh->GetRenderUnit(i)->MinBoundBox;
		f4TempMaxBox = FBXMesh->GetRenderUnit(i)->MaxBoundBox;
		if (f4MinBox.x > f4TempMinBox.x)
		{
			f4MinBox.x = f4TempMinBox.x;
		}
		if (f4MinBox.y > f4TempMinBox.y)
		{
			f4MinBox.y = f4TempMinBox.y;
		}
		if (f4MinBox.z > f4TempMinBox.z)
		{
			f4MinBox.z = f4TempMinBox.z;
		}

		if (f4MaxBox.x < f4TempMaxBox.x)
		{
			f4MaxBox.x = f4TempMaxBox.x;
		}
		if (f4MaxBox.y < f4TempMaxBox.y)
		{
			f4MaxBox.y = f4TempMaxBox.y;
		}
		if (f4MaxBox.z < f4TempMaxBox.z)
		{
			f4MaxBox.z = f4TempMaxBox.z;
		}
	}
	ResultBox.x = f4MaxBox.x - f4MinBox.x;
	ResultBox.y = f4MaxBox.y - f4MinBox.y;
	ResultBox.z = f4MaxBox.z - f4MinBox.z;

	return ResultBox;
}


AnimationBoneData GameEngineFBXRenderer::GetBoneData(std::string _Name)
{
	Bone* BoneData = FBXMesh->FindBone(_Name);

	AnimationBoneData Data;

	if (nullptr == BoneData)
	{
		MsgAssert(std::string(_Name) + "존재하지 않는 본의 데이터를 찾으려고 했습니다.");
		return Data;
	}


	Data = GetBoneData(BoneData->Index);

	const TransformData& TransFormData = GetTransform()->GetTransDataRef();
	Data.Pos += TransFormData.LocalPosition;
	Data.RotEuler = Data.RotQuaternion.QuaternionToEulerDeg();

	return Data;
}

void GameEngineFBXRenderer::SetAttachTransform(std::string_view _Name, GameEngineTransform* _Transform, float4 _OffsetPos, float4 _OffsetRot)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	Bone* BoneData = FBXMesh->FindBone(UpperName);

	if (nullptr == BoneData)
	{
		MsgAssert(std::string(_Name) + "존재하지 않는 본의 데이터를 찾으려고 했습니다.");
		return;
	}

	SetAttachTransform(BoneData->Index, _Transform, _OffsetPos, _OffsetRot);
}

void GameEngineFBXRenderer::SetAttachTransform(int Index, GameEngineTransform* _Transform, float4 _OffsetPos, float4 _OffsetRot)
{
	float4x4 Rot;
	float4x4 Pos;

	Rot.RotationDeg(_OffsetRot);
	Pos.Pos(_OffsetPos);

	// OffsetMat.Affine(float4(1.0f, 1.0f, 1.0f), _OffsetRot.EulerDegToQuaternion(), _OffsetPos);


	AttachTransformValue.push_back({ Index, _Transform, _OffsetPos, _OffsetRot, Rot * Pos });
}