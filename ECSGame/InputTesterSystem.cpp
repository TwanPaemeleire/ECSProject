#include "InputTesterSystem.h"
#include <InputHandler.h>
#include <iostream>
#include <IdCreator.h>

using namespace Bloodforge;

void InputTesterSystem::OnStart()
{
	InputHandler& inputHandler = InputHandler::GetInstance();
	inputHandler.CreateMap(CreateId("TestMap"));

	inputHandler.CreateAction(CreateId("TestAction"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_LEFT);
	inputHandler.AddListenerToInputAction(CreateId("TestAction"), CreateId("TestMap"), [this](const Bloodforge::InputActionInfo& info)
		{
			OnLeftMouseAction(info);
		});

	inputHandler.CreateAction(CreateId("TestActionMotion"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_MOTION);
	inputHandler.AddListenerToInputAction(CreateId("TestActionMotion"), CreateId("TestMap"), [this](const Bloodforge::InputActionInfo& info)
		{
			OnMouseMotionAction(info);
		});

	inputHandler.SetCurrentMap(CreateId("TestMap"));
}

void InputTesterSystem::OnLeftMouseAction(const Bloodforge::InputActionInfo& info)
{
	if (info.started)
	{
		std::cout << "Mouse started at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
	if (info.ongoing)
	{
		std::cout << "Mouse ongoing at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << " Motion: " << info.InputActionVectorDeltaData.X << " , " << info.InputActionVectorDeltaData.Y << std::endl;
	}
	if (info.finished)
	{
		std::cout << "Mouse stopped at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
}

void InputTesterSystem::OnMouseMotionAction(const Bloodforge::InputActionInfo& info)
{
	if (info.started)
	{
		std::cout << "Mouse motion started at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
	if (info.ongoing)
	{
		std::cout << "Mouse motion ongoing at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << " Motion: " << info.InputActionVectorDeltaData.X << " , " << info.InputActionVectorDeltaData.Y << std::endl;
	}
	if (info.finished)
	{
		std::cout << "Mouse motion stopped at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
}