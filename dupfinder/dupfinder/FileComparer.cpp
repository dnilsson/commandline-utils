#include "FileComparer.h"
#include <fstream>
#include <iostream>

const uint64_t BUF_SIZE = 4096;
const uint64_t PROGRESS_INTERVAL_SECONDS = 10;

std::vector<std::vector<FileEntry>> FileComparer::FindDuplicates(const std::vector<FileEntry>& files)
{
	std::vector<std::vector<FileEntry>> result;

	std::cout << "Comparing contents of files with identical sizes and hashes...\n";

	auto latestProgressTime = std::chrono::steady_clock::now();

	uint64_t totalBytes = 0;
	uint64_t bytesProcessed = 0;
	for (const auto& file : files)
		totalBytes += file.size;

	unsigned int startIndex = 0;

	for (unsigned int i = 1; i < files.size(); i++)
	{
		bytesProcessed += files[i].size;

		if (files[i].size == files[startIndex].size && files[i].hash == files[startIndex].hash)
			continue;

		FileComparer::Compare(files, startIndex, i, result);
		startIndex = i;

		auto now = std::chrono::steady_clock::now();
		if (now - latestProgressTime > std::chrono::seconds(PROGRESS_INTERVAL_SECONDS))
		{
			std::cout << " " << bytesProcessed << "/" << totalBytes << " processed...\n";
			latestProgressTime = now;
		}
	}

	FileComparer::Compare(files, startIndex, files.size(), result);

	int numNonUniqueFiles = 0;

	for (const auto& duplicateList : result)
	{
		numNonUniqueFiles += duplicateList.size();
	}

	std::cout << "Found " << numNonUniqueFiles << " non-unique files.\n";

	return result;
}

void FileComparer::Compare(const std::vector<FileEntry>& files, unsigned int startIndex, unsigned int endIndex, std::vector<std::vector<FileEntry>>& result)
{
	if (endIndex - startIndex < 2)
		return;

	std::vector<std::vector<FileEntry>> duplicates;

	for (unsigned int i = startIndex; i < endIndex; i++)
	{
		const FileEntry& fileEntry = files[i];

		bool duplicate = false;

		for (unsigned int j = 0; j < duplicates.size(); j++)
		{
			if (AreIdentical(fileEntry.path, duplicates[j][0].path, fileEntry.size))
			{
				duplicates[j].push_back(fileEntry);
				duplicate = true;
				break;
			}
		}

		if (!duplicate)
		{
			duplicates.emplace_back();
			duplicates.back().push_back(fileEntry);
		}
	}

	for (const auto& duplicateList : duplicates)
	{
		if (duplicateList.size() > 1)
			result.push_back(duplicateList);
	}
}

bool FileComparer::AreIdentical(const std::filesystem::path& path1, const std::filesystem::path& path2, uint64_t fileSize)
{
	if (fileSize == 0)
		return true;

	std::ifstream s1(path1, std::ios::binary);
	std::ifstream s2(path2, std::ios::binary);

	static char buf1[BUF_SIZE];
	static char buf2[BUF_SIZE];

	while (fileSize > 0)
	{
		auto toRead = fileSize < BUF_SIZE ? fileSize : BUF_SIZE;
		s1.read(buf1, toRead);
		s2.read(buf2, toRead);

		if (memcmp(buf1, buf2, static_cast<size_t>(toRead)) != 0)
			return false;

		fileSize -= toRead;
	}

	return true;
}
