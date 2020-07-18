#include "Graphics.h"
#include <Window.h>

namespace wrl = Microsoft::WRL;

Graphics::Graphics(Window* window)
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = window->GetWindow();
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&context
		);

	wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);

 	wrl::ComPtr<ID3DBlob> blob;
 	D3DReadFileToBlob(L"VertexShader.cso", &blob);
 	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
 	context->VSSetShader(vertexShader.Get(), nullptr, 0);
 
 	wrl::ComPtr<ID3D11InputLayout> inputLayout;
 	const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
 	{
 		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
 	};
 
 	device->CreateInputLayout(inputDesc, (UINT)std::size(inputDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
 
 	context->IASetInputLayout(inputLayout.Get());

 	D3DReadFileToBlob(L"PixelShader.cso", &blob);
 	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
 	context->PSSetShader(pixelShader.Get(), nullptr, 0);

	context->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);

	auto windowSize = window->GetSize();
	viewport.Width = (float) windowSize.first;
	viewport.Height = (float) windowSize.second;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	context->RSSetViewports(1, &viewport);
}

void Graphics::ShowFrame()
{
	swapChain->Present(1, 0);
}

void Graphics::ClearFrame(float red, float blue, float green)
{
	const float colors[] = { red, blue, green, 1.f };
	context->ClearRenderTargetView(renderTarget.Get(), colors);
}

void Graphics::DrawTriangle(Vector vertex1, Vector vertex2, Vector vertex3)
{
	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{vertex1.x, vertex1.y},
		{vertex2.x, vertex2.y},
		{vertex3.x, vertex3.y}
	};

	wrl::ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA subData = { 0 };
	subData.pSysMem = vertices;

	device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer);
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(3, 0);
}

void Graphics::DrawEntities(const std::vector<Entity*>& entities)
{
	struct Vertex
	{
		float x;
		float y;
	};

	for (Entity* entity : entities)
	{
		int vertexCount = entity->GetShape().GetVertexCount();
		Vertex* vertices = new Vertex[vertexCount];
		Vector* vectors = entity->GetShape().GetVertices();

		for (int i = 0; i < vertexCount; i++)
		{
			vertices[i].x = vectors[i].x + entity->GetPosition().x;
			vertices[i].y = vectors[i].y + entity->GetPosition().y;
		}

		wrl::ComPtr<ID3D11Buffer> vertexBuffer;
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
		bufferDesc.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA subData = { 0 };
		subData.pSysMem = vertices;

		device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer);
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		context->Draw(vertexCount, 0);
	}
}