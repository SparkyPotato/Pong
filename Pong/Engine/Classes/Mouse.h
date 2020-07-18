#pragma once
#include <Minimal.h>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LeftPress,
			LeftRelease,
			RightPress,
			RightRelease,
			MiddlePress,
			MiddleRelease,
			WheelUp,
			WheelDown,
			Move,
			EnterWindow,
			LeaveWindow,
			Invalid
		};

		Event(Type type, const Mouse* mouse)
			:
			type(type),
			x(mouse->x),
			y(mouse->y),
			leftButton(mouse->leftButton),
			rightButton(mouse->rightButton),
			middleButton(mouse->middleButton)
		{}
		std::pair<int, int> GetPosition() { return { x, y }; }
		int GetPositionX() { return x; }
		int GetPositionY() { return y; }
		bool IsLeftButtonPressed() { return leftButton; }
		bool IsRightButtonPressed() { return rightButton; }
		bool IsMiddleButtonPressed() { return middleButton; }

		Type GetType() { return type; }

	private:
		Type type;
		int x;
		int y;
		bool leftButton;
		bool rightButton;
		bool middleButton;
	};

	int GetPositionX() { return x; }
	int GetPositionY() { return y; }
	bool IsLeftPressed() { return leftButton; }
	bool IsRightPressed() { return rightButton; }
	bool IsMiddlePressed() { return middleButton; }
	bool IsInWindow() { return inWindow; }

	Event GetNextEvent();

private:
	int x;
	int y;
	bool leftButton;
	bool rightButton;
	bool middleButton;
	bool inWindow;
	std::queue<Event> buffer;

	static constexpr unsigned int bufferSize = 32;

	void AddMovementInput(int xPos, int yPos);
	void AddLeftDown();
	void AddLeftUp();
	void AddRightDown();
	void AddRightUp();
	void AddMiddleDown();
	void AddMiddleUp();
	void AddWheelUp();
	void AddWheelDown();
	void EnterWindow();
	void LeaveWindow();

	static void TrimBuffer(std::queue<Event> buffer);
};

