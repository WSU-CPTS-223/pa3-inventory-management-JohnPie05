/*
    Author: John Pierce
    Date: 10/22/2025
    Description: this is a vector like arrray implemented from scratch
*/

#pragma once

#include <cstddef>
#include <cstdlib>

template <typename T>
class DynamicArray
{
    public:
        DynamicArray(): data(nullptr), size(0), cap(0) {}

        DynamicArray(const DynamicArray& other) : data(nullptr), size(0), cap(0)
        {
            reserve(other.cap);
            for (size_t i = 0; i < other.size; i++)
            {
                data[i] = other.data[i];
            }
            size = other.size;
        }

    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this == &other) return *this;

        clear();
        reserve(other.cap);

        for (size_t i = 0; i < other.size; i++)
        {
            data[i] = other.data[i];
        }

        size = other.size;
        return *this;
    }

    ~DynamicArray() { std::free(data); }

    void reserve(size_t newCap)
    {
        if (newCap <= cap) return;

        T* newData = (T*)std::malloc(sizeof(T) * newCap);

        if (!newData) return;

        for (size_t i = 0; i < size; i++) newData[i] = data[i];
        std::free(data);
        data = newData;
        cap  = newCap;
    }

    void pushBack(const T& value)
    {
        grow();
        data[size++] = value;
    }

    size_t size() const { return size; }

    void clear()
    {
        std::free(data);

        data = nullptr;
        size = 0;
        cap  = 0;
    }

    T& operator[](size_t index) { return _data[index]; }

    const T& operator[](size_t index) const { return data[index]; }

    bool removeFirst(const T& value)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (data[i] == value)
            {
                // shift down
                for (size_t j = i + 1; j < size; j++)
                {
                    data[j - 1] =_data[j];
                }
                size--;
                return true;
            }
        }
        return false;
    }

    private:
        T* data;
        size_t size;
        size_t cap;

        void grow()
        {
            size_t newCap;
            if (_cap == 0)
            {
                newCap = 4;
            }else 
            {
                newCap = cap * 2;
            }

            reserve(newCap);
        }
        
};
