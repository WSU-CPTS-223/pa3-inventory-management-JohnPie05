/*
    Author: John Pierce
    Date: 10/23/2025
    Description: This file reads the csv
*/

#pragma once
#include <string>

#include "DynamicArray.hpp"

class CSVReader
{
public:
    static bool readAll(const char* path, DynamicArray< DynamicArray<std::string>>& out);
};
