#include "HashCalculator.h"
#include <fstream>
#include <iostream>

const uint64_t BUF_SIZE = 4096;
const uint64_t PROGRESS_INTERVAL_SECONDS = 10;

void HashCalculator::CalculateHashes(std::vector<FileEntry>& files)
{
	std::cout << "Calculating hashes for files with non-unique sizes...\n";

	auto latestProgressTime = std::chrono::steady_clock::now();

	for (unsigned int i = 0; i < files.size(); i++)
	{
		if ((i < files.size() - 1 && files[i].size == files[i + 1].size)
			|| (i > 0 && files[i - 1].size == files[i].size))
		{
			files[i].hash = HashCalculator::CalculateHash(files[i].path, files[i].size);
			auto now = std::chrono::steady_clock::now();
			if (now - latestProgressTime > std::chrono::seconds(PROGRESS_INTERVAL_SECONDS))
			{
				std::cout << " " << i << "/" << files.size() << " processed...\n";
				latestProgressTime = now;
			}
		}
	}

	std::sort(files.begin(), files.end(), [](const FileEntry& a, const FileEntry& b)
		{
			if (a.size != b.size)
				return a.size < b.size;
			else if (a.hash != b.hash)
				return a.hash < b.hash;
			else
				return a.path < b.path;
		});

	int numNonUniqueHashFiles = 0;

	for (unsigned int i = 0; i < files.size(); i++)
	{
		if ((i > 0 && files[i].size == files[i - 1].size && files[i].hash == files[i - 1].hash)
			|| (i < files.size() - 1 && files[i].size == files[i + 1].size && files[i].hash == files[i + 1].hash))
		{
			numNonUniqueHashFiles++;
		}
	}

	std::cout << "Found " << numNonUniqueHashFiles << " files with non-unique sizes and hashes.\n";
}

uint64_t HashCalculator::CalculateHash(const std::filesystem::path& path, uint64_t fileSize)
{
	uint64_t result = 0;

	if (fileSize == 0)
		return result;

	auto readSize = fileSize < BUF_SIZE ? fileSize : BUF_SIZE;
	static char buf[BUF_SIZE];

	{
		std::ifstream s(path, std::ios::binary);
		s.read(buf, readSize);
	}

	for (uint64_t pos = 0; pos < readSize; pos++)
	{
		result = (result * 17) + buf[pos];
	}

	return result;
}
