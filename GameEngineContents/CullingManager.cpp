#include "PrecompileHeader.h"
#include "CullingManager.h"

#include "FortressLevel.h"
#include "OfficeLevel.h"

#include "Map_Fortress.h"

#include "CullingObject.h"
#include "CullingTrigger.h"

#include "Player.h"

CullingManager::CullingManager()
{
}

CullingManager::~CullingManager()
{
}

void CullingManager::Start()
{
	ContentLevel* CurContentLevel = dynamic_cast<ContentLevel*>(GetLevel());
	if (nullptr == CurContentLevel)
	{
		MsgAssert("������ ���̳��� ĳ��Ʈ�� �����߽��ϴ�. ����");
		return;
	}

	ContentLevelType LevelType = CurContentLevel->GetContentLevelType();
	switch (LevelType)
	{
	case ContentLevelType::NONE:
	{
		MsgAssert("����������Ÿ���� NONE �Դϴ�. Level �� Start ���� Ÿ���� �������ּ���.");
		break;
	}
	case ContentLevelType::OfficeLevel:
	{

		break;
	}
	case ContentLevelType::FortressLevel:
	{
		std::shared_ptr<FortressLevel> CurLevel = CurContentLevel->DynamicThis<FortressLevel>();
		std::shared_ptr<Map_Fortress> CurMap = CurLevel->GetMap();

		// ���� Ʈ���ſ� �ø�������Ʈ�� �������ش�.
		m_vCullingTriggers = CurMap->GetCullingTrigger();
		m_vCullingObjects = CurMap->GetCullingObject();

		break;
	}
	case ContentLevelType::BossFrogLevel:
		break;
	default:
		break;
	}

	// ���⼭ Ʈ���ſ� ������Ʈ�� ��ũ��Ų��. 
	LinkTrigger(LevelType);
}

void CullingManager::Update(float _DeltaTime)
{
	if (0.3f >= GetLiveTime())
	{
		On_FirstTrigger();

		size_t TriggerSize = m_vCullingTriggers.size();
		for (size_t i = 0; i < TriggerSize; ++i)
		{
			// ������Ʈ �������� �̷��� ������� 
			m_vCullingTriggers[i]->isPhysXCollision = 0;
		}

		
		return;
	}

	Culling();
}

void CullingManager::Culling()
{
	float4 PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	
	size_t TriggerSize = m_vCullingTriggers.size();

	// ���⼭ Ʈ���� ��üũ�� ����
	for (int i = 0; i < static_cast<int>(TriggerSize); ++i)
	{
		// �÷��̾�� �浹�߰� 
		if (true == m_vCullingTriggers[i]->CheckCollision(PhysXFilterGroup::PlayerDynamic))
		{
			// Ʈ���Ű� �ߵ����� �ʾҴٸ� 
			if (false == m_vCullingTriggers[i]->IsActivate())
			{
				// ���� Ʈ���Ÿ� Off ó�� ���ְ�
				m_vCullingTriggers[m_iCurTrigger_Idx]->TriggerOff();

				// ���� Ʈ���� �ε����� ����
				m_iCurTrigger_Idx = i;
				if (0 <= m_iCurCullingObj_Idx0)
				{
					Off_Trigger();
				}

				// Ȱ��ȭ �� Ʈ���� �ε����� ���� �� Ʈ���� on 
				Set_ActiveTrigger_Index(i);
				m_vCullingTriggers[i]->TriggerOn();

				// idx ���� �ʱ�ȭ
				CullingObjIdxClear();

				// Ʈ���ſ� ������ ������Ʈ �ѹ� �޾ƿ��� 
				std::vector<int> Numbers = m_vCullingTriggers[i]->Get_CullingObjectNumbers();
				size_t Size = Numbers.size();

				if (0 == Size)
				{
					MsgAssert("���� ������Ʈ �ѹ� ������ ����� 0 �Դϴ�.");
					return;
				}

				// �ε��� �ѹ����� 
				m_iCurCullingObj_Idx0 = Numbers[0];
				m_iCurCullingObj_Idx1 = Numbers[1];
				m_iCurCullingObj_Idx2 = Numbers[2];

				if (Size == 4)
				{
					m_iCurCullingObj_Idx3 = Numbers[3];
				}

				for (size_t i = 0; i < Size; i++)
				{
					int Idx = Numbers[i];
					m_vCullingObjects[Idx]->GetRenderer()->On();
				}
			}
			
		}

		// ������Ʈ �������� �̷��� ������� 
		m_vCullingTriggers[i]->isPhysXCollision = 0;
	}
}

template<typename EnumType>
void CullingManager::LinkTrigger(EnumType _LevelType)
{
	ContentLevelType LevelType = static_cast<ContentLevelType>(_LevelType);
	
	// ������Ʈ�� Ʈ���Ÿ� ��ũ�Ѵ�.
	size_t TriggersSize = m_vCullingTriggers.size();
	size_t ObjectsSize = m_vCullingObjects.size();

	if (TriggersSize <= static_cast<size_t>(0) || ObjectsSize <= static_cast<size_t>(0))
	{
		MsgAssert("������ ����� 0 �Դϴ�. Ʈ���ſ� ������Ʈ�� �ڷᱸ���� ����Ǿ����� Ȯ���ϼ���.");
		return;
	}

	switch (LevelType)
	{
	case ContentLevelType::OfficeLevel:
		break;
	case ContentLevelType::FortressLevel:
	{
		// �ּ� 3��, �ִ� 4������ ��������
		// 0�� ���ڴ� �ݵ�� ���Ե� �ٴڸ޽��ΰɷ� 
		m_vCullingTriggers[0]->Set_CullingObjectNumber(0, 3, 4);		 // �÷��̾� ������ġ 
		m_vCullingTriggers[1]->Set_CullingObjectNumber(0, 3, 4, 5);		 // �ʹݺ� ū �� �Ա�
		m_vCullingTriggers[2]->Set_CullingObjectNumber(0, 4, 5);		 // �� ��� �� �������� ��� ������ �׸� ���� 
		m_vCullingTriggers[3]->Set_CullingObjectNumber(0, 5, 6);	     // ������ ��ٸ��� �������� �ö󰡰� ���� �ٷ� 
		m_vCullingTriggers[4]->Set_CullingObjectNumber(0, 1, 5, 7);	     // ������ ���� �ٷ�
		m_vCullingTriggers[5]->Set_CullingObjectNumber(0, 1, 7, 8);	     // �����������ٸ� ������ ó�������� �׸� ���� 
		m_vCullingTriggers[6]->Set_CullingObjectNumber(0, 1, 7, 8);	     // �ٷ� ���� �ٸ� �ǳʱ� �� 
		m_vCullingTriggers[7]->Set_CullingObjectNumber(0, 1, 8, 9);	     // �ٸ� �ǳʰ� �ٷ� 
		m_vCullingTriggers[8]->Set_CullingObjectNumber(0, 1, 8, 9);      // �ٸ� ���� ������ �ٽ� �ٴ����� �Լ��� 
		m_vCullingTriggers[9]->Set_CullingObjectNumber(1, 9, 10);        // �ٷδ���
		
		// ���⼭���� 
		m_vCullingTriggers[10]->Set_CullingObjectNumber(1, 2, 9, 10);   // �����ٸ��Ѿ���� 
		m_vCullingTriggers[11]->Set_CullingObjectNumber(1, 2, 9 ,10);   // �����ٸ� �߰�
		m_vCullingTriggers[12]->Set_CullingObjectNumber(1, 2, 10, 11);      // ���ٸ��Ѿ���� 
		m_vCullingTriggers[13]->Set_CullingObjectNumber(1, 2, 11, 12);		// ���ٸ��߰� 
		m_vCullingTriggers[14]->Set_CullingObjectNumber(2, 12, 13);		// �������ű� 
		m_vCullingTriggers[15]->Set_CullingObjectNumber(2, 12, 13);		// 4 �簢�� ���� �Ա� 

		m_vCullingTriggers[16]->Set_CullingObjectNumber(2, 13, 14);		// 4 �簢�� ���� �Ա� 

		
		m_vCullingTriggers[17]->Set_CullingObjectNumber(2, 14, 15, 16);	 // 4 �簢�� ������ ���� �ΰ� 
		m_vCullingTriggers[18]->Set_CullingObjectNumber(2, 14, 15, 16);

		m_vCullingTriggers[19]->Set_CullingObjectNumber(2, 15, 16); // �ǳʼ� �ٷ�


		break;
	}
	case ContentLevelType::BossFrogLevel:
		break;
	}

	CheckLink();
}

void CullingManager::On_FirstTrigger()
{
	m_vCullingTriggers[0]->TriggerOn();
	
	m_iCurCullingObj_Idx0 = 0;
	m_iCurCullingObj_Idx1 = 3;
	m_iCurCullingObj_Idx2 = 4;

	m_vCullingObjects[m_iCurCullingObj_Idx0]->GetRenderer()->On();
	m_vCullingObjects[m_iCurCullingObj_Idx1]->GetRenderer()->On();
	m_vCullingObjects[m_iCurCullingObj_Idx2]->GetRenderer()->On();
}

void CullingManager::CheckLink()
{
	std::vector<std::shared_ptr<CullingObject>>::iterator StartIter = m_vCullingObjects.begin();
	std::vector<std::shared_ptr<CullingObject>>::iterator EndIter = m_vCullingObjects.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		// �ϳ��� false ��� ����
		if (false == (*StartIter)->IsLink())
		{
			// �ϴ� �ּ� 
			// MsgAssert("Ʈ���ſ��� ��ũ�� ���������� ������� �ʾҽ��ϴ�.");
			return;
		}
	}
}

inline void CullingManager::Set_ActiveTrigger_Index(int _Index)
{
	size_t TriggersSize = m_vCullingTriggers.size();

	if (_Index >= TriggersSize)
	{
		MsgAssert("�ε��� ���� �߸��Ǿ����ϴ�.");
		return;
	}

	m_iCurTrigger_Idx = _Index;
}

void CullingManager::Off_Trigger()
{
	m_vCullingObjects[m_iCurCullingObj_Idx0]->GetRenderer()->Off();
	m_vCullingObjects[m_iCurCullingObj_Idx1]->GetRenderer()->Off();
	m_vCullingObjects[m_iCurCullingObj_Idx2]->GetRenderer()->Off();

	if (m_iCurCullingObj_Idx3 == -1)
	{
		return;
	}

	m_vCullingObjects[m_iCurCullingObj_Idx3]->GetRenderer()->Off();
}

// �����ؾߵɰ�? 
// �ø��Ŵ������� ������ �ִ� Ʈ���ŵ��� �浹�˻縦 �����Ѵ�. 