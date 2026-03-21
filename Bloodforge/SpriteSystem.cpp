#include "pch.h"
#include "SpriteSystem.h"
#include "EntityManager.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "BloodRenderer.h"
#include "BloodTime.h"
#include "InputHandler.h"
#include "SdbmHash.h"
#include <SDL3/SDL.h>

namespace Bloodforge
{
	void SpriteSystem::OnStart()
	{
		auto& inputHandler = InputHandler::GetInstance();
		inputHandler.CreateMap(make_sdbm_hash("TestMap"));
		inputHandler.CreateAction(make_sdbm_hash("TestAction"), make_sdbm_hash("TestMap"), SDLK_SPACE);
		inputHandler.AddListenerToInputAction(make_sdbm_hash("TestAction"), make_sdbm_hash("TestMap"), [this](const InputActionInfo& info)
			{
				InputTest(info);
			});
		inputHandler.SetCurrentMap(make_sdbm_hash("TestMap"));

		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
			{
				std::get<0>(view.ComponentArrays)[i].X = static_cast<float>(i);
				std::get<0>(view.ComponentArrays)[i].Y = static_cast<float>(i);
			}
		}
	}
	void SpriteSystem::OnRender()
	{
		EntityQueryResult result =  EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
			{
				float x = std::get<0>(view.ComponentArrays)[i].X;
				float y = std::get<0>(view.ComponentArrays)[i].Y;
				BloodRenderer::GetInstance().RenderTexture(*std::get<1>(view.ComponentArrays)[i].Texture, x, y);
			}
		}
	}
	void SpriteSystem::InputTest(const InputActionInfo& info)
	{
		if (!info.started) return;
		EntityQueryResult result = EntityManager::GetInstance().QueryEntities<TransformComponent, SpriteComponent>();
		for (Bloodforge::ChunkView view : result.Chunks)
		{
			for (int i = 0; i < std::get<0>(view.ComponentArrays).size(); ++i)
			{
				std::get<0>(view.ComponentArrays)[i].X += 50;
				std::get<0>(view.ComponentArrays)[i].Y += 50;
			}
		}
	}
}