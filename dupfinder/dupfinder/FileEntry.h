#pragma once
#include <filesystem>

struct FileEntry
{
	FileEntry(const std::filesystem::path& path, uint64_t size) : path(path), size(size) {}

	std::filesystem::path path;
	uint64_t size;
	uint64_t hash = 0;
	//bool sameContentAsPredecessor = false;
};
