#include <Windows.h>
#include <Xinput.h>
#include "Controller.h"

using namespace dae;
class Controller::ControllerImpl
{
public:
	ControllerImpl(int controllerIndex)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		controllerIdx = controllerIndex;
	}

	void Update()
	{
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		XInputGetState(controllerIdx, &currentState);

		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned int button) const
	{
		return buttonsPressedThisFrame & button;
	}

	bool IsUpThisFrame(unsigned int button) const
	{
		return buttonsReleasedThisFrame & button;
	}

	bool IsPressed(unsigned int button) const
	{
		return currentState.Gamepad.wButtons & button;
	}
private:
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame{};
	WORD buttonsReleasedThisFrame{};

	int controllerIdx{};
};

Controller::Controller(int controllerIdx)
{
	pImpl = new ControllerImpl(controllerIdx);
}

Controller::~Controller()
{
	delete pImpl;
}

void Controller::Update()
{
	pImpl->Update();
}

bool Controller::IsDown(ControllerButton button) const
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool Controller::IsUp(ControllerButton button) const
{
	return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool Controller::IsPressed(ControllerButton button) const
{
	return pImpl->IsPressed(static_cast<unsigned int>(button));
}