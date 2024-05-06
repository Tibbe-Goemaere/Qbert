#pragma once
#include "Singleton.h"
#include <memory>
#include <vector>
#include "Controller.h"
#include "Command.h"

namespace dae
{
	struct KeyboardCommandPair {
		SDL_Keycode key;
		std::unique_ptr<Command> pCommand;
		//bool to show if we dont want to be able to hold the button
		bool executeOncePerClick = false;
		bool canPress = true;
		bool pressed = false;
	};

	struct ControllerCommandPair {
		Controller::ControllerButton key;
		std::unique_ptr<Command> pCommand;
		bool executeOncePerClick = false;
		bool canPress = true;
		bool pressed = false;
		int idx;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void BindCommand(SDL_Keycode key,std::unique_ptr<Command> pCommand, bool executeOncePerClick = false);
		void BindCommand(Controller::ControllerButton key, std::unique_ptr<Command> pCommand, bool executeOncePerClick = false, int idx = 0);

		void UnregisterCommands();
	private:
		std::vector<KeyboardCommandPair> m_KeyboardCommands;
		std::vector<ControllerCommandPair> m_ControllerCommands;
		std::vector<std::unique_ptr<Controller>> m_pControllers;
	};

}
