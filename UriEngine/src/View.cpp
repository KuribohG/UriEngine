#include "View.h"
#include <cstdio>

namespace UriEngine
{
	CView::CView(CResource *pResource)
	{
		m_pResource = pResource;
	}

	CView::~CView()
	{

	}

	CVertexBufferView::CVertexBufferView(CResource *pResource, uint32_t count, uint32_t stride)
		: CView(pResource)
	{
		m_StrideInBytes = stride;
		m_SizeInBytes = count * stride;
	}
	
	CVertexBufferView::~CVertexBufferView()
	{

	}

	D3D12_VERTEX_BUFFER_VIEW CVertexBufferView::GetView()
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = m_pResource->GetGPUVirtualAddress();
		vbv.SizeInBytes = m_SizeInBytes;
		vbv.StrideInBytes = m_StrideInBytes;
		return vbv;
	}

	CIndexBufferView::CIndexBufferView(CResource* pResource, uint32_t count, DXGI_FORMAT format)
		: CView(pResource)
	{
		m_Format = format;
		switch (m_Format)
		{
		case DXGI_FORMAT_R16_UINT:
			m_SizeInBytes = count * 2; break;
		case DXGI_FORMAT_R32_UINT:
			m_SizeInBytes = count * 4; break;
		default:
			printf("Index buffer format error!");
		}
	}

	CIndexBufferView::~CIndexBufferView()
	{

	}

	D3D12_INDEX_BUFFER_VIEW CIndexBufferView::GetView()
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = m_pResource->GetGPUVirtualAddress();
		ibv.SizeInBytes = m_SizeInBytes;
		ibv.Format = m_Format;
		return ibv;
	}
}