// GamecubeInputController.h

#ifndef _GAMECUBE_INPUT_CONTROLLER_h
#define _GAMECUBE_INPUT_CONTROLLER_h


#include <ControllerTaskTemplateInclude.h>

// https://github.com/GitMoDu/NintendoControllerReader
#include <SerialJoyBusGCController.h>

template<
	typename Calibration,
	const uint32_t UpdatePeriodMillis = 15>
	class GamecubeInputController : public ControllerTaskTemplate<UpdatePeriodMillis>
{
protected:
	using GamecubeButtons = GameCube::Buttons;

	// Template calibrations.
	AxisCentered<int8_t, uint16_t, Calibration::JoyXMin, Calibration::JoyXMax, Calibration::JoyXOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1X;
	AxisCentered<int8_t, uint16_t, Calibration::JoyYMin, Calibration::JoyYMax, Calibration::JoyYOffset, Calibration::JoyDeadZoneRadius, 0, UINT16_MAX> AxisJoy1Y;

	AxisCentered<int8_t, uint16_t, Calibration::JoyCXMin, Calibration::JoyCXMax, Calibration::JoyCXOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2X;
	AxisCentered<int8_t, uint16_t, Calibration::JoyCYMin, Calibration::JoyCYMax, Calibration::JoyCYOffset, Calibration::JoyCDeadZoneRadius, 0, UINT16_MAX> AxisJoy2Y;

	AxisLinear<uint8_t, uint16_t, Calibration::TriggerLMin, Calibration::TriggerLMax, Calibration::TriggerLDeadZone, 0, UINT16_MAX > AxisTriggerL;
	AxisLinear<uint8_t, uint16_t, Calibration::TriggerRMin, Calibration::TriggerRMax, Calibration::TriggerRDeadZone, 0, UINT16_MAX> AxisTriggerR;

	// Controller reader.
	SerialJoyBusGCController Controller;

	//Template members for use in this class.
	using BaseClass = ControllerTaskTemplate<UpdatePeriodMillis>;

	//Template members for use in this class.
	using BaseClass::Delay;
	using BaseClass::Enable;
	using BaseClass::OnControllerReadOk;
	using BaseClass::OnControllerFail;

	enum PollStateEnum
	{
		Polling,
		Reading
	} PollState = PollStateEnum::Polling;

public:
	GamecubeInputController(Scheduler* scheduler, HardwareSerial* serialInstance)
		: ControllerTaskTemplate<UpdatePeriodMillis>(scheduler)
		, Controller(serialInstance)
	{
		Controller.Data.JoystickX = AxisJoy1X.GetCenter();
		Controller.Data.JoystickY = AxisJoy1Y.GetCenter();
	}

	virtual void StartController()
	{
		Controller.Start();
		BaseClass::StartController();
		PollState = PollStateEnum::Polling;
	}

	virtual void StopController()
	{
		BaseClass::StopController();
		Controller.Stop();
	}

	virtual void GetDirection(bool& left, bool& right, bool& up, bool& down)
	{
		left = Controller.Data.Buttons & (1 << GamecubeButtons::Left);
		right = Controller.Data.Buttons & (1 << GamecubeButtons::Right);
		up = Controller.Data.Buttons & (1 << GamecubeButtons::Up);
		down = Controller.Data.Buttons & (1 << GamecubeButtons::Down);
	}

	virtual uint16_t GetJoy1X()
	{
		return AxisJoy1X.Parse(Controller.Data.JoystickX);
	}

	virtual uint16_t GetJoy1Y()
	{
		return AxisJoy1Y.Parse(Controller.Data.JoystickY);
	}

	virtual uint16_t GetJoy2X()
	{
		return AxisJoy2X.Parse(Controller.Data.JoystickCX);
	}

	virtual uint16_t GetJoy2Y()
	{
		return AxisJoy2Y.Parse(Controller.Data.JoystickCY);
	}

	virtual uint16_t GetTriggerL()
	{
		return AxisTriggerL.Parse(Controller.Data.SliderLeft);
	}

	virtual uint16_t GetTriggerR()
	{
		uint16_t value = AxisTriggerR.Parse(Controller.Data.SliderRight);
		if (value > Calibration::TriggerRDeadZone)
		{
			return value;
		}
		else
		{
			return value;
		}
	}

	virtual bool GetLeft() { return  Controller.Data.Buttons & (1 << GamecubeButtons::Left); }
	virtual bool GetRight() { return  Controller.Data.Buttons & (1 << GamecubeButtons::Right); }
	virtual bool GetUp() { return Controller.Data.Buttons & (1 << GamecubeButtons::Up); }
	virtual bool GetDown() { return Controller.Data.Buttons & (1 << GamecubeButtons::Down); }

	virtual bool GetButton0() { return Controller.Data.Buttons & (1 << GamecubeButtons::A); }
	virtual bool GetButton1() { return Controller.Data.Buttons & (1 << GamecubeButtons::B); }
	virtual bool GetButton2() { return Controller.Data.Buttons & (1 << GamecubeButtons::X); }
	virtual bool GetButton3() { return Controller.Data.Buttons & (1 << GamecubeButtons::Y); }
	virtual bool GetButton4() { return Controller.Data.Buttons & (1 << GamecubeButtons::Z); }
	virtual bool GetButton5() { return Controller.Data.Buttons & (1 << GamecubeButtons::L); }
	virtual bool GetButton6() { return Controller.Data.Buttons & (1 << GamecubeButtons::R); }
	virtual bool GetButton7() { return Controller.Data.Buttons & (1 << GamecubeButtons::Start); }

	// Interface controls redirection.
	virtual bool GetButtonAccept() { return GetButton0(); }
	virtual bool GetButtonReject() { return GetButton1(); }
	virtual bool GetButtonHome() { return GetButton7(); }

protected:
	virtual void OnValuesUpdated() {}


protected:
	bool Callback()
	{
		switch (PollState)
		{
		case PollStateEnum::Polling:
			Controller.Poll();
			PollState = PollStateEnum::Reading;
			Task::delay(2);
			break;
		case PollStateEnum::Reading:
			if (Controller.Read())
			{
				OnValuesUpdated();
				OnControllerReadOk();
			}
			else
			{
				OnControllerFail();
			}
			PollState = PollStateEnum::Polling;
			Task::delay(UpdatePeriodMillis - 1);
			break;
		default:
			PollState = PollStateEnum::Polling;
			Task::forceNextIteration();
			break;
		}

		return true;
	}
};
#endif