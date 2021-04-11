#pragma once

#include "FileEntry.h"
#include <filesystem>

class HashCalculator
{
public:
	static void CalculateHashes(std::vector<FileEntry>& files);

private:
	static uint64_t CalculateHash(const std::filesystem::path& path, uint64_t fileSize);
};
