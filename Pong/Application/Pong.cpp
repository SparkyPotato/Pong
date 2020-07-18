#include "Pong.h"

TestApp::TestApp()
	:
	Application("Pong", 1280, 720)
{
	Vector vertices[] =
	{
		Vector(-0.025f, -0.2f),
		Vector(-0.025f, 0.2f),
		Vector(0.025f, -0.2f),
		Vector(0.025f, 0.2f)
	};
	Shape square(vertices, 4);
	LeftPlayer = new Entity(square, Vector(-0.9f, 0.f));
	RightPlayer = new Entity(square, Vector(0.9f, 0.f));

	Vector vertice[] =
	{
		Vector(-0.01f, -0.02f),
		Vector(-0.01f, 0.02f),
		Vector(0.01f, -0.02f),
		Vector(0.01f, 0.02f)
	};
	Shape ball(vertice, 4);
	Ball = new Entity(ball, Vector(0.f, 0.f));
}

TestApp::~TestApp()
{
	delete LeftPlayer;
	delete RightPlayer;
	delete Ball;
}

void TestApp::Start()
{
	Application::Start();

	engine->SpawnEntity(LeftPlayer);
	engine->SpawnEntity(RightPlayer);
	engine->SpawnEntity(Ball);
	Ball->SetVelocity(Vector(-3.f, 0.f));
}

void TestApp::Tick(long double deltaTime)
{
	if (engine->GetInput()->IsKeyDown('W') || engine->GetInput()->IsKeyDown('S'))
	{
		if (engine->GetInput()->IsKeyDown('W'))
		{
			LeftPlayer->SetVelocity(Vector(0.f, 2.f) + LeftPlayer->GetVelocity());
		}
		if (engine->GetInput()->IsKeyDown('S'))
		{
			LeftPlayer->SetVelocity(Vector(0.f, -2.f) + LeftPlayer->GetVelocity());
		}
	}
	else
	{
		LeftPlayer->SetVelocity(Vector(0.f, 0.f) - LeftPlayer->GetVelocity() * 0.05f * deltaTime / 1000);
	}

	if (engine->GetInput()->IsKeyDown(0x26) || engine->GetInput()->IsKeyDown(0x28))
	{
		if (engine->GetInput()->IsKeyDown(0x26))
		{
			RightPlayer->SetVelocity(Vector(0.f, 2.f) + RightPlayer->GetVelocity());
		}
		if (engine->GetInput()->IsKeyDown(0x28))
		{
			RightPlayer->SetVelocity(Vector(0.f, -2.f) + RightPlayer->GetVelocity());
		}
	}
	else
	{
		RightPlayer->SetVelocity(Vector(0.f, 0.f) - LeftPlayer->GetVelocity() * 0.05f * deltaTime / 1000);
	}

	if (LeftPlayer->GetVelocity().GetLength() > MaxVelocity)
	{
		LeftPlayer->SetVelocity(LeftPlayer->GetVelocity().GetNormalized() * MaxVelocity);
	}
	if (RightPlayer->GetVelocity().GetLength() > MaxVelocity)
	{
		RightPlayer->SetVelocity(RightPlayer->GetVelocity().GetNormalized() * MaxVelocity);
	}

	if (LeftPlayer->GetPosition().y > 0.9f)
	{
		LeftPlayer->SetPosition(Vector(-0.9f, 0.9f));
	}
	else if (LeftPlayer->GetPosition().y < -0.9f)
	{
		LeftPlayer->SetPosition(Vector(-0.9f, -0.9f));
	}

	if (RightPlayer->GetPosition().y > 0.9f)
	{
		RightPlayer->SetPosition(Vector(0.9f, 0.9f));
	}
	else if (RightPlayer->GetPosition().y < -0.9f)
	{
		RightPlayer->SetPosition(Vector(0.9f, -0.9f));
	}

	if ((Ball->GetPosition().y < RightPlayer->GetPosition().y + 0.2f && Ball->GetPosition().y > RightPlayer->GetPosition().y - 0.2f) && (Ball->GetPosition().x > 0.9f))
	{
		Vector reflection = Vector(-1.f, 0.f) * (-2 * (Vector(-1.f, 0.f) * Ball->GetVelocity())) + Ball->GetVelocity();
		Ball->SetVelocity(reflection + RightPlayer->GetVelocity());
	}

	if ((Ball->GetPosition().y < LeftPlayer->GetPosition().y + 0.2f && Ball->GetPosition().y > LeftPlayer->GetPosition().y - 0.2f) && (Ball->GetPosition().x < -0.9f))
	{
		Vector reflection = Vector(1.f, 0.f) * (-2 * (Vector(1.f, 0.f) * Ball->GetVelocity())) + Ball->GetVelocity();
		Ball->SetVelocity(reflection + LeftPlayer->GetVelocity());
	}

	if (Ball->GetPosition().y > 1.0f)
	{
		Ball->SetVelocity(Vector(0.f, -1.f) * (-2 * (Vector(0.f, -1.f) * Ball->GetVelocity())) + Ball->GetVelocity());
	}

	if (Ball->GetPosition().y < -1.0f)
	{
		Ball->SetVelocity(Vector(0.f, 1.f) * (-2 * (Vector(0.f, 1.f) * Ball->GetVelocity())) + Ball->GetVelocity());
	}

	if (Ball->GetPosition().x > 1.0f || Ball->GetPosition().x < -1.0f)
	{
		Ball->SetPosition(Vector(0.f, 0.f));
		Ball->SetVelocity(Vector(-3.f, 0.f));
	}
}