#include "RootSignature.h"
#include "Utils.h"

namespace UriEngine
{
	CRootSignature::CRootSignature()
	{
		m_pRootSignature = nullptr;
	}

	CRootSignature::~CRootSignature()
	{
		SAFE_DELETE(m_pRootSignature);
	}
}