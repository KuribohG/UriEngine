#pragma once

#include "stdafx.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <cstdint>
#include "View.h"

namespace UriEngine
{
	constexpr int NUM_FRAMEBUFFERS = 3;
	constexpr uint32_t MAX_VERTEX_BUFFERS = 16;

	class CRender
	{
	public:
		CRender();
		~CRender();
		void CreateRenderWindow(HWND hwnd);
		uint64_t Signal();
		void WaitForFenceValue(uint64_t value);
		void BeginFrame();
		void Present();
		void ClearRenderTargetView(float *values);
		void SetVertexBufferView(uint32_t slot, CVertexBufferView *view);
		void SetIndexBufferView(CIndexBufferView *view);
		void DrawIndexed(uint32_t IndexCount, uint32_t StartIndexLocation = 0, int BaseVertexLocation = 0);
	private:
		void BindVbIb();
		void BindResources();
		void BindPSO();

		ID3D12Device2 *m_pDevice;
		IDXGIFactory4 *m_pDxgiFactory;

		ID3D12CommandQueue *m_pCommandQueue;
		IDXGISwapChain4 *m_pSwapChain;

		ID3D12DescriptorHeap *m_pRtvDescriptorHeap;
		ID3D12Resource *m_pBackBuffers[NUM_FRAMEBUFFERS];

		ID3D12CommandAllocator *m_pCommandAllocators[NUM_FRAMEBUFFERS];
		ID3D12GraphicsCommandList *m_pCommandList;

		ID3D12Fence *m_pFence;
		HANDLE m_fenceEvent;
		uint64_t m_fenceValue;
		uint64_t m_frameFenceValues[NUM_FRAMEBUFFERS];

		CVertexBufferView *m_pVertexBufferViews[MAX_VERTEX_BUFFERS];
		CIndexBufferView *m_pIndexBufferView;
	};
}