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
#include <SdbmHash.h>
#include <RectColliderComponent.h>
#include <BloodRenderer.h>
#include <SpriteAnimationComponent.h>
#include <SpriteAnimationSystem.h>

void InitializeRectColliderComponent(Bloodforge::Entity& entity, const Bloodforge::Vector2& size, const Bloodforge::Vector2& offset = { 0.0f, 0.0f })
{
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::RectColliderComponent* collider = entityManager.GetComponent<Bloodforge::RectColliderComponent>(entity);
	collider->SetSize(size);
	collider->SetOffset(offset);
}

void AnimationEventTest()
{
	std::cout << "Anim event triggered" << std::endl;
}

void LoadFunction()
{
	auto& scene = Bloodforge::SceneManager::GetInstance().GetActiveScene();
	auto& entityManager = Bloodforge::EntityManager::GetInstance();
	auto& renderer = Bloodforge::BloodRenderer::GetInstance();
	renderer.SetBackgroundColor({ 127, 127, 127, 255 });

	Bloodforge::Entity& entity = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, Bloodforge::RectColliderComponent, Bloodforge::SpriteAnimationComponent>();
	int entityId = entity.Id;
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity)->SetTexture(Bloodforge::ResourceManager::GetInstance().LoadTexture("BatSheet.png"));
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity)->FlipVertical = false;
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity)->FlipHorizontal = true;
	Bloodforge::SpriteAnimationComponent* animComp = entityManager.GetComponent<Bloodforge::SpriteAnimationComponent>(entity);
	Bloodforge::AnimationUtils::InitializeAnimation(*animComp, Bloodforge::ResourceManager::GetInstance().LoadTexture("Test.png"), 5);
	animComp->FrameTime = 1.0f;
	animComp->StartingFrameIndexAfterLoop = 0;
	animComp->StartingFrame = 0;
	Bloodforge::AnimationUtils::AddAnimationEvent(*animComp, AnimationEventTest, 4);
	Bloodforge::TransformComponent* transformComp1 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity);
	transformComp1->SetLocalPosition(300.0f, 300.0f);
	InitializeRectColliderComponent(entity, { 40.0f, 40.0f });

	Bloodforge::Entity& entity2 = entityManager.CreateEntity<Bloodforge::TransformComponent, Bloodforge::SpriteComponent, Bloodforge::RectColliderComponent>();
	int entity2Id = entity2.Id;
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity2)->SetTexture(Bloodforge::ResourceManager::GetInstance().LoadTexture("Heart.png"));
	entityManager.GetComponent<Bloodforge::SpriteComponent>(entity2)->FlipVertical = true;
	Bloodforge::TransformComponent* transformComp2 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity2);
	transformComp2->SetParent(entityId);
	transformComp2->SetLocalPosition(100.0f, 100.0f);
	transformComp2->SetLocalScale({1.0f, 1.0f});
	InitializeRectColliderComponent(entity2, { 80.0f, 80.0f });

	Bloodforge::Entity& entity3 = entityManager.CreateEntity<Bloodforge::TransformComponent, RotationComponent, Bloodforge::TextComponent>();
	entityManager.GetComponent<Bloodforge::TextComponent>(entity3)->SetFont(Bloodforge::ResourceManager::GetInstance().LoadFont("Font2.otf", 60));
	entityManager.GetComponent<Bloodforge::TextComponent>(entity3)->SetText("Testing");
	entityManager.GetComponent<Bloodforge::TextComponent>(entity3)->SetColor({ 0,0,0,255 });
	entityManager.GetComponent<Bloodforge::TextComponent>(entity3)->FlipHorizontal = true;
	entityManager.GetComponent<Bloodforge::TextComponent>(entity3)->FlipVertical = true;
	Bloodforge::TransformComponent* transformComp3 = entityManager.GetComponent<Bloodforge::TransformComponent>(entity3);
	transformComp3->SetParent(entity2Id);
	transformComp3->SetLocalPosition(100.0f, 100.0f);
	transformComp3->SetLocalScale({ 1.0f, 1.0f });
	RotationComponent* rotComp3 = entityManager.GetComponent<RotationComponent>(entity3);
	rotComp3->SpeedDegPerSec = 180.f;

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
	Bloodforge::WindowUtils::SetCustomCursor(Bloodforge::ResourceManager::GetInstance().LoadCustomCursor(make_sdbm_hash("TestCursor"), "Cursor.png", 0, 0));

	auto& sceneManager = Bloodforge::SceneManager::GetInstance();
	sceneManager.RegisterScene("TestScene", LoadFunction);
	sceneManager.RequestSetCurrentScene("TestScene");
	engine.Run();
	return 0;
}