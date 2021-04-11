#pragma once

#include "FileEntry.h"
#include <vector>

class ResultWriter
{
public:
	static void WriteResult(std::vector<std::vector<FileEntry>> duplicates);
};
