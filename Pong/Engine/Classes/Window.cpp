#include "Window.h"

WindowException::WindowException(int lineNumber, const char* fileName, HRESULT hResult) noexcept
	:
	Exception(lineNumber, fileName),
	hResult(hResult)
{}


const char* WindowException::what() const noexcept
{
	std::ostringstream outputStream;
	outputStream << GetExceptionType() << std::endl
				 << "Error Code: " << GetErrorCode() << std::endl
				 << "Description: " << GetErrorDescription() << std::endl
				 << GetFormattedString();
	buffer = outputStream.str();
	return buffer.c_str();
}

const char* WindowException::GetExceptionType() const noexcept
{
	return "Window Exception";
}

std::string WindowException::TranslateErrorCode(HRESULT hResult) noexcept
{
	char* messageBuffer = nullptr;
	DWORD messageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, 
		hResult, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer), 
		0, 
		nullptr
	);

	if (messageLength == 0)
	{
		return "Unidentified error code";
	}

	std::string errorString = messageBuffer;

	LocalFree(messageBuffer);
	return errorString;
}

HRESULT WindowException::GetErrorCode() const noexcept
{
	return hResult;
}

std::string WindowException::GetErrorDescription() const noexcept
{
	return TranslateErrorCode(hResult);
}

WindowManager WindowManager::windowClass;

WindowManager::WindowManager() noexcept
	:
	hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = Window::HandleMessageSetup;
	windowClass.lpszClassName = GetName();
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetInstance();
	windowClass.hIcon = nullptr;
	windowClass.hIconSm = nullptr;
	windowClass.hCursor = nullptr;
	windowClass.hbrBackground = nullptr;

	RegisterClassEx(&windowClass);
}

WindowManager::~WindowManager()
{
	UnregisterClass(windowClassName, GetInstance());
}


Window::Window(const char* windowName, int width, int height, Input* inputRef)
	:
	width(width),
	height(height)
{
	input = inputRef;

	RECT windowRect;
	windowRect.left = 100;
	windowRect.right = windowRect.left + width;
	windowRect.top = 100;
	windowRect.bottom = windowRect.top + height;
	if (AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw WINDOW_LAST_EXCEPTION();
	}

	window = CreateWindow(
		WindowManager::GetName(),
		windowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		WindowManager::GetInstance(),
		this
	);
	if (!window)
	{
		throw WINDOW_LAST_EXCEPTION();
	}

	ShowWindow(window, SW_SHOW);

	graphics = std::make_unique<Graphics>(this);
}

Window::~Window()
{
	DestroyWindow(window);
}

void Window::ChangeWindowTitle(const std::string title)
{
	SetWindowText(window, title.c_str());
}

int Window::MessageLoop()
{
	MSG message;
	int getMessage;
	while ((getMessage = GetMessage(&message, window, 0, 0)) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return getMessage;
}

LRESULT CALLBACK Window::HandleMessageSetup(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		const CREATESTRUCTW* const createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const userWindow = static_cast<Window*>(createStruct->lpCreateParams);
		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userWindow));
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
		return userWindow->HandleMessage(window, message, wParam, lParam);
	}
	return DefWindowProc(window, message, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageThunk(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* const userWindow = reinterpret_cast<Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
	return userWindow->HandleMessage(window, message, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_QUIT:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		input->keyboard.ClearStates();
		break;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000))
		{
			input->keyboard.AddKeydown(static_cast<char>(wParam));
			break;
		}
	case WM_CHAR:
		input->keyboard.AddChar(static_cast<char>(wParam));
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		input->keyboard.AddKeyup(static_cast<char>(wParam));
		break;

	case WM_MOUSEMOVE:
	{
		const POINTS points = MAKEPOINTS(lParam);
		if (points.x >= 0 && points.x < width && points.y >= 0 && points.y < height)
		{
			input->mouse.AddMovementInput(points.x, points.y);
			if (!input->mouse.IsInWindow())
			{
				SetCapture(window);
				input->mouse.EnterWindow();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				input->mouse.AddMovementInput(points.x, points.y);
			}
			else
			{
				ReleaseCapture();
				input->mouse.LeaveWindow();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
		input->mouse.AddLeftDown();
		break;
	case WM_RBUTTONDOWN:
		input->mouse.AddRightDown();
		break;
	case WM_MBUTTONDOWN:
		input->mouse.AddMiddleDown();
		break;
	case WM_LBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		input->mouse.AddLeftUp();
		if (points.x < 0 || points.x >= width || points.y < 0 || points.y >= height)
		{
			ReleaseCapture();
			input->mouse.LeaveWindow();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		input->mouse.AddRightUp();
		if (points.x < 0 || points.x >= width || points.y < 0 || points.y >= height)
		{
			ReleaseCapture();
			input->mouse.LeaveWindow();
		}
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		input->mouse.AddMiddleUp();
		if (points.x < 0 || points.x >= width || points.y < 0 || points.y >= height)
		{
			ReleaseCapture();
			input->mouse.LeaveWindow();
		}
		break;
	}
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			input->mouse.AddWheelUp();
		}
		else
		{
			input->mouse.AddWheelDown();
		}
		break;
	}

	return DefWindowProc(window, message, wParam, lParam);
}
