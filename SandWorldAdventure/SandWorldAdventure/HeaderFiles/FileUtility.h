#pragma once
#include <fstream>

namespace SandboxEngine
{
	static struct FileUtility
	{
	private:
		static bool ms_IsInitialized;
	public:
		static std::ifstream s_InputStream;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="ppOut">pointer to placeholder for array(not sized or initialized)</param>
		/// <param name="pSizeOut"></param>
		/// <param name="fullFilePath"></param>
		static void LoadFile(char** ppOut, long* pSizeOut, const char* fullFilePath,
							bool print = false, long printOff = 0, long printEnd = 0);
	};
}