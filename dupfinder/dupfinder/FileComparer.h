#pragma once

#include "FileEntry.h"
#include <filesystem>

class FileComparer
{
public:
	static std::vector<std::vector<FileEntry>> FindDuplicates(const std::vector<FileEntry>& files);

private:
	static void Compare(const std::vector<FileEntry>& files, unsigned int startIndex, unsigned int endIndex, std::vector<std::vector<FileEntry>>& result);
	static bool AreIdentical(const std::filesystem::path& path1, const std::filesystem::path& path2, uint64_t fileSize);
};
