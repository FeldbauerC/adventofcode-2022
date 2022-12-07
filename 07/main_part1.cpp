#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <list>

struct folder
{
    std::string name;
    int sizeFiles;
    int sizeFolders;
    folder *outerfolder;

    std::map<std::string, folder> folders;
};

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;
    folder rootFolder;
    rootFolder.name = "/";
    rootFolder.outerfolder = nullptr;

    folder *currentfolder = &rootFolder;

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
            if (line.at(2) == 'c') // command cd
            {
                std::cout << "change folder" << std::endl;
                std::string name;
                char buf[80];

                sscanf(line.c_str(), "$ cd %s", &buf[0]);
                name = buf;
                if (name == "..")
                {
                    currentfolder = currentfolder->outerfolder;
                }
                else if (name == "/")
                {
                    currentfolder = &rootFolder;
                }
                currentfolder = &currentfolder->folders.find(name)->second;
            }
            else if (line.at(2) == 'l') // command ls
            {
                std::cout << "ls" << std::endl;
                continue;
            }
        }
        else if (line.substr(0, 3) == "dir")
        {
            std::cout << "found folder" << std::endl;
            std::string name;
            char buf[80];
            sscanf(line.c_str(), "dir %s", &buf[0]);
            name = buf;
            folder newFolder;
            newFolder.name = name;
            newFolder.outerfolder = currentfolder;
            currentfolder->folders.emplace(name, newFolder);
        }
        else
        {
            std::cout << "found file" << std::endl;
            int size;
            std::string name;
            char buf[80];
            sscanf(line.c_str(), "%d %s", &size, &buf[0]);
            name = buf;
            currentfolder->sizeFiles += size;
            currentfolder->outerfolder->sizeFolders = currentfolder->sizeFiles + currentfolder->sizeFolders;
        }
    }
    input.close();

    int size;
    std::vector<folder *> biggerfolders;
    for (auto &&i : currentfolder->folders)
    {
        if ((i.second.sizeFiles + i.second.sizeFolders) >= 10000)
            biggerfolders.push_back(&i.second);
    }
    for (auto &&i : biggerfolders)
    {
        if (i->sizeFolders >= 10000)
        {
            for (auto &&i : i->folders)
            {
                if ((i.second.sizeFiles + i.second.sizeFolders) >= 10000)
                    biggerfolders.push_back(&i.second);
            }
        }
    }
    for (auto &&i : biggerfolders)
    {
        size += i->sizeFolders;
        size += i->sizeFiles;
    }

    std::cout << "the result is: " << size << std::endl;

    return 0;
}