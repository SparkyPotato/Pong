#include "Engine.h"
#include <Application.h>

Engine::Engine(Application* owner, std::string appName, int width, int height)
	:
	window(appName.c_str(), width, height, &input)
{
	app = owner;
}

Engine::~Engine()
{
	delete tickThread;
}

int Engine::StartMessageLoop()
{
	return window.MessageLoop();
}

void Engine::ChangeTitle(std::string newTitle)
{
	window.ChangeWindowTitle(newTitle);
}

void Engine::InitEngine()
{
	tickThread = new std::thread(&Engine::RunTick, this);
	tickPoint = std::chrono::steady_clock::now();
}

void Engine::SpawnEntity(Entity* entity)
{
	entityList.push_back(entity);
}

void Engine::RunTick()
{
	Vector vertex1(-0.5f, 0.5f);
	Vector vertex2(0.5f, 0.5f);
	Vector vertex3(0.f, -0.5f);
	while (true)
	{
		//std::chrono::duration<long double> time_span = duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - tickPoint);
		window.GetGraphics().ClearFrame(0.f, 0.f, 0.f);
		app->Tick(0);
		UpdatePositions(0);
		window.GetGraphics().DrawEntities(entityList);
		window.GetGraphics().ShowFrame();
		tickPoint = std::chrono::steady_clock::now();
	}
}

void Engine::UpdatePositions(long double deltaTime)
{
	for (Entity* entity : entityList)
	{
		entity->SetPosition(entity->GetPosition() + entity->GetVelocity() / 1000);
	}
}