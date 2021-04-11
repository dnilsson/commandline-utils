#pragma once

#include "FileEntry.h"
#include <vector>

class FileScanner
{
public:
	static std::vector<FileEntry> Scan();
};
