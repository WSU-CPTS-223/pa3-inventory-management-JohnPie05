/*
    Author: John Pierce
    Date: 10/22/2025
    Description: hash table functions using linear probing
*/

#pragma once
#include <cstddef>
#include <functional> // std::hash
#include <cstdlib>
#include <new>

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable
{
public:
    explicit HashTable(size_t initialCap = 101)
        : table(nullptr), cap(0), size(0), hasher(Hash())
    {
        cap = nextPrime(initialCap);
        table = (Slot*)std::malloc(sizeof(Slot) * cap);
        if (table)
        {
            for (size_t i = 0; i < cap; i++)
            {
                new (&table[i]) Slot();
            }
        }
    }

    ~HashTable() { std::free(table); }

    // Renamed getters so they do not collide with data members
    size_t getSize() const { return size; }
    size_t getCapacity() const { return cap; }

    bool insert(const Key& key, const Value& value)
    {
        grow();

        size_t index = probeIndex(key);
        size_t firstTomb = cap; // cap means no tombstone found yet

        for (size_t i = 0; i < cap; i++)
        {
            Slot& s = table[index];

            if (s.state == EMPTY)
            {
                size_t target = (firstTomb != cap) ? firstTomb : index;
                table[target].key = key;
                table[target].value = value;
                table[target].state = OCCUPIED;
                size = size + 1;
                return true;
            }
            else if (s.state == TOMBSTONE)
            {
                if (firstTomb == cap) firstTomb = index;
            }
            else // OCCUPIED
            {
                if (s.key == key)
                {
                    s.value = value;
                    return true;
                }
            }

            index = (index + 1) % cap;
        }
        return false;
    }

    bool find(const Key& key, Value& out) const
    {
        size_t index = probeIndex(key);
        for (size_t i = 0; i < cap; i++)
        {
            const Slot& s = table[index];

            if (s.state == EMPTY) return false;

            if (s.state == OCCUPIED && s.key == key)
            {
                out = s.value;
                return true;
            }
            index = (index + 1) % cap;
        }
        return false;
    }

    bool erase(const Key& key)
    {
        size_t index = probeIndex(key);
        for (size_t i = 0; i < cap; i++)
        {
            Slot& s = table[index];

            if (s.state == EMPTY) return false;

            if (s.state == OCCUPIED && s.key == key)
            {
                s.state = TOMBSTONE;
                size = size - 1;
                return true;
            }
            index = (index + 1) % cap;
        }
        return false;
    }

    // iterate over occupied slots
    template <typename Fn>
    void each(Fn fn) const
    {
        for (size_t i = 0; i < cap; i++)
        {
            if (table[i].state == OCCUPIED)
            {
                fn(table[i].key, table[i].value);
            }
        }
    }

private:
    enum SlotState { EMPTY = 0, OCCUPIED = 1, TOMBSTONE = 2 };

    struct Slot
    {
        Key       key;
        Value     value;
        SlotState state;
        Slot() : key(), value(), state(EMPTY) {}
    };

    Slot*  table;
    size_t cap;
    size_t size;
    Hash   hasher;

    static bool isPrime(size_t n)
    {
        if (n < 2) return false;
        for (size_t i = 2; i * i <= n; i++)
            if (n % i == 0) return false;
        return true;
    }

    static size_t nextPrime(size_t n)
    {
        size_t x = n;
        if (x < 2) x = 2;
        while (!isPrime(x)) x = x + 1;
        return x;
    }

    size_t probeIndex(const Key& k) const
    {
        return hasher(k) % cap;
    }

    void grow()
    {
        // load factor threshold 0.6
        if (size * 10 < cap * 6) return;
        rehash(cap * 2);
    }

    void rehash(size_t newCap)
    {
        if (newCap < 11) newCap = 11;
        size_t newCap2 = nextPrime(newCap);

        Slot* newTable = (Slot*)std::malloc(sizeof(Slot) * newCap2);
        if (!newTable) return;

        for (size_t i = 0; i < newCap2; i++)
        {
            new (&newTable[i]) Slot();
        }

        for (size_t i = 0; i < cap; i++)
        {
            if (table[i].state == OCCUPIED) {
                const Key& k = table[i].key;
                size_t index = std::hash<Key>{}(k) % newCap2;
                while (newTable[index].state == OCCUPIED)
                    index = (index + 1) % newCap2;
                newTable[index] = table[i];
            }
        }
        std::free(table);
        table = newTable;
        cap   = newCap2;
    }
};
