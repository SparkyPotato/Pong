#pragma once
#include <Minimal.h>

class Keyboard
{
	friend class Window;
public:
	Keyboard();
	~Keyboard();

	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
		Event(Type type, char key)
			:
			type(type),
			key(key)
		{}

		Type GetType() { return type; }
		char GetKey() { return key; }
	private:
		Type type;
		char key;
	};

	bool IsKeyDown(char keyCode) { return keyStates[keyCode]; };

	Event GetNextEvent();

	char KeyChar();

private:
	static constexpr unsigned int bufferSize = 32;

	std::bitset<256> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;

	template<typename T>
	static void TrimBuffer(std::queue<T> buffer);

	void AddKeydown(char keyCode);
	void AddKeyup(char keyCode);
	void AddChar(char keyCode);
	void ClearStates();
};