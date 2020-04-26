#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

struct Entry
{
    Entry(const std::string& path, int64_t size) : path(path), size(size) {}
    std::string path;
    int64_t size;
    unsigned long start = 0;
    int indent = 0;
};

void parseLine(const std::string line, std::vector<Entry>& result)
{
    if (line.length() == 0 || !isdigit(line[0]))
        return;

    int64_t size;
    int pos = 0;
    for (int i = 0; i < line.length(); i++)
    {
        if (!isdigit(line[i]))
        {
            char buf[i + 1];
            strncpy(buf, line.c_str(), i);
            buf[i] = '\0';
            size = atoll(buf);
            pos = i;
            break;
        }
    }

    while (pos < line.size() && isspace(line[pos]))
        pos++;
    
    result.emplace_back(line.substr(pos), size);
}

std::vector<Entry> parseIndata()
{
    std::vector<Entry> result;
    
    std::string line;
    while (!std::cin.eof())
    {
        std::getline(std::cin, line);
        parseLine(line, result);
    }

    return result;
}

void filterSmallEntries(std::vector<Entry>& entries, int64_t sizeLimit)
{
    for (auto i = entries.size(); i > 0; i--)
    {
        if (entries[i - 1].size < sizeLimit)
            entries.erase(entries.begin() + i - 1);
    }
}

void sortByPath(std::vector<Entry>& entries)
{
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b){ return a.path.compare(b.path) < 0; });
}

void removeIntermediates(std::vector<Entry>& entries)
{
    for (int i = 1; i < entries.size(); i++)
    {
        if (entries[i].size == entries[i - 1].size
            && entries[i].path.substr(0, entries[i - 1].path.length()) == entries[i - 1].path)
        {
            entries.erase(entries.begin() + (i - 1));
        }
    }
}

void indent(const Entry& predecessor, Entry& entry)
{
    unsigned long pos = 0;
    int count = 0;

    for (;;)
    {
        if (pos >= predecessor.path.length() || predecessor.path[pos] != entry.path[pos])
        {
            break;
        }
        pos++;
    }

    entry.start = 0;
    entry.indent = 0;
    
    for (;;)
    {
        if (pos < entry.path.length() && entry.path[pos] == '/')
        {
            entry.indent++;
            if (entry.start == 0)
                entry.start = pos;
        }
        if (pos == 0)
            break;
        pos--;
    }
}

void indentPaths(std::vector<Entry>& entries)
{
    for (int i = 1; i < entries.size(); i++)
        indent(entries[i - 1], entries[i]);
}

void outputResult(const std::vector<Entry>& entries, bool reportPercentage)
{
    auto maxSize = std::max_element(
                            entries.cbegin(), entries.cend(),
                            [](const Entry& a, const Entry& b){ return a.size < b.size; }
                            )->size;
    std::stringstream ss;
    ss << maxSize;
    auto numLength = ss.str().length();
    
    for (const auto& entry : entries)
    {
        std::stringstream ss2;
        ss2 << entry.size;
        for (auto i = ss2.str().length(); i < numLength; i++)
            std::cout << " ";
        std::cout << entry.size << "  ";

        if (reportPercentage)
        {
            double perc = 100.0 * static_cast<double>(entry.size) / static_cast<double>(maxSize);
            char buf[10];
            sprintf(buf, "%5.1f", perc);
            std::cout << buf << "  ";
        }
        
        for (auto i = 0; i < entry.indent; i++)
            std::cout << "    ";
        std::cout << entry.path.substr(entry.start) << "\n";
    }
}


int main(int argc, const char * argv[])
{
    int64_t sizeLimit = 0;
    bool mergeIntermediates = false;
    bool indent = false;
    bool showPercent = false;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            std::cout << "Options:\n";
            std::cout << "-f (limit)  Only show directories larger than the limit\n";
            std::cout << "-m          Merge intermediate directories with same size as child\n";
            std::cout << "-i          Show indented directory instead of entire path\n";
            std::cout << "-p          Show percentage of total size\n";
            std::cout << "-h          Show help\n";
            exit(0);
        }
        else if (strcmp(argv[i], "-m") == 0)
            mergeIntermediates = true;
        else if (strcmp(argv[i], "-i") == 0)
            indent = true;
        else if (strcmp(argv[i], "-p") == 0)
            showPercent = true;
        else if (i > 0 && strcmp(argv[i - 1], "-f") == 0)
            sizeLimit = atoll(argv[i]);
    }
    
    auto entries = parseIndata();
    if (sizeLimit > 0)
        filterSmallEntries(entries, sizeLimit);
    sortByPath(entries);
    if (mergeIntermediates)
        removeIntermediates(entries);
    if (indent)
        indentPaths(entries);
    outputResult(entries, showPercent);

    return 0;
}
