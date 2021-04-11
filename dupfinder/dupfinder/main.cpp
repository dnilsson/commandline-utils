#include "FileScanner.h"
#include "HashCalculator.h"
#include "FileComparer.h"
#include "ResultWriter.h"
#include <iostream>

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		std::cout << "dupfinder - Search for duplicate files in current directory and its subdirectories.\n";
		return 0;
	}

	auto files = FileScanner::Scan();
	HashCalculator::CalculateHashes(files);
	auto result = FileComparer::FindDuplicates(files);
	ResultWriter::WriteResult(result);

	return 0;
}
