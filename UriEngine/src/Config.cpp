#include "Config.h"

namespace UriEngine
{
	CConfig::CConfig()
	{
		SetEnableDebugLayer(true);
	}

	CConfig::~CConfig()
	{

	}

	CConfig &CConfig::GetInstance()
	{
		static CConfig instance;
		return instance;
	}
}