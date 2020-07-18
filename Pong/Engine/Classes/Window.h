#pragma once
#include <Windows.h>
#include <Minimal.h>
#include <Input.h>
#include <Graphics.h>

#define WINDOW_EXCEPTION( hResult ) WindowException( __LINE__,__FILE__,hResult )
#define WINDOW_LAST_EXCEPTION() WindowException( __LINE__,__FILE__,GetLastError() )

class WindowException : public Exception
{
public:
	WindowException(int lineNumber, const char* fileName, HRESULT hResult) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetExceptionType() const noexcept;
	static std::string TranslateErrorCode(HRESULT hResult) noexcept;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorDescription() const noexcept;
private:
	HRESULT hResult;
};

class WindowManager
{
public:
	static const char* GetName() noexcept { return windowClassName; };
	static HINSTANCE GetInstance() noexcept { return windowClass.hInstance; };
private:
	WindowManager() noexcept;
	~WindowManager();
	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;
	static constexpr const char* windowClassName = "potatoEngine Window";
	static WindowManager windowClass;
	HINSTANCE hInstance;
};

class Window
{
	friend class Input;
public:
	Window(const char* windowName, int width, int height, Input* inputRef);
	~Window();

	void ChangeWindowTitle(const std::string title);
	Graphics& GetGraphics() { return *graphics; }
	std::pair<int, int> GetSize() { return std::make_pair(width, height); }
	HWND GetWindow() { return window; }

	static LRESULT CALLBACK HandleMessageSetup(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	int MessageLoop();
private:
	std::future<int> messageLoop;

	HWND window;
	std::unique_ptr<Graphics> graphics;

	int width;
	int height;

	Input* input;

	static LRESULT CALLBACK HandleMessageThunk(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};