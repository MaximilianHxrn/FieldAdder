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

using namespace std;

string fileType;
string tableName;
string fieldID;
string fieldName;
string fieldType;
string branchName;

void processFile(string fileName)
{
    ifstream myfile(fileName);
    stringstream os;
    string line;
    regex r(fileType + " .*extends \"" + tableName + "\"");
    getline(myfile, line);
    if (!regex_match(line, r) | !myfile.is_open())
    {
        return;
    }
    os << line << endl;
    while (getline(myfile, line))
    {
        os << line << endl;
    }
    string newField("fields\n\t{\n\t\tfield(" + fieldID + "; \"" + fieldName + "\"; " + fieldType + ")\n\t\t{");
    if (fieldName.rfind("SIT ", 0) == 0)
    {
        fieldName = fieldName.substr(4, fieldName.length());
    }
    newField += "\n\t\t\tCaption = '" + fieldName + "';";
    newField += "\n\t\t\tDataClassification = CustomerContent;";
    newField += "\n\t\t\tDescription = '" + branchName + "';";
    newField += "\n\t\t}";
    string temp = os.str();
    string temp_for_check = temp;
    temp = regex_replace(temp, regex("fields\n.*\\{"), newField);
    if (temp.compare(temp_for_check) != 0)
    {
        ofstream write;
        write.open(fileName, ios::out | ios::binary);
        write << temp;
        write.close();
    }
}

bool hasEnding(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

void processFolder(string folder)
{
    // Copied from:
    // https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c
    cout << "searching: " << folder << endl;
    string process_path = folder + "\\";
    string search_path = folder + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if (hasEnding(fd.cFileName, ".al"))
                {
                    string temp;
                    std::string ws(fd.cFileName);
                    std::string s(ws.begin(), ws.end());
                    temp = process_path + s;
                    processFile(temp.c_str());
                }
            }
            else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                string temp;
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

    // fileType = "tableextension";
    // tableName = "Sales Header";
    // fieldID = "55145";
    // fieldName = "SIT Test Field";
    // fieldType = "Decimal";
    // branchName = "SC123456";
    tableName = replaceAll(tableName, ".", "\\.");
    processFolder(argv[7]);
    return 0;
}