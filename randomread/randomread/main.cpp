#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <sys/stat.h>

void randomRead(const std::string& filePath, uint64_t blockSize, uint64_t align, uint64_t offset, uint64_t iterations)
{
    // 1. Determine how large the file is and how many blocks can fit into it.
    struct stat stat_buf;
    int rc = stat(filePath.c_str(), &stat_buf);
    if (rc != 0)
    {
        std::cerr << "Failed to open file: " << filePath << "\n";
        exit(-1);
    }
    std::cout << "File size in megabytes: " << (static_cast<double>(stat_buf.st_size) / 1024.0 / 1024.0) << "\n";
    
    uint64_t numBlocks = (stat_buf.st_size - offset) / align;
    if (numBlocks == 0)
    {
        std::cerr << "File too small.\n";
        exit(-1);
    }
    std::cout << "Block size in bytes: " << blockSize << "\n";
    std::cout << "File contains " << numBlocks << " eligible blocks." << std::endl;
    
    if (iterations == 0)
        iterations = numBlocks;
    
    // 2. Prepare an array with 0 to the number of blocks, and shuffle it.
    std::vector<uint64_t> blockIndexes(numBlocks);
    for (uint64_t i = 0; i < numBlocks; i++)
        blockIndexes[i] = i;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(blockIndexes.begin(), blockIndexes.end(), g);
    
    // 3. Loop through the array continiously, seek to the block at the index, and read it.
    FILE* pf = std::fopen(filePath.c_str(), "rb");
    if (pf == nullptr)
    {
        std::cerr << "Failed to open file: " << filePath << "\n";
        exit(-1);
    }

    
    
    char buf[blockSize];
    uint64_t blockIndex = 0;
    auto startTime = std::chrono::system_clock::now();
    
    for (uint64_t i = 0; i < iterations; i++)
    {
        if (i % 10000 == 0 && i > 0)
            std::cout << "Read " << i << " blocks." << std::endl;
        //std::cout << "Reading block #" << blockIndexes[blockIndex] << "\n";
        fseek(pf, offset + align * blockIndexes[blockIndex], 0);
        fread(buf, blockSize, 1, pf);
        char a = buf[0];
        blockIndex++;
        if (blockIndex == numBlocks)
            blockIndex = 0;
    }

    auto stopTime = std::chrono::system_clock::now();
    auto elapsedTime = stopTime - startTime;
    
    auto bytesRead = iterations * blockSize;
    
    std::cout << "Done.\n";
    std::cout << "Elapsed time in seconds: " << (static_cast<double>(elapsedTime.count()) / 1000000.0) << "\n";
    std::cout << "Megabytes read: " << (static_cast<double>(bytesRead) / 1024.0 / 1024.0) << "\n";
    std::cout << "Megabytes per second: " << (static_cast<double>(bytesRead) / elapsedTime.count() * 1000000.0 / 1024.0 / 1024.0) << "\n";
    std::cout << "IOPS: " << (static_cast<double>(iterations) / elapsedTime.count() * 1000000.0) << "\n";
    fclose(pf);
}

int main(int argc, const char * argv[])
{
    uint64_t align = 0;
    uint64_t offset = 0;
    uint64_t blockSize = 4096;
    uint64_t iterations = 0;
    std::string filePath;
    bool showUsage = true;
    
    for (int i = 1; i < argc; i++)
    {
        if (i > 0 && strcmp(argv[i - 1], "-a") == 0)
        {
            align = atoll(argv[i]);
        }
        else if (i > 0 && strcmp(argv[i - 1], "-o") == 0)
        {
            offset = atoll(argv[i]);
        }
        else if (i > 0 && strcmp(argv[i - 1], "-b") == 0)
        {
            blockSize = atoll(argv[i]);
        }
        else if (i > 0 && strcmp(argv[i - 1], "-i") == 0)
        {
            iterations = atoll(argv[i]);
        }
        else if (filePath.empty() && strlen(argv[i]) > 0 && argv[i][0] != '-')
        {
            filePath = argv[i];
            showUsage = false;
        }
    }

    if (showUsage)
    {
        std::cout << "Reads blocks randomly from a file, and reports speed and operations per second afterwards.\n";
        std::cout << "\n";
        std::cout << "Usage: ";
        std::cout << "[-b blocksize_in_bytes] ";
        std::cout << "[-a alignment_in_bytes] ";
        std::cout << "[-o offset_in_bytes] ";
        std::cout << "filename\n";
        std::cout << "\n";
        std::cout << "Hints for an accurate result:\n";
        std::cout << "- Run 'purge' before, to clear the disk cache.\n";
        std::cout << "- Don't run more iterations than there are eligible blocks in the file.\n";
        std::cout << "- Use a block size of at least 4096 bytes.\n";
        exit(-1);
    }

    if (align == 0)
        align = blockSize;
    
    randomRead(filePath, blockSize, align, offset, iterations);
    return 0;
}
