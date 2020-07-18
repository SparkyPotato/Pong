#include <Windows.h>
#include <Application.h>
#include "../Application/Connection.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
		Application* app = new APPCLASS();
		app->Begin();

		return 0;
	}
	catch (const Exception& exception)
	{
		MessageBox(nullptr, exception.what(), exception.GetExceptionType(), MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& exception)
	{
		MessageBox(nullptr, exception.what(), "Standard Exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details", "Unknown Exception", MB_OK | MB_ICONERROR);
	}
	return -1;
}