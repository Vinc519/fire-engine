#include "Directory.h"

#include "core/debug/Assert.h"
#include "core/debug/Logger.h"
#include "File.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstring>

constexpr u8 MAX_DIR_DEPTH = 16;

b8 DirectoryCreate(const char* path)
{ 
	FE_ASSERT_MSG(path != nullptr, "Path is null");
	// TODO: Get the return value.
	return CreateDirectory(path, nullptr);
}

// TODO: Replace this stack by a real data strucure.
b8 DirectoryDelete(const char* path, b8 recursive)
{ 
	FE_ASSERT_MSG(path != nullptr, "Path is null");

	b8 allDeleted = TRUE;
	if (!recursive)
		allDeleted = static_cast<b8>(RemoveDirectory(path));
	else
	{
		char toVisit[MAX_DIR_DEPTH][MAX_PATH] = { 0 };
		u32 visitTop = 0;

		char toDelete[MAX_DIR_DEPTH][MAX_PATH] = { 0 };
		u32 deleteTop = 0;

		strcpy_s(toVisit[visitTop++], path);

		while (visitTop > 0)
		{
			char actualPath[MAX_PATH] = { 0 };
			strcpy_s(actualPath, MAX_PATH, toVisit[--visitTop]);
			strcpy_s(toDelete[deleteTop++], MAX_PATH, actualPath);
			memset(toVisit[visitTop], 0, MAX_PATH);

			WIN32_FIND_DATA fileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;
			char searchPath[MAX_PATH];
			snprintf(searchPath, MAX_PATH, "%s\\*", actualPath);
			hFind = FindFirstFile(searchPath, &fileData);
			if (hFind == INVALID_HANDLE_VALUE)
				continue;

			do
			{
				if (strcmp(fileData.cFileName, ".") == 0 || strcmp(fileData.cFileName, "..") == 0)
					continue;

				if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					FE_ASSERT_MSG(visitTop < MAX_DIR_DEPTH && deleteTop < MAX_DIR_DEPTH,
					              "DirectoryDelete: max directory depth reached");
					snprintf(toVisit[visitTop++], MAX_PATH, "%s\\%s", 
							 actualPath,
					         fileData.cFileName
					);
					strcpy_s(toDelete[deleteTop++], toVisit[visitTop - 1]);
				}
				else
				{
					char filePath[MAX_PATH];
					snprintf(filePath, MAX_PATH, "%s\\%s", actualPath, fileData.cFileName);
					if (!FileDelete(filePath))
					{
						FE_LOG_ERROR("DirectoryDelete: failed to delete '%s' (error %lu)", filePath,
						             GetLastError());
						allDeleted = FALSE;
					}
				}
			} while (FindNextFile(hFind, &fileData) != 0);

			if (!FindClose(hFind))
			{
				FE_LOG_ERROR("DirectoryDelete: FindClose failed (error %lu)", GetLastError());
				allDeleted = FALSE;
			}
		}

		while (deleteTop > 0)
		{
			if (!RemoveDirectory(toDelete[--deleteTop]))
			{
				FE_LOG_ERROR("DirectoryDelete: failed to remove '%s' (error %lu)",
				             toDelete[deleteTop], GetLastError());
				allDeleted = FALSE;
			}
		}
	}

	return allDeleted;
}

b8 DirectoryExists(const char* path)
{ 
	FE_ASSERT_MSG(path != nullptr, "Path is null");
	WIN32_FILE_ATTRIBUTE_DATA fileInfo = { 0 };
	if (GetFileAttributesEx(path, GetFileExInfoStandard, &fileInfo))
	{
		return (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}
	return FALSE;
}

u32 DirectoryListFiles(const char* path, char** outNames, char* outBuffer, u32 maxCount)
{
	FE_ASSERT_MSG(path != nullptr, "Path is null");
	FE_ASSERT_MSG(outBuffer != nullptr, "outBuffer is null");
	FE_ASSERT_MSG(outNames != nullptr, "outNames is null");
	FE_ASSERT_MSG(maxCount > 0, "maxCount is less than or equal to 0");

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char researchPath[MAX_PATH];

	// Préparation du chemin de recherche (ajoute \* pour lister tous les fichiers)
	snprintf(researchPath, MAX_PATH, "%s\\*", path);

	// Search the first file
	hFind = FindFirstFile(researchPath, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		FE_LOG_ERROR("DirectoryListFiles: cannot open directory '%s'", path);
		return 0;
	}

	// File browsing
	u32 fileCount = 0;
	do
	{
		// Ignore directories
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			// Max number of caracters for the name is MAX_WINDOWS_FILE_NAME_SIZE.
			char *dest = outBuffer + (fileCount * MAX_WINDOWS_FILE_NAME_SIZE);
			snprintf(dest, MAX_WINDOWS_FILE_NAME_SIZE, "%s", findFileData.cFileName);
			outNames[fileCount++] = dest;

			if (fileCount >= maxCount)
				break;
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	// Close the handle
	FindClose(hFind);
	hFind = nullptr;

	return fileCount;
}