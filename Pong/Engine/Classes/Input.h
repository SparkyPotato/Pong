#pragma once
#include <Minimal.h>
#include <Keyboard.h>
#include <Mouse.h>

class Input
{
	friend class Window;
private:
	Keyboard keyboard;
	Mouse mouse;

public:
	bool IsKeyDown(char keyCode) { return keyboard.IsKeyDown(keyCode); }
	Keyboard::Event GetNextKeyboardEvent() { return keyboard.GetNextEvent(); }
	char GetCharacterInput() { return keyboard.KeyChar(); }

	std::pair<int, int> GetMousePosition() { return { mouse.GetPositionX(), mouse.GetPositionY() }; }
	bool IsLeftMouseButtonDown() { return mouse.IsLeftPressed(); }
	bool IsRightMouseButtonDown() { return mouse.IsRightPressed(); }
	bool IsMiddleMouseButtonDown() { return mouse.IsMiddlePressed(); }
	bool IsMouseInWindow() { return mouse.IsInWindow(); }

	Mouse::Event GetNextMouseEvent() { return mouse.GetNextEvent(); }
};

