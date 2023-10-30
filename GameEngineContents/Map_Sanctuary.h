#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class Map_Sanctuary : public GameEngineActor
{
public:
	// constrcuter destructer
	Map_Sanctuary();
	~Map_Sanctuary();

	// delete Function
	Map_Sanctuary(const Map_Sanctuary& _Other) = delete;
	Map_Sanctuary(Map_Sanctuary&& _Other) noexcept = delete;
	Map_Sanctuary& operator=(const Map_Sanctuary& _Other) = delete;
	Map_Sanctuary& operator=(Map_Sanctuary&& _Other) noexcept = delete;

	inline std::shared_ptr<class ContentFBXRenderer> GetNaviMeshRenderer() const
	{
		return m_pNaviRenderer;
	}

	inline std::shared_ptr<class FrogFloor> GetFloor() const
	{
		return m_pFrogFloor;
	}

	const float4 GetTilePos(const int _Y, const int _X);
	const float4 GetTileIndex(const float4& _Pos);

	void DestroyTile(const int _Y, const int _X);
	bool IsTile(const int _Y, const int _X);
	void ResetTile();
	void ShakeTile(const int _Y, const int _X);

	void OnRotationFloor();
	void OffRotationFloor();
	
	void NaviRenderSwitch();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();
	void InitKey();
	void KeyUpdate();
	void Create_PhysXComponent();

	std::shared_ptr<class ContentFBXRenderer> m_pNaviRenderer = nullptr;
	std::shared_ptr<class ContentFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<class PhysXTriangleComponent> m_pTriangleComp = nullptr;

	std::shared_ptr<class FrogFloor> m_pFrogFloor = nullptr;

	const float4 m_MapRot = float4{ 0 , -135 , 0 };
	const float4 m_NavRot = float4{ 0, 45, 0 };
	const float4 m_MapPos = float4{ 0, 0 , 0 };

	
};
