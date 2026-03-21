#include "pch.h"
#include "InputHandler.h"
#include <SDL3/SDL.h>
#include "Bloodforge.h"
#include <stdexcept>

void Bloodforge::InputHandler::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			Bloodforge::GetInstance().RequestStop();
		}

		if (e.type == SDL_EVENT_KEY_UP || e.type == SDL_EVENT_KEY_DOWN)
		{
			for (auto& [key, value] : m_InputActionMaps[m_CurrentMapId])
			{
				if (value->Button != e.key.key) continue;
				InputActionInfo info;
				info.started = (e.type == SDL_EVENT_KEY_DOWN) && (!value->WasDownLastFrame);
				value->WasDownLastFrame = e.type == SDL_EVENT_KEY_DOWN;
				info.ongoing = e.type == SDL_EVENT_KEY_DOWN;
				info.finished = e.type == SDL_EVENT_KEY_UP;
				for (const auto& listener : value->Listeners)
				{
					listener(info);
				}
			}
		}
	}
}

void Bloodforge::InputHandler::CreateMap(MapId mapId)
{
	if (m_InputActionMaps.contains(mapId)) throw std::runtime_error("Trying to create an input action map with an id that already exists.");
	m_InputActionMaps.emplace(mapId, std::unordered_map<ActionId, std::unique_ptr<InputAction>>{});
}

void Bloodforge::InputHandler::CreateAction(ActionId actionId, MapId mapId, ButtonId buttonId)
{
	if (!m_InputActionMaps.contains(mapId)) throw std::runtime_error("Trying to create an action in an input action map that doesn't exist.");
	if (m_InputActionMaps[mapId].contains(actionId)) throw std::runtime_error("Trying to create an action in an input action map that already has an action with this id.");
	std::unique_ptr<InputAction> inputAction = std::make_unique<InputAction>();
	inputAction->Button = buttonId;
	m_InputActionMaps[mapId][actionId] = std::move(inputAction);
}

void Bloodforge::InputHandler::SetCurrentMap(MapId mapId)
{
	if (!m_InputActionMaps.contains(mapId)) throw std::runtime_error("Trying to set an input action map that doesn't exist.");
	m_CurrentMapId = mapId;
}

void Bloodforge::InputHandler::AddListenerToInputAction(ActionId actionId, MapId mapId, const ListenerSignature& listener)
{
	if (!m_InputActionMaps.contains(mapId)) throw std::runtime_error("Trying to access an input action map that doesn't exist.");
	if (!m_InputActionMaps[mapId].contains(actionId)) throw std::runtime_error("Trying to access an action that doesn't exist in this input action map.");

	m_InputActionMaps[mapId][actionId]->Listeners.emplace_back(listener);
}