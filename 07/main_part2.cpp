#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <list>
#include <algorithm>

constexpr int totalDiskSpace = 70000000;
constexpr int updateNeededSpace = 30000000;

struct folder
{
    std::string name;
    int sizeFiles;
    int sizeFolders;
    int totalSize;
    folder *outerFolder;

    std::map<std::string, folder> folders;
    std::map<std::string, int> files;
};

void calculateTotalSizeOfSubFolder(std::map<std::string, folder> *folders)
{
    for (auto &folder : *folders)
    {
        if (folder.second.folders.size() > 0)
            calculateTotalSizeOfSubFolder(&(folder.second.folders));

        folder.second.totalSize = folder.second.sizeFiles + folder.second.sizeFolders;
        folder.second.outerFolder->sizeFolders += folder.second.totalSize;
    }
}

void filterBiggerFolders(folder rootfolder, int magicSize, std::vector<int> *bigfolders)
{
    for (auto folder : rootfolder.folders)
    {
        if (folder.second.totalSize >= magicSize)
            bigfolders->push_back(folder.second.totalSize);

        if (folder.second.folders.size() >= 0)
            filterBiggerFolders(folder.second, magicSize, bigfolders);
    }
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;
    folder rootFolder;
    folder *currentfolder = &rootFolder;

    rootFolder.name = "/";
    rootFolder.outerFolder = nullptr;
    rootFolder.sizeFiles = 0;
    rootFolder.sizeFolders = 0;
    rootFolder.totalSize = 0;

    bool getLSinput = false;

    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[80];
        std::string line;

        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;

        if (line.length() == 0)
        {
            std::cerr << "parsed empty line" << std::endl; // ok if last line
            continue;
        }

        if (line.find('$') != std::string::npos)
        {
            getLSinput = false;
            if (line.at(2) == 'c') // command cd
            {
                std::string name;
                char buf[80];

                sscanf(line.c_str(), "$ cd %s", &buf[0]);
                name = buf;
                if (name == "..")
                {
                    if (currentfolder->outerFolder) // check if outer folder exists
                        currentfolder = currentfolder->outerFolder;
                }
                else if (name == "/")
                    currentfolder = &rootFolder;
                else
                    currentfolder = &currentfolder->folders.find(name)->second;
            }
            else if (line.at(2) == 'l') // command ls
            {
                getLSinput = true;
                continue;
            }
        }
        else if (line.substr(0, 3) == "dir")
        {
            std::string name;
            char buf[80];
            sscanf(line.c_str(), "dir %s", &buf[0]);
            name = buf;
            if (getLSinput && currentfolder->folders.find(name) == currentfolder->folders.end())
            {
                folder newFolder;
                newFolder.name = name;
                newFolder.outerFolder = currentfolder;
                newFolder.sizeFiles = 0;
                newFolder.sizeFolders = 0;
                newFolder.totalSize = 0;
                currentfolder->folders.emplace(name, newFolder);
                continue;
            }
        }
        else
        {
            uint64_t size;
            std::string name;
            char buf[80];
            sscanf(line.c_str(), "%lu %s", &size, &buf[0]);
            name = buf;

            if (getLSinput && currentfolder->files.find(name) == currentfolder->files.end())
            {
                currentfolder->sizeFiles += size;
                currentfolder->files.emplace(name, size);
            }
        }
    }
    input.close();

    calculateTotalSizeOfSubFolder(&rootFolder.folders);
    rootFolder.totalSize = rootFolder.sizeFiles + rootFolder.sizeFolders;

    int freeDiskSpace = totalDiskSpace - rootFolder.totalSize;
    int minimumToDelete = 0;
    if (freeDiskSpace < updateNeededSpace)
        minimumToDelete = updateNeededSpace - freeDiskSpace;

    std::vector<int> sizeBiggerFolders;

    filterBiggerFolders(rootFolder, minimumToDelete, &sizeBiggerFolders);
    int size = *std::min_element(sizeBiggerFolders.begin(), sizeBiggerFolders.end());

    std::cout << "the result is: " << size << std::endl;

    return 0;
}