/*
    Author: John Pierce
    Date: 10/25/2025
    Description: This is the .cpp file for DataStore.hpp
*/

#include "DataStore.hpp"
#include "CSVReader.hpp"

#include <cctype>
#include <cstring>

DataStore::DataStore() : idIndex(10007), catIndex(10007) {}

std::string DataStore::header(const std::string& in)
{
    std::string out;
    for (char c : in)
    {
        if (std::isalpha((unsigned char)c)) out.push_back(std::tolower(c));
        else if (std::isdigit((unsigned char)c)) out.push_back(c);
        else if (c == ' ' || c == '_') out.push_back('_');
    }
    return out;
}

int DataStore::headerIndex(const DynamicArray<std::string>& h, const char* target)
{
    for (size_t i = 0; i < h.getSize(); i++)
    {
        if (header(h[i]) == target)
            return (int)i;
    }
    return -1;
}

static void trim(std::string& s)
{
    size_t a = 0;
    size_t b = s.size();

    // more info on std::isspace, https://en.cppreference.com/w/cpp/string/byte/isspace.html
    while (a < b && std::isspace((unsigned char)s[a])) a++;

    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;

    s = s.substr(a, b - a);
}

void DataStore::splitCategories(const std::string& t, DynamicArray<std::string>& out)
{
    if (t.empty())
    {
        out.pushBack("N/A");
        return;
    }

    std::string cur;
    for (char c : t)
    {
        if (c == '|')
        {
            trim(cur);
            if (cur.empty()) 
            {
            out.pushBack("N/A");
            }else 
            {
                out.pushBack(cur);
            }
            cur.clear();
        } else 
        {
            cur.push_back(c);
        }
    }

    trim(cur);
    if (cur.empty())
    {
        out.pushBack("NA");
    }else
    {
        out.pushBack(cur);
    }
}

bool DataStore::loadCSV(const char* path, std::string& errorMsg)
{
    DynamicArray< DynamicArray<std::string>> rows;

    if (!CSVReader::readAll(path, rows))
    {
        errorMsg = "\nFailed to open CSV file!!!\n";
        return false;
    }

    if (rows.getSize() == 0)
    {
        errorMsg = "CSV is empty!!";
        return false;
    }

    const DynamicArray<std::string>& header = rows[0];
    const int idxUniq = headerIndex(header, "uniq_id");
    const int idxName = headerIndex(header, "product_name");
    const int idxCate = headerIndex(header, "category");

    for (size_t r = 1; r < rows.getSize(); r++)
    {
        const auto& row = rows[r];

        const std::string& id = row[idxUniq];
        const std::string& name = row[idxName];
        const std::string& cats = row[idxCate];

        Product p(id, name, cats);
        idIndex.insert(id, p);

        DynamicArray<std::string> t;
        splitCategories(cats, t);

        for (size_t i = 0; i < t.getSize(); i++)
        {
            std::string tok = t[i];
            DynamicArray<std::string> list;

            if (catIndex.find(tok, list))
            {
                list.pushBack(id);
                catIndex.insert(tok, list);
            }else
            {
                DynamicArray<std::string> a;
                a.pushBack(id);
                catIndex.insert(tok, a);
            }
        }
    }
    return true;
}

bool DataStore::findByID(const std::string& id, Product& out) const
{
    return idIndex.find(id, out);
}

bool DataStore::listIdsForCategory(const std::string& category, DynamicArray<std::string>& outIds) const
{
    DynamicArray<std::string> ids;

    if (!catIndex.find(category, ids)) return false;

    outIds = ids;
    return true;
}

bool DataStore::getNameForId(const std::string& id, std::string& outName) const
{
    Product p;

    if (!idIndex.find(id, p)) return false;

    outName = p.productName;
    return true;
}

bool DataStore::deleteById(const std::string& id, std::string& msg) {
    Product p;

    if (!idIndex.find(id, p))
    {
        msg = "\nInventory not found\n";
        return false;
    }

    // removes id from every category list it is in
    DynamicArray<std::string> toks;

    splitCategories(p.categories, toks);

    for (size_t i = 0; i < toks.getSize(); i++)
    {
        std::string tok = toks[i];
        DynamicArray<std::string> list;
        if (catIndex.find(tok, list))
        {
            bool removed = list.removeFirst(id);

            if (removed)
            {
                catIndex.insert(tok, list);
            }
        }
    }

    // erase from id index
    bool ok = idIndex.erase(id);

    if (ok)
    {
        msg = "Inventory removed";
    }else
    {
        msg = "Remove failed";
    }
    return ok;
}

bool DataStore::countByCategory(const std::string& category, size_t& outCount) const
{
    DynamicArray<std::string> s;

    if (!catIndex.find(category, s)) return false;

    outCount = s.getSize();
    return true;
}

void DataStore::listAllCategories(DynamicArray<std::string>& outCats) const
{
    catIndex.each([&](const std::string& k, const DynamicArray<std::string>& v) {(void)v;outCats.pushBack(k);} );
}
