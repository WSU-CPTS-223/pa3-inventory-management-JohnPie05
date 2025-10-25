/*
    Author: John Pierce
    Date: 10/22/2025
    Description: this is a vector like array implemented from scratch
*/

#pragma once

#include <cstddef>
#include <cstdlib>

template <typename T>
class DynamicArray
{
public:
    DynamicArray() : data(nullptr), size(0), cap(0) {}

    DynamicArray(const DynamicArray& other) : data(nullptr), size(0), cap(0)
    {
        reserve(other.cap);
        for (size_t i = 0; i < other.getSize(); i++)
        {
            data[i] = other.data[i];
        }
        size = other.getSize();
    }

    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this == &other) return *this;

        clear();
        reserve(other.cap);

        for (size_t i = 0; i < other.getSize(); i++)
        {
            data[i] = other.data[i];
        }

        size = other.getSize();
        return *this;
    }

    ~DynamicArray()
    {
        std::free(data);
    }

    void reserve(size_t newCap)
    {
        if (newCap <= cap) return;

        T* newData = (T*)std::malloc(sizeof(T) * newCap);
        if (!newData) return;

        for (size_t i = 0; i < size; i++) newData[i] = data[i];

        std::free(data);
        data = newData;
        cap = newCap;
    }

    void pushBack(const T& value)
    {
        growIfNeeded();
        data[size] = value;
        size = size + 1;
    }

    // Renamed getter — avoids name clash
    size_t getSize() const { return size; }

    void clear()
    {
        std::free(data);
        data = nullptr;
        size = 0;
        cap = 0;
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    bool removeFirst(const T& value)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (data[i] == value)
            {
                for (size_t j = i + 1; j < size; j++)
                {
                    data[j - 1] = data[j];
                }
                size = size - 1;
                return true;
            }
        }
        return false;
    }

private:
    T* data;
    size_t size;
    size_t cap;

    void growIfNeeded()
    {
        size_t newCap;
        if (cap == 0)
        {
            newCap = 4;
        }
        else
        {
            newCap = cap * 2;
        }
        reserve(newCap);
    }
};
