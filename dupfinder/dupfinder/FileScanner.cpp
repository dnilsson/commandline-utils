#include "FileScanner.h"
#include <iostream>

std::vector<FileEntry> FileScanner::Scan()
{
    std::cout << "Scanning files...\n";

    std::vector<FileEntry> result;

    try
    {
        for (auto& p : std::filesystem::recursive_directory_iterator(".", std::filesystem::directory_options::skip_permission_denied))
        {
            if (p.is_regular_file())
            {
                result.emplace_back(p.path(), p.file_size());
            }
        }
    }
    catch (std::filesystem::filesystem_error e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }

    std::sort(result.begin(), result.end(), [](const FileEntry& a, const FileEntry& b)
        {
            if (a.size != b.size)
                return a.size < b.size;
            else
                return a.path < b.path;
        });

    int numNonUniqueSizeFiles = 0;
    for (unsigned int i = 0; i < result.size(); i++)
    {
        if ((i < result.size() - 1 && result[i].size == result[i + 1].size)
            || (i > 0 && result[i - 1].size == result[i].size))
        {
            numNonUniqueSizeFiles++;
        }
    }

    std::cout << "Found " << numNonUniqueSizeFiles << " files with non-unique sizes, out of totally " << result.size() << " files.\n";

    return result;
}
