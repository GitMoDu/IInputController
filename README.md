# IInputController

Embedded virtual controller, inspired by RetroArch's RetroPad.

## IInputController
- Read API for controller state
- Optional shallow copy, for double buffering state
- Bit field button states, optimized for low RAM and fast copy
- ControllerParser to extract action events or digital values from an analog source
	- Button Up/Down
 	- Joystick Up/Down/Left/Right
  	- Axis Up/Down

### WriteInputController
- Inherits from IInputController
- Write API for controller state
- IInputMapper helper to translate "analog" values (Joysticks and Sliders)


## Supported Source Controllers
- Arduino ADC+IO simple controller.
- Nintendo64/GameCube controllers (JoybusOverUart https://github.com/GitMoDu/JoybusOverUart)

## Task Based Beatures
Task features depend on Task Scheduler https://github.com/arkhipenko/TaskScheduler

- ControllerMap: fixed period, template mapper from source to typed IInputController
- ControllerDispatch: fixed period notifier for dispatch interface
- ControllerToX360: fixed period mapper from IInputController to USB-XBox360 controller

## Examples provided

- Source Controller to IInputController
	- Arduino ADC+IO
	- Nintendo64/GameCube

- Dispatch listener with IInputControllerDispatch interface

- Button Actions
	- Single action for each button event
 	- Actions expire after each parse step
	- Joystick digitalization, with hysteresis

- Nintendo64/GameCube controllers Passthrough to XBox360
	- STM32Duino only https://github.com/rogerclarkmelbourne/Arduino_STM32
	- USB-XBox360 https://github.com/arpruss/USBComposite_stm32f1

 
 
