#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <Bloodforge.h>
#include <SceneManager.h>
#include <Scene.h>
#include <ResourceManager.h>
#include "TestSystem.h"
#include "RotationComponent.h"
#include <TextComponent.h>
#include <FileSaveLoadUtils.h>
#include "TestSaveFile.h"
#include <WindowUtils.h>
#include <IdCreator.h>
#include <RectColliderComponent.h>
#include <BloodRenderer.h>
#include <SpriteAnimatorComponent.h>

void InitializeRectColliderComponent(Bloodforge::Entity& entity, const Bloodforge::Vector2& size, const Bloodforge::Vector2& offset = { 0.0f, 0.0f })
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::RectColliderComponent* collider = entityManager.GetComponent<Bloodforge::RectColliderComponent>(entity);
	collider->SetSize(size);
	collider->SetOffset(offset);
}

void AnimationEventTest(Bloodforge::SpriteAnimatorComponent& sprite)
{
	std::cout << "Anim event triggered" << std::endl;
	Bloodforge::AnimationUtils::PlayAnimation(sprite, CreateId("TestAnim2"));
}

void AnimationEventTest2(Bloodforge::SpriteAnimatorComponent& sprite)
{
	std::cout << "Anim event2 triggered" << std::endl;
	Bloodforge::AnimationUtils::PlayAnimation(sprite, CreateId("TestAnim"));

}

void LoadFunction()
{
	auto& scene = Bloodforge::SceneManager::GetInstance().GetActiveScene();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	auto& renderer = Bloodforge::BloodRenderer::GetInstance();
	renderer.SetBackgroundColor({ 127, 127, 127, 255 });

	//////////
	Bloodforge::Entity& entity = entityManager.CreateEntity();
	int entityId = entity.Id;
	Bloodforge::SpriteComponent* spriteComp = entityManager.AddComponent<Bloodforge::SpriteComponent>(entityId);
	spriteComp->SetTexture(Bloodforge::ResourceManager::GetInstance().LoadTexture("BatSheet.png"));
	spriteComp->FlipVertical = false;
	spriteComp->FlipHorizontal = true;
	Bloodforge::SpriteAnimatorComponent* animComp = entityManager.AddComponent<Bloodforge::SpriteAnimatorComponent>(entity);

	Bloodforge::AnimationData data;
	data.FrameTime = 1.0f;
	data.ShouldLoop = true;
	data.NumberOfFrames = 5;
	data.Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Test.png");
	Bloodforge::AnimationUtils::AddAnimation(*animComp, CreateId("TestAnim"), data);
	Bloodforge::AnimationUtils::AddAnimationEvent(*animComp, CreateId("TestAnim"), AnimationEventTest, 4);

	Bloodforge::AnimationData data2;
	data2.FrameTime = 1.0f;
	data2.ShouldLoop = true;
	data2.NumberOfFrames = 4;
	data2.Texture = Bloodforge::ResourceManager::GetInstance().LoadTexture("Test2.png");
	Bloodforge::AnimationUtils::AddAnimation(*animComp, CreateId("TestAnim2"), data2);
	Bloodforge::AnimationUtils::AddAnimationEvent(*animComp, CreateId("TestAnim2"), AnimationEventTest2, 1);

	Bloodforge::AnimationUtils::PlayAnimation(*animComp, CreateId("TestAnim"));

	Bloodforge::TransformComponent* transformComp1 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity);
	transformComp1->SetLocalPosition(300.0f, 300.0f);

	entityManager.AddComponent<Bloodforge::RectColliderComponent>(entity);
	InitializeRectColliderComponent(entity, { 40.0f, 40.0f });
	//////////

	//////////
	Bloodforge::Entity& entity2 = entityManager.CreateEntity();
	int entity2Id = entity2.Id;
	Bloodforge::SpriteComponent* spriteComp2 = entityManager.AddComponent<Bloodforge::SpriteComponent>(entity2);
	spriteComp2->SetTexture(Bloodforge::ResourceManager::GetInstance().LoadTexture("Heart.png"));
	spriteComp2->FlipVertical = true;
	Bloodforge::TransformComponent* transformComp2 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity2);
	transformComp2->SetParent(entityId);
	transformComp2->SetLocalPosition(100.0f, 100.0f);
	transformComp2->SetLocalScale({1.0f, 1.0f});
	entityManager.AddComponent<Bloodforge::RectColliderComponent>(entity2);
	InitializeRectColliderComponent(entity2, { 80.0f, 80.0f });
	//////////

	//////////
	Bloodforge::Entity& entity3 = entityManager.CreateEntity();
	Bloodforge::TextComponent* textComp = entityManager.AddComponent<Bloodforge::TextComponent>(entity3);
	textComp->SetFont(Bloodforge::ResourceManager::GetInstance().LoadFont("Font2.otf", 60));
	textComp->SetText("Testing");
	textComp->SetColor({ 0,0,0,255 });
	textComp->FlipHorizontal = true;
	textComp->FlipVertical = true;
	Bloodforge::TransformComponent* transformComp3 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity3);
	transformComp3->SetParent(entity2Id);
	transformComp3->SetLocalPosition(100.0f, 100.0f);
	transformComp3->SetLocalScale({ 1.0f, 1.0f });
	RotationComponent* rotComp3 = entityManager.AddComponent<RotationComponent>(entity3);
	rotComp3->SpeedDegPerSec = 180.f;
	//////////

	scene.RegisterSystem<TestSystem>();
}

int main(int, char* []) 
{
	Bloodforge::Bloodforge& engine = Bloodforge::Bloodforge::GetInstance();
	engine.SetResourcesDirectory("Resources");

	TestSaveFile testSaveFile;
	testSaveFile.Coins = 200;
	testSaveFile.Health = 50.0f;
	Bloodforge::FileSaveLoadUtils::SaveFile<TestSaveFile>(testSaveFile, "TestSaveFile.json");
	testSaveFile = Bloodforge::FileSaveLoadUtils::LoadFile<TestSaveFile>("TestSaveFile.json");

	Bloodforge::WindowUtils::SetWindowAlwaysOnTop(false);
	Bloodforge::WindowUtils::SetWindowBordered(true);
	Bloodforge::WindowUtils::SetWindowFullScreen(false);
	Bloodforge::WindowUtils::SetWindowIcon("Heart.png");
	Bloodforge::WindowUtils::SetCustomCursor(Bloodforge::ResourceManager::GetInstance().LoadCustomCursor(CreateId("TestCursor"), "Cursor.png", 0, 0));

	auto& sceneManager = Bloodforge::SceneManager::GetInstance();
	sceneManager.RegisterScene("TestScene", LoadFunction);
	sceneManager.RequestSetCurrentScene("TestScene");
	engine.Run();
	return 0;
}