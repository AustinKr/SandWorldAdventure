#pragma once
#include "dllClause.h"

#ifndef OUT
// used for readability
#define OUT
#endif

namespace SWAEngine
{
	// Static utility struct for file management
	struct SWA_ENGINE_API FileManager
	{
		enum
		{
			// Everything went fine
			FILE_MANAGER_SUCCEEDED = 0,
			// Something failed
			FILE_MANAGER_FAILED = 1,

			// Failed to find the file at the given path
			FILE_MANAGER_ERROR_OPEN = 2,
			// Failed to create the file at the given path
			FILE_MANAGER_ERROR_CREATE = 4,
			// The given file was empty
			FILE_MANAGER_ERROR_EMPTY = 8,
		};

		// Outputs the bytes contained inside the file and the size
		static int ReadFile(const char* filePath, OUT char** ppBytes, OUT size_t* pSize);

		// Creates a file at the given path with the given text
		static int WriteTextFile(const char* fileDirectory, const char* fileName, const char* textString, size_t textSize, bool tryCreateDirectory = false);
	};
}