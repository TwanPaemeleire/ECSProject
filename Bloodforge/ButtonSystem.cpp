#include "pch.h"
#include "ButtonSystem.h"
#include "InputHandler.h"
#include "IdCreator.h"
#include "EntityManager.h"
#include "ButtonComponent.h"
#include "BloodTime.h"
#include "SpriteComponent.h"
#include "Texture2D.h"

namespace Bloodforge
{
	ButtonSystem::ButtonSystem()
	{
		InputHandler& inputHandler = InputHandler::GetInstance();
		inputHandler.CreateAction(CreateId("LeftMouseButtonDown"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_LEFT);
		inputHandler.AddListenerToInputAction(CreateId("LeftMouseButtonDown"), CreateId("TestMap"), [this](const InputActionInfo& actionInfo)
			{ 
				OnLeftMouseButtonDown(actionInfo); 
			});
		inputHandler.CreateAction(CreateId("MouseMove"), CreateId("TestMap"), BLOODFORGE_KEYCODE_MOUSE_MOTION);
		inputHandler.AddListenerToInputAction(CreateId("MouseMove"), CreateId("TestMap"), [this](const InputActionInfo& actionInfo)
			{
				OnMouseMove(actionInfo);
			});
	}

	void ButtonSystem::OnUpdate()
	{
		EntityQueryResult<ButtonComponent, TransformComponent, SpriteComponent> result = EntityManager::GetInstance().QueryEntities<ButtonComponent, TransformComponent, SpriteComponent>();
		for (EntityView<ButtonComponent, TransformComponent, SpriteComponent> entityView : result.EntityViews)
		{
			ButtonComponent& buttonComp = entityView.GetComponent<ButtonComponent>();
			TransformComponent& transformComp = entityView.GetComponent<TransformComponent>();
			SpriteComponent& spriteComp = entityView.GetComponent<SpriteComponent>();

			Vector2 currentScale = transformComp.GetLocalScale();
			Vector2 targetScale = buttonComp.MouseIsOver ? buttonComp.HoverScale : buttonComp.NormalScale;

			if (currentScale != targetScale) // Not at target scale yet
			{
				buttonComp.HoverTransitionTime += BloodTime::GetInstance().DeltaTime;
				float timeToReachTargetScale = buttonComp.MouseIsOver ? buttonComp.TimeToReachHoverScale : buttonComp.TimeToReachNormalScale;
				currentScale = Vector2::SmoothLerp(buttonComp.ScalingStartScale, targetScale, buttonComp.HoverTransitionTime / timeToReachTargetScale);
				if (buttonComp.HoverTransitionTime >= timeToReachTargetScale)
				{
					currentScale = targetScale;
					buttonComp.HoverTransitionTime = 0.0f;
				}
				transformComp.SetLocalScale(currentScale);
			}

			if (buttonComp.MouseIsOver && buttonComp.MouseIsPressed)
			{
				spriteComp.SetTexture(buttonComp.PressedTexture);
			}
			else if (buttonComp.MouseIsOver)
			{
				spriteComp.SetTexture(buttonComp.HoverTexture);
			}
			else
			{
				spriteComp.SetTexture(buttonComp.NormalTexture);
			}
		}
	}

	void ButtonSystem::OnLeftMouseButtonDown(const InputActionInfo& actionInfo)
	{
		// Check if over button or not 
		if (actionInfo.ongoing) return;
		EntityQueryResult<ButtonComponent, TransformComponent, SpriteComponent> result = EntityManager::GetInstance().QueryEntities<ButtonComponent, TransformComponent, SpriteComponent>();
		for (EntityView<ButtonComponent, TransformComponent, SpriteComponent> entityView : result.EntityViews)
		{
			ButtonComponent& buttonComp = entityView.GetComponent<ButtonComponent>();
			TransformComponent& transformComp = entityView.GetComponent<TransformComponent>();
			bool mouseIsOverButton = MouseIsOverButton(buttonComp, transformComp, actionInfo.InputActionVectorData);
			if (!mouseIsOverButton) continue;
			if (actionInfo.started)
			{
				buttonComp.MouseIsPressed = true;
			}
			else if (actionInfo.finished && buttonComp.MouseIsPressed)
			{
				buttonComp.MouseIsPressed = false;
				buttonComp.OnClick.Invoke(buttonComp);
			}
		}
	}

	void ButtonSystem::OnMouseMove(const InputActionInfo& actionInfo)
	{
		// Check if over button or not 
		// Store the current scale in the component, so we can have support for hovering again when anim is still going on
		EntityQueryResult<ButtonComponent, TransformComponent, SpriteComponent> result = EntityManager::GetInstance().QueryEntities<ButtonComponent, TransformComponent, SpriteComponent>();
		for (EntityView<ButtonComponent, TransformComponent, SpriteComponent> entityView : result.EntityViews)
		{
			ButtonComponent& buttonComp = entityView.GetComponent<ButtonComponent>();
			TransformComponent& transformComp = entityView.GetComponent<TransformComponent>();
			bool mouseIsOverButton = MouseIsOverButton(buttonComp, transformComp, actionInfo.InputActionVectorData);
			if (mouseIsOverButton && !buttonComp.MouseIsOver) // Just started hovering
			{
				buttonComp.MouseIsOver = true;
				buttonComp.HoverTransitionTime = 0.0f;
				buttonComp.ScalingStartScale = transformComp.GetLocalScale();
			}
			else if(!mouseIsOverButton && buttonComp.MouseIsOver) // Just stopped hovering
			{
				buttonComp.MouseIsPressed = false;
				buttonComp.MouseIsOver = false;
				buttonComp.HoverTransitionTime = 0.0f;
				buttonComp.ScalingStartScale = transformComp.GetLocalScale();
			}
		}
	}
	bool ButtonSystem::MouseIsOverButton(const ButtonComponent& buttonComp, TransformComponent& transformComp, Vector2 mousePos)
	{
		Vector2 buttonWorldPos = transformComp.GetWorldPosition();
		Vector2 buttonWorldScale = transformComp.GetWorldScale();
		Vector2 textureSize = (buttonComp.MouseIsOver) ? buttonComp.HoverTexture->GetSize() : buttonComp.NormalTexture->GetSize();
		Vector2 scaledTextureSize = Vector2::Multiply(textureSize, buttonWorldScale);
		Vector2 halfSize = scaledTextureSize / 2.0f;

		Vector2 topLeft = buttonWorldPos - halfSize;
		Vector2 bottomRight = buttonWorldPos + halfSize;

		return (mousePos.X >= topLeft.X && mousePos.X <= bottomRight.X &&
				mousePos.Y >= topLeft.Y && mousePos.Y <= bottomRight.Y);
	}
}