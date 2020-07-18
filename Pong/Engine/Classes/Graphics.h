#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <Minimal.h>
#include <d3dcompiler.h>
#include <Entity.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Window;

class Graphics
{
public:
	Graphics(Window* window);
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;
	~Graphics() = default;

	void ShowFrame();
	void ClearFrame(float red, float blue, float green);
	void DrawTriangle(Vector vertex1, Vector vertex2, Vector vertex3);
	void DrawEntities(const std::vector<Entity*>& entities);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

	D3D11_VIEWPORT viewport;
};

