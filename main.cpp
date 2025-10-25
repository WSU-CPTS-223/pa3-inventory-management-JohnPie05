#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cctype>
#include "DataStore.hpp"

using namespace std;

static DataStore Store;

//helpers
static int startsWith(const char* s, const char* p) {
    size_t i = 0;
    while (p[i] != '\0')
    {
        if (s[i] != p[i]) return 0;
        i++;
    }
    return 1;
}

static void trim(std::string& s)
{
    size_t a = 0, b = s.size();

    while (a < b && std::isspace((unsigned char)s[a])) a++;
    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;

    s = s.substr(a, b - a);
}

static void quotes(std::string& s)
{
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
    {
        s = s.substr(1, s.size() - 2);
    }
}

void printHelp()
{
    cout << "Supported list of commands: " << endl;
    cout << " 1. find <inventoryid> - Finds if the inventory exists. If exists, prints details. If not, prints 'Inventory not found'." << endl;
    cout << " 2. listInventory <category_string> - Lists just the id and name of all inventory belonging to the specified category. If the category doesn't exists, prints 'Invalid Category'.\n"
         << endl;
    cout << " Use :quit to quit the REPL" << endl;
}

bool validCommand(string line)
{
    if (line == ":help") return 1;
    if (startsWith(line.c_str(), "find")) return 1;
    if (startsWith(line.c_str(), "listInventory")) return 1;
    return 0;
}

void evalCommand(string line)
{
    if (line == ":help")
    {
        printHelp();
    }

    // find id
    if (startsWith(line.c_str(), "find"))
    {
        std::string id;

        if (line.size() > 4) id = line.substr(4);

        trim(id);
        if (id.empty())
        {
            cout << "Usage find <inventoryid>\n";
        }

        Product p;
        if (Store.findByID(id, p))
        {
            cout << "Inventory found\n";
            cout << "  Uniq Id      %s\n", p.uniqID.c_str();
            cout << "  Product Name %s\n", p.productName.c_str();
            if (p.categories.empty())
            {
                cout << "  Categories   NA\n";
            } else
            {
                cout << "  Categories   %s\n", p.categories.c_str();
            }
        } else
        {
            cout << "Inventory not found\n";
        }
    }

    if (startsWith(line.c_str(), "listInventory"))
    {
        std::string cat;
        if (line.size() > 14) cat = line.substr(14);

        trim(cat);
        quotes(cat);
        if (cat.empty())
        {
            cout << "Usage listInventory <category_string>\n";
        }


        DynamicArray<std::string> ids;
        if (!Store.listIdsForCategory(cat, ids))
        {
            cout << "Invalid Category\n";
        }

        for (size_t i = 0; i < ids.getSize(); i++)
        {
            std::string name;
            if (Store.getNameForId(ids[i], name))
            {
                cout << "%s %s\n", ids[i].c_str(), name.c_str();
            } else
            {
                cout << "%s NameMissing\n", ids[i].c_str();
            }
        }
    }

    cout << "Command not supported. Enter :help for list of supported commands\n";
}

static bool bootStrap(int argc, const char* argv[])
{
    cout << "\n Welcome to Amazon Inventory Query System" << endl;
    cout << " enter :quit to exit. or :help to list supported commands." << endl;
    cout << "\n> ";

    if (argc < 2)
    {
        cout << "Usage ./app <path_to_csv>\n";
        return false;
    }

    string err;
    if (!Store.loadCSV(argv[1], err))
    {
        cout << "Load error " << err << "\n";
        return false;
    }

    cout << "Data loaded. Ready.\n";
    return true;
}

int main(int argc, char const *argv[])
{
    if (!bootStrap(argc, argv))
    {
        return 1;
    }

    string line;
    cout << "> ";
    while (getline(cin, line) && line != ":quit")
    {
        if (validCommand(line))
        {
            evalCommand(line);
        } else
        {
            cout << "Command not supported. Enter :help for list of supported commands\n";
        }
        cout << "> ";
    }
    return 0;
}
