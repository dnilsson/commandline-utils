#include "ResultWriter.h"
#include <iostream>

void ResultWriter::WriteResult(std::vector<std::vector<FileEntry>> duplicates)
{
	for (const auto& duplicateList : duplicates)
	{
		std::cout << "\nDuplicate files of size " << duplicateList[0].size << " and with hash " << duplicateList[0].hash << ":\n";
		for (const auto& file : duplicateList)
		{
			std::cout << "  " << file.path << "\n";
		}
	}

	int numUnnecessaryFiles = 0;
	uint64_t numUnnecessaryBytes = 0;

	for (const auto& duplicateList : duplicates)
	{
		numUnnecessaryFiles += (duplicateList.size() - 1);
		numUnnecessaryBytes += ((duplicateList.size() - 1) * duplicateList[0].size);
	}

	std::cout << "\nA total of " << numUnnecessaryBytes << " bytes can be regained by removing " << numUnnecessaryFiles << " files.\n";
}
