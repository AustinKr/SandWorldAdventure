#include "HeaderFiles/FileUtility.h"
#include <string>

namespace SandboxEngine
{
	bool FileUtility::ms_IsInitialized;
	std::ifstream FileUtility::s_InputStream;

	void FileUtility::LoadFile(char** ppOut, long* pSizeOut, const char* fullFilePath, bool print, long printOff, long printEnd)
	{
		// Initialize
		if (!ms_IsInitialized)
		{
			s_InputStream = {};
			ms_IsInitialized = true;
		}
		// Open
		s_InputStream.open(fullFilePath, std::ios::binary); // TODO: Could reuse the input stream
		if (!s_InputStream.is_open())
		{
			fprintf(stderr, std::string("Failed to open file path: ").append(fullFilePath).c_str());
#ifdef _DEBUG
			throw std::invalid_argument("Invalid file path!");
#endif
			* ppOut = nullptr;
			return;
		}
		// Get begin, end, and size
		std::streampos begin, end;
		begin = s_InputStream.tellg();;
		end = s_InputStream.seekg(0, std::ios::end).tellg();
		*pSizeOut = end - begin + 1;
		s_InputStream.seekg(0, std::ios::beg);
		// Read
		*ppOut = new char[*pSizeOut];
		s_InputStream.read(*ppOut, *pSizeOut);
		// Close
		s_InputStream.close();

#ifdef _DEBUG
		if (print)
			for (long i = printOff; i < (printEnd == 0 ? *pSizeOut : std::min(printEnd, *pSizeOut)) - printOff; i++)
			{
				fprintf(stdout, std::string("| ").append(std::to_string((*ppOut)[i])).c_str());
			}
#endif
	}
}