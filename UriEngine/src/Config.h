#pragma once

namespace UriEngine
{
	class CConfig
	{
	public:
		static CConfig& GetInstance();

		void SetEnableDebugLayer(bool value) { m_bEnableDebugLayer = value; }
		bool GetEnableDebugLayer() { return m_bEnableDebugLayer; }
	private:
		CConfig();
		~CConfig();
		CConfig(CConfig const&) = delete;
		void operator=(CConfig const&) = delete;

		bool m_bEnableDebugLayer;
	};
}