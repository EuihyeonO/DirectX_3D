#pragma once
#include <GameEngineCore/GameEngineActor.h>

class MyTest : public GameEngineActor
{

public:

	MyTest();
	~MyTest();

	MyTest(const MyTest& _Other) = delete;
	MyTest(MyTest&& _Other) noexcept = delete;
	MyTest& operator=(const MyTest& _Other) = delete;
	MyTest& operator=(MyTest&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	
	void TestRender();

	void CreateKey();

	void Render2Dto3D();
	std::shared_ptr<GameEngineRenderTarget> NewTarget = nullptr;
	GameEngineRenderUnit Unit;
	std::shared_ptr<class ContentUIRenderer> Test1;

	std::shared_ptr<class ContentFBXRenderer> TestRd = nullptr;

};

