#include "Application.h"

Application::Application(std::string appName, int width, int height)
{
	engine = new Engine(this, appName, width, height);
}

Application::~Application()
{
	delete engine;
	delete appMainThread;
}

void Application::Begin()
{
	appMainThread = new std::thread(&Application::Start, this);
	engine->StartMessageLoop();
}

void Application::Tick(long double deltaTime)
{

}

void Application::Start()
{
	engine->InitEngine();
}