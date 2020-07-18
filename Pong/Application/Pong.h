#pragma once
#include <Application.h>

class TestApp : public Application
{
public:
	TestApp();
	~TestApp();

	virtual void Start() override;
	virtual void Tick(long double deltaTime) override;

	Entity* LeftPlayer;
	Entity* RightPlayer;
	Entity* Ball;

	float MaxVelocity = 8.f;
};

