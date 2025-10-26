/*
    Author: John Pierce
    Date: 10/25/2025
    Description: this is the .cpp class for CSVReader
*/

#include "CSVReader.hpp"

#include <cstdio>
#include <cstring>

static void parseCsvLine(const char* line, DynamicArray<std::string>& fields)
{
    std::string cur;
    int in= 0;

    for (size_t i = 0; line[i] != '\0'; i++)
    {
        char c = line[i];
        if (c == '"')
        {
            if (in && line[i + 1] == '"')
            {
                cur.push_back('"');
                i++;
            } else
            {
                in = !in;
            }
        } else if (c == ',' && !in) {
            fields.pushBack(cur);
            cur.clear();
        } else cur.push_back(c);
    }
    fields.pushBack(cur);
}

bool CSVReader::readAll(const char* path, DynamicArray< DynamicArray<std::string> >& out)
{
    FILE* infile = std::fopen(path, "rb");  // "rb" handles Windows line endings too
    if (!infile)
    {
        std::fprintf(stderr, "fopen failed for '%s': %s\n", path, std::strerror(errno));
        return false;
    }


    FILE* infile = std::fopen(path, "r");
    if (!infile) return false;

    const size_t num = 8192;
    char* buf = (char*)std::malloc(num);

    size_t n = 0;
    int temp;

    //info on std::fgetc, https://en.cppreference.com/w/cpp/io/c/fgetc.html
    while ((temp = std::fgetc(infile)) != EOF)
    {
        if (n + 1 >= num)
        {
            buf[n] = '\0';
            DynamicArray<std::string> row;
            parseCsvLine(buf, row);
            out.pushBack(row);
            n = 0;
        }
        if (temp == '\n')
        {
            buf[n] = '\0';
            DynamicArray<std::string> row;
            parseCsvLine(buf, row);
            out.pushBack(row);
            n = 0;
        } else if (temp != '\r')
        {
            buf[n++] = (char)temp;
        }
    }

    if (n > 0)
    {
        buf[n] = '\0';
        DynamicArray<std::string> row;
        parseCsvLine(buf, row);
        out.pushBack(row);
    }

    std::free(buf);
    std::fclose(infile);
    return true;
}
