#pragma once
#include <Window.h>
#include <Input.h>
#include <Vector.h>
#include <Entity.h>

class Application;

class Engine
{
public:
	Engine(Application* owner, std::string appName, int width, int height);
	~Engine();

	int StartMessageLoop();
	void ChangeTitle(std::string newTitle);

	Input* GetInput() { return &input; }

	void InitEngine();

	void SpawnEntity(Entity* entity);

private:
	Window window;
	Input input;
	Application* app;

	std::vector<Entity*> entityList;

	std::chrono::steady_clock::time_point tickPoint;

	std::thread* tickThread;

	void RunTick();
	void UpdatePositions(long double deltaTime);
};