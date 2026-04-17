#include "SWAEngine/FileManager.h"
#include <filesystem>
#include <fstream>>
#include <string>

using namespace std;

namespace SWAEngine
{
	int FileManager::ReadFile(const char* filePath, OUT char** ppBytes, OUT size_t* pSize)
	{
		*ppBytes = nullptr;
		*pSize = 0;

		// Create a stream
		ifstream stream(filePath, ios::in | ios::binary);
		if (!stream.is_open())
			return FILE_MANAGER_FAILED | FILE_MANAGER_ERROR_OPEN;

		// Get size
		stream.seekg(0, std::ios::end);
		std::streampos position = stream.tellg();
		if (position <= 0)
			return FILE_MANAGER_FAILED | FILE_MANAGER_ERROR_EMPTY;
		*pSize = static_cast<size_t>(position);
		
		// Read into new buffer
		*ppBytes = new char[*pSize];
		stream.seekg(0, std::ios::beg);
		stream.read(*ppBytes, *pSize);

		// Return
		stream.close();
		return FILE_MANAGER_SUCCEEDED;
	}
	int FileManager::WriteTextFile(const char* fileDirectory, const char* fileName, const char* textString, size_t textSize, bool tryCreateDirectory)
	{
		// Try create directory
		if (tryCreateDirectory)
			filesystem::create_directory(fileDirectory);

		// Create file
		ofstream stream(string(fileDirectory).append(fileName), ios::out | ios::binary);
		if (!stream.is_open())
			return FILE_MANAGER_FAILED | FILE_MANAGER_ERROR_CREATE;
		
		// Write to file
		stream.write(textString, textSize);
		
		// Return
		stream.close();
		return FILE_MANAGER_SUCCEEDED;
	}
}