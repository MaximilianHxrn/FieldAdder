#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <windows.h>
#include <sstream>
#include <regex>
#include <chrono>
#include <process.h>
#include <thread>

std::string fileType;
std::string tableName;
std::string fieldID;
std::string fieldName;
std::string fieldType;
std::string branchName;
bool fileFound = false;
bool singleName = false;

void processFile(std::string fileName)
{
    std::ifstream myfile(fileName);
    std::stringstream os;
    std::string line;
    std::regex r("");
    if (fileType == "table")
    {
        if (singleName)
        {
            r.assign(fileType + ".*" + tableName);
        }
        else
        {
            r.assign(fileType + ".*\"" + tableName + "\"");
        }
    }
    else if (fileType == "tableextension")
    {
        if (singleName)
        {
            r.assign(fileType + " .*extends " + tableName);
        }
        else
        {
            r.assign(fileType + " .*extends \"" + tableName + "\"");
        }
    }
    for (int i = 0; i < 3; i++)
    {
        getline(myfile, line);
        os << line << std::endl;
        if (regex_match(line, r))
        {
            fileFound = true;
            break;
        }
    }
    if (!fileFound)
    {
        return;
    }
    while (getline(myfile, line))
    {
        os << line << std::endl;
    }
    std::string newField("");
    if (fieldName.rfind("SIT ", 0) == 0)
    {
        newField += "\n\t\tfield(" + fieldID + "; \"" + fieldName + "\"; " + fieldType + ")\n\t\t{";
        fieldName = fieldName.substr(4, fieldName.length());
    }
    else
    {
        newField += "\n\t\tfield(" + fieldID + "; \"SIT " + fieldName + "\"; " + fieldType + ")\n\t\t{";
    }
    newField += "\n\t\t\tCaption = '" + fieldName + "';\n\t\t\tDataClassification = CustomerContent;";
    newField += "\n\t\t\tDescription = '" + branchName + "';\n\t\t}";
    std::string temp = os.str();
    std::string temp_for_check = temp;
    std::regex regex_first("field\\(.*\n.*\\{");
    std::sregex_iterator iter(temp.begin(), temp.end(), regex_first);
    std::sregex_iterator end;
    int pos;
    while (iter != end)
    {
        pos = iter->position();
        ++iter;
    }
    std::string second = temp.substr(pos);
    std::regex regex_second("\\}");
    std::smatch m;
    if (std::regex_search(second, m, regex_second))
    {
        int pos2 = m.position();
        temp = temp.substr(0, pos + pos2 + 1) + newField + temp.substr(pos + pos2 + 1);
    }
    if (temp_for_check.compare(temp) != 0)
    {
        std::ofstream write;
        write.open(fileName, std::ios::out | std::ios::binary);
        write << temp;
        write.close();
    }
}

bool hasEnding(std::string const &fullstring, std::string const &ending)
{
    if (fullstring.length() >= ending.length())
    {
        return (0 == fullstring.compare(fullstring.length() - ending.length(), ending.length(), ending));
    }
    return false;
}

void processFolder(std::string folder)
{
    // Copied from:
    // https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c
    std::string process_path = folder + "\\";
    std::string search_path = folder + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (fileFound)
            {
                return;
            }
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if (hasEnding(fd.cFileName, ".al"))
                {
                    std::string temp;
                    std::string ws(fd.cFileName);
                    std::string s(ws.begin(), ws.end());
                    temp = process_path + s;
                    processFile(temp.c_str());
                }
            }
            else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string temp;
                std::string ws(fd.cFileName);
                std::string s(ws.begin(), ws.end());
                temp = search_path.substr(0, search_path.length() - 1) + s;
                if ((ws != ".") && (ws != ".."))
                {
                    processFolder(temp);
                }
            }
        } while (::FindNextFileA(hFind, &fd));
        ::FindClose(hFind);
    }
}

std::string &replaceAll(std::string &s, const std::string &from, const std::string &to)
{
    if (!from.empty())
        for (size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}

int main(int argc, char const *argv[])
{
    fileType = argv[1];
    tableName = argv[2];
    fieldID = argv[3];
    fieldName = argv[4];
    fieldType = argv[5];
    branchName = argv[6];

    if (tableName.find_first_of("\t\n ") == std::string::npos)
    {
        singleName = true;
    }

    tableName = replaceAll(tableName, ".", "\\.");
    processFolder(argv[7]);
    if (!fileFound)
    {
        std::cout << "the file wasn´t found." << std::endl;
    }
    return 0;
}