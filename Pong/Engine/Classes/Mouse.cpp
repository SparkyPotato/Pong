#include "Mouse.h"

Mouse::Event Mouse::GetNextEvent()
{
	if (buffer.size() > 0)
	{
		Event event = buffer.front();
		buffer.pop();
		return event;
	}
	return Event(Event::Type::Invalid, this);
}

void Mouse::AddMovementInput(int xPos, int yPos)
{
	x = xPos;
	y = yPos;
	buffer.push(Event(Event::Type::Move, this));
	TrimBuffer(buffer);
}

void Mouse::AddLeftDown()
{
	leftButton = true;
	buffer.push(Event(Event::Type::LeftPress, this));
	TrimBuffer(buffer);
}

void Mouse::AddLeftUp()
{
	leftButton = false;
	buffer.push(Event(Event::Type::LeftRelease, this));
	TrimBuffer(buffer);
}

void Mouse::AddRightDown()
{
	rightButton = true;
	buffer.push(Event(Event::Type::RightPress, this));
	TrimBuffer(buffer);
}

void Mouse::AddRightUp()
{
	rightButton = false;
	buffer.push(Event(Event::Type::RightRelease, this));
	TrimBuffer(buffer);
}

void Mouse::AddMiddleDown()
{
	middleButton = true;
	buffer.push(Event(Event::Type::MiddlePress, this));
	TrimBuffer(buffer);
}

void Mouse::AddMiddleUp()
{
	middleButton = false;
	buffer.push(Event(Event::Type::MiddleRelease, this));
	TrimBuffer(buffer);
}

void Mouse::AddWheelUp()
{
	buffer.push(Event(Event::Type::WheelUp, this));
	TrimBuffer(buffer);
}

void Mouse::AddWheelDown()
{
	buffer.push(Event(Event::Type::WheelDown, this));
	TrimBuffer(buffer);
}

void Mouse::EnterWindow()
{
	inWindow = true;
	buffer.push(Event(Event::Type::EnterWindow, this));
	TrimBuffer(buffer);
}

void Mouse::LeaveWindow()
{
	inWindow = false;
	buffer.push(Event(Event::Type::LeaveWindow, this));
	TrimBuffer(buffer);
}

void Mouse::TrimBuffer(std::queue<Event> buffer)
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}