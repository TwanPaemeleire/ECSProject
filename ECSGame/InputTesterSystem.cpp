#include "InputTesterSystem.h"
#include <InputHandler.h>
#include <iostream>
#include <IdCreator.h>

using namespace Bloodforge;

InputTesterSystem::InputTesterSystem()
{
	InputHandler& inputHandler = InputHandler::GetInstance();

	inputHandler.AddListenerToInputAction(CreateId("TestAction"), CreateId("TestMap"), [this](const Bloodforge::InputActionInfo& info)
		{
			OnLeftMouseAction(info);
		});

	inputHandler.AddListenerToInputAction(CreateId("TestActionMotion"), CreateId("TestMap"), [this](const Bloodforge::InputActionInfo& info)
		{
			OnMouseMotionAction(info);
		});
}

void InputTesterSystem::OnLeftMouseAction(const Bloodforge::InputActionInfo& info)
{
	if (info.started)
	{
		//std::cout << "Mouse started at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
	if (info.ongoing)
	{
		//std::cout << "Mouse ongoing at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << " Motion: " << info.InputActionVectorDeltaData.X << " , " << info.InputActionVectorDeltaData.Y << std::endl;
	}
	if (info.finished)
	{
		//std::cout << "Mouse stopped at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
}

void InputTesterSystem::OnMouseMotionAction(const Bloodforge::InputActionInfo& info)
{
	if (info.started)
	{
		//std::cout << "Mouse motion started at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
	if (info.ongoing)
	{
		//std::cout << "Mouse motion ongoing at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << " Motion: " << info.InputActionVectorDeltaData.X << " , " << info.InputActionVectorDeltaData.Y << std::endl;
	}
	if (info.finished)
	{
		//std::cout << "Mouse motion stopped at: " << info.InputActionVectorData.X << " , " << info.InputActionVectorData.Y << std::endl;
	}
}