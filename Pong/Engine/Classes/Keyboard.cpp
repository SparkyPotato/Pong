#include "Keyboard.h"

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

Keyboard::Event Keyboard::GetNextEvent()
{
	if (keyBuffer.size() > 0)
	{
		Event event = keyBuffer.front();
		keyBuffer.pop();
		return event;
	}
	return Event(Event::Type::Invalid, 0);
}

char Keyboard::KeyChar()
{
	if (charBuffer.size() > 0)
	{
		char character = charBuffer.front();
		charBuffer.pop();
		return character;
	}
	return '\0';
}

void Keyboard::AddKeydown(char keyCode)
{
	keyStates[keyCode] = 1;
	keyBuffer.push(Event(Event::Type::Press, keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::AddKeyup(char keyCode)
{
	keyStates[keyCode] = 0;
	keyBuffer.push(Event(Event::Type::Release, keyCode));
	TrimBuffer(keyBuffer);
}


void Keyboard::AddChar(char keyCode)
{
	charBuffer.push(keyCode);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearStates()
{
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T> buffer)
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
