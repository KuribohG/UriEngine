#pragma once

#include "stdafx.h"
#include "Resource.h"
#include <cstdint>

namespace UriEngine
{
	class CView
	{
	public:
		CView(CResource *pResource);
		virtual ~CView();
		CResource* GetResource() { return m_pResource; }
	protected:
		CResource *m_pResource;
	};

	class CVertexBufferView : public CView
	{
	public:
		CVertexBufferView(CResource *pResource, uint32_t count, uint32_t stride);
		~CVertexBufferView();
		UINT GetSize() { return m_SizeInBytes; }
		UINT GetStride() { return m_StrideInBytes; }
		D3D12_VERTEX_BUFFER_VIEW GetView();
	private:
		UINT m_SizeInBytes;
		UINT m_StrideInBytes;
	};

	class CIndexBufferView : public CView
	{
	public:
		CIndexBufferView(CResource* pResource, uint32_t count, DXGI_FORMAT format);
		~CIndexBufferView();
		UINT GetSize() { return m_SizeInBytes; }
		DXGI_FORMAT GetFormat() { return m_Format; }
		D3D12_INDEX_BUFFER_VIEW GetView();
	private:
		UINT m_SizeInBytes;
		DXGI_FORMAT m_Format;
	};
}