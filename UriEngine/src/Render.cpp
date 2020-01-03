#include "Render.h"
#include "Utils.h"
#include <d3d12.h>
#include <cstdio>
#include "d3dx12.h"
#include <chrono>
#include "Device.h"
#include "Config.h"

namespace UriEngine
{
	CRender::CRender()
	{
		m_pDevice = nullptr;

		m_pCommandQueue = nullptr;
		m_pSwapChain = nullptr;

		m_pRtvDescriptorHeap = nullptr;
		for (int i = 0; i < NUM_FRAMEBUFFERS; i++)
			m_pBackBuffers[i] = nullptr;

		for (int i = 0; i < NUM_FRAMEBUFFERS; i++)
			m_pCommandAllocators[i] = nullptr;
		m_pCommandList = nullptr;

		m_pFence = nullptr;
		m_fenceEvent = nullptr;
		m_fenceValue = 0;
		for (int i = 0; i < NUM_FRAMEBUFFERS; i++)
			m_frameFenceValues[i] = 0;
	}

	CRender::~CRender()
	{

	}

	void CRender::CreateRenderWindow(HWND hwnd)
	{
		m_pDevice = CDevice::GetInstance().GetDevice();
		m_pDxgiFactory = CDevice::GetInstance().GetDxgiFactory();

		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDesc.NodeMask = 0;
		ThrowIfFailed(m_pDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_pCommandQueue)));

		IDXGISwapChain1 *swapChain;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = 1920;
		swapChainDesc.Height = 1080;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = NUM_FRAMEBUFFERS;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = 0; // DXGI_PRESENT_ALLOW_TEARING
		ThrowIfFailed(m_pDxgiFactory->CreateSwapChainForHwnd(m_pCommandQueue,
			hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain));
		ThrowIfFailed(swapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain)));

		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
		rtvDescriptorHeapDesc.NumDescriptors = NUM_FRAMEBUFFERS;
		rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		ThrowIfFailed(m_pDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&m_pRtvDescriptorHeap)));

		auto rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		for (int i = 0; i < NUM_FRAMEBUFFERS; i++)
		{
			ID3D12Resource *backBuffer;
			ThrowIfFailed(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
			m_pDevice->CreateRenderTargetView(backBuffer, nullptr, rtvHandle);
			m_pBackBuffers[i] = backBuffer;
			rtvHandle.Offset(rtvDescriptorSize);
		}

		for (int i = 0; i < NUM_FRAMEBUFFERS; i++)
		{
			ThrowIfFailed(m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocators[i])));
		}
		int currentBackBufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();
		ThrowIfFailed(m_pDevice->CreateCommandList(0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_pCommandAllocators[currentBackBufferIdx],
			nullptr,
			IID_PPV_ARGS(&m_pCommandList)));
		ThrowIfFailed(m_pCommandList->Close());

		ThrowIfFailed(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));
		m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
	}

	uint64_t CRender::Signal()
	{
		++m_fenceValue;
		ThrowIfFailed(m_pCommandQueue->Signal(m_pFence, m_fenceValue));
		return m_fenceValue;
	}

	void CRender::WaitForFenceValue(uint64_t value)
	{
		if (m_pFence->GetCompletedValue() < value)
		{
			auto maxTime = static_cast<DWORD>(std::chrono::milliseconds::max().count());
			ThrowIfFailed(m_pFence->SetEventOnCompletion(value, m_fenceEvent));
			WaitForSingleObject(m_fenceEvent, maxTime);
		}
	}

	void CRender::BeginFrame()
	{
		auto backBufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();
		WaitForFenceValue(m_frameFenceValues[backBufferIdx]);

		m_pCommandAllocators[backBufferIdx]->Reset();
		m_pCommandList->Reset(m_pCommandAllocators[backBufferIdx], nullptr);

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_pBackBuffers[backBufferIdx],
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_pCommandList->ResourceBarrier(1, &barrier);
	}

	void CRender::Present()
	{
		auto prevBackBufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();
		//printf("%d\n", prevBackBufferIdx);
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_pBackBuffers[prevBackBufferIdx],
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_pCommandList->ResourceBarrier(1, &barrier);
		ThrowIfFailed(m_pCommandList->Close());

		auto commandList = (ID3D12CommandList*)m_pCommandList;
		m_pCommandQueue->ExecuteCommandLists(1, &commandList);
		m_frameFenceValues[prevBackBufferIdx] = Signal();

		ThrowIfFailed(m_pSwapChain->Present(0, 0));
		//ThrowIfFailed(m_pSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}

	void CRender::ClearRenderTargetView(float *values)
	{
		auto backBufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			backBufferIdx, m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		m_pCommandList->ClearRenderTargetView(rtv, values, 0, nullptr);
	}
}