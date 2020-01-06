#pragma once

#include "stdafx.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "Shader.h"
#include <cstdint>

namespace UriEngine
{
	constexpr uint32_t MAX_RENDER_TARGETS = 8;

	class CGraphicsPSO : public CPipelineState
	{
	public:
		void SetRootSig(CRootSignature *pRootSig);
		void SetVS(CShader *pVS);
		void SetPS(CShader *pPS);
		void SetDS(CShader *pDS);
		void SetHS(CShader *pHS);
		void SetGS(CShader *pGS);
		void SetBlendState(const D3D12_BLEND_DESC &desc);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC &desc);
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC &desc);
		void SetSampleMask(UINT SampleMask);
		void SetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY_TYPE& TopologyType);
		void SetRTVFormats(int count, DXGI_FORMAT *formats);
		void SetRTVFormat(int slot, DXGI_FORMAT format);
		void SetDSVFormat(DXGI_FORMAT format);
	private:
		CRootSignature *m_pRootSig;
		CShader *m_pVS;
		CShader *m_pPS;
		CShader *m_pDS;
		CShader *m_pHS;
		CShader *m_pGS;
		D3D12_BLEND_DESC m_BlendState;
		D3D12_RASTERIZER_DESC m_RasterizerState;
		D3D12_DEPTH_STENCIL_DESC m_DepthStencilState;
		UINT m_SampleMask;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE m_TopologyType;
		DXGI_FORMAT	m_RTVFormats[8];
		DXGI_FORMAT m_DSVFormat;
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE m_IBProps;
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
}