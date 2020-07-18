#pragma once
#include <Minimal.h>
#include <Windows.h>
#include <Engine.h>

class Application
{
public:
	Application(std::string appName, int width, int height);
	~Application();

	void Begin();
	virtual void Tick(long double deltaTime);

protected:
	Engine* engine;
	virtual void Start();

private:
	std::thread* appMainThread;
};