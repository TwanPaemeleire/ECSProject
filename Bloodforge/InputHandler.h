#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>
#include "Vector2.h"
#include "KeyDefs.h"

namespace Bloodforge 
{
	using MapId = unsigned int;
	using ActionId = unsigned int;
	using ButtonId = unsigned int;
	struct InputActionInfo;
	using ListenerSignature = std::function<void(const InputActionInfo& info)>;
	struct InputActionInfo
	{
		bool started = false;
		bool finished = false;
		bool ongoing = false;
		Vector2 InputActionVectorData; // Used for example for xy position of mouse
		Vector2 InputActionVectorDeltaData; // Used for example for xy motion of mouse
	};

	struct InputAction
	{
		std::vector<ListenerSignature> Listeners;
		ButtonId Button = 0;
		bool WasDownLastFrame = false;
	};

	class InputHandler final :public Singleton<InputHandler>
	{
	public:
		void ProcessInput();

		void CreateMap(MapId mapId);
		void CreateAction(ActionId actionId, MapId mapId, ButtonId buttonId);
		void SetCurrentMap(MapId mapId);
		void AddListenerToInputAction(ActionId actionId, MapId mapId, const ListenerSignature& listener);
	private:
		std::unordered_map<MapId, std::unordered_map<ActionId, std::unique_ptr<InputAction>>> m_InputActionMaps;
		MapId m_CurrentMapId = 0;
	};
}