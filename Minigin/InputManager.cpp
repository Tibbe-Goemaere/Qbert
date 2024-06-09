#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include "Command.h"
#include <iostream>
#include "TimeManager.h"

bool dae::InputManager::ProcessInput()
{

	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) {
			return false;
		}

		if (e.type == SDL_KEYDOWN)
		{
			for (auto& pair : m_KeyboardCommands) 
			{
				if (pair.key == e.key.keysym.sym)
				{
					if (pair.executeOncePerClick)
					{
						if (pair.canPress) 
						{
							pair.pressed = true;
						}
					}
					else
					{
						pair.pressed = true;
					}
				}
			}
		}
		else if (e.type == SDL_KEYUP)
		{
			for (auto& pair : m_KeyboardCommands)
			{
				if (pair.key == e.key.keysym.sym)
				{
					if (pair.executeOncePerClick)
					{
						pair.canPress = true;
					}
					else
					{
						pair.pressed = false;
					}
				}
			}
		}
		
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (auto& pair : m_KeyboardCommands)
	{
		if (pair.pressed)
		{
			pair.pCommand->Execute();
			if (pair.executeOncePerClick)
			{
				pair.pressed = false;
				pair.canPress = false;
			}
		}
	}

	for (const auto& controller : m_pControllers)
	{
		controller->Update();
	}

	for (auto& controllerCommand : m_ControllerCommands)
	{
		const auto& pController = m_pControllers[controllerCommand.idx];
		if (!pController)
		{
			return false;
		}


		if (pController->IsDown(controllerCommand.key))
		{
			if (controllerCommand.executeOncePerClick)
			{
				if (controllerCommand.canPress)
				{
					controllerCommand.pressed = true;
				}
			}
			else
			{
				controllerCommand.pressed = true;
			}
		}
		else if(pController->IsPressed(controllerCommand.key))
		{
			if (controllerCommand.executeOncePerClick)
			{
				if (controllerCommand.canPress)
				{
					controllerCommand.pressed = true;
				}
			}
			else
			{
				controllerCommand.pressed = true;
			}
		}
		else if (pController->IsUp(controllerCommand.key))
		{
			if (controllerCommand.executeOncePerClick)
			{
				controllerCommand.canPress = true;
			}
			else
			{
				controllerCommand.pressed = false;
			}
		}
		if (controllerCommand.pressed)
		{
			controllerCommand.pCommand->Execute();
			if (controllerCommand.executeOncePerClick)
			{
				controllerCommand.pressed = false;
				controllerCommand.canPress = false;
			}
		}
		
	}

	return true;
}

void dae::InputManager::BindCommand(SDL_Keycode key, std::unique_ptr<Command> pCommand, bool executeOncePerClick)
{
	m_KeyboardCommands.push_back(KeyboardCommandPair{key, std::move(pCommand),executeOncePerClick });
}

void dae::InputManager::BindCommand(Controller::ControllerButton key, std::unique_ptr<Command> pCommand, bool executeOncePerClick, int idx)
{
	if (idx >= static_cast<int>(m_pControllers.size()))
	{
		m_pControllers.push_back(std::make_unique<Controller>(idx));
	}
	m_ControllerCommands.push_back(ControllerCommandPair{ key, std::move(pCommand),executeOncePerClick });
}

void dae::InputManager::UnregisterCommands()
{
	m_ControllerCommands.clear();
	m_KeyboardCommands.clear();
}

void dae::InputManager::UnregisterCommand(Command* pCommand)
{
	// Remove from keyboard commands
	m_KeyboardCommands.erase(
		std::remove_if(m_KeyboardCommands.begin(), m_KeyboardCommands.end(),
			[pCommand](const KeyboardCommandPair& pair) {
				return pair.pCommand.get() == pCommand;
			}),
		m_KeyboardCommands.end()
	);

	// Remove from controller commands
	m_ControllerCommands.erase(
		std::remove_if(m_ControllerCommands.begin(), m_ControllerCommands.end(),
			[pCommand](const ControllerCommandPair& pair) {
				return pair.pCommand.get() == pCommand;
			}),
		m_ControllerCommands.end()
	);
}
