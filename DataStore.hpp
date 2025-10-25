/*
    Author: John Pierce
    Date: 10/25/2025
    Description: this class helps with storing data and managment
*/

#pragma once
#include <string>

#include "DynamicArray.hpp"
#include "HashTable.hpp"
#include "Product.hpp"

class DataStore
{
public:

    DataStore();

    bool loadCSV(const char* path, std::string& errorMsg);

    bool findByID(const std::string& id, Product& out) const;

    bool listIdsForCategory(const std::string& category, DynamicArray<std::string>& outIds) const;

    bool getNameForId(const std::string& id, std::string& outName) const;

    // helpers
    bool deleteById(const std::string& id, std::string& msg);
    bool countByCategory(const std::string& category, size_t& outCount) const;
    void listAllCategories(DynamicArray<std::string>& outCats) const;

    private:

    HashTable<std::string, Product> idIndex;
    HashTable<std::string, DynamicArray<std::string>> catIndex;

    static std::string header(const std::string& in);
    static int headerIndex(const DynamicArray<std::string>& h, const char* target);
    static void splitCategories(const std::string& t, DynamicArray<std::string>& out);
};
