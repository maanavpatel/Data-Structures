/**
 * @file dhhashtable.cpp
 * Implementation of the DHHashTable class.
 */

#include "dhhashtable.h"
using hashes::hash;
using hashes::secondary_hash;
using std::pair;

template <class K, class V>
DHHashTable<K, V>::DHHashTable(size_t tsize)
{
    if (tsize <= 0)
        tsize = 17;
    size = findPrime(tsize);
    table = new std::pair<K, V>*[size];
    should_probe = new bool[size];
    for (size_t i = 0; i < size; i++) {
        table[i] = NULL;
        should_probe[i] = false;
    }
    elems = 0;
}

template <class K, class V>
DHHashTable<K, V>::~DHHashTable()
{
    destroy();
}

template <class K, class V>
DHHashTable<K, V> const& DHHashTable<K, V>::operator=(DHHashTable const& rhs)
{
    if (this != &rhs) {
        destroy();

        copy(rhs);
    }
    return *this;
}

template <class K, class V>
DHHashTable<K, V>::DHHashTable(DHHashTable<K, V> const& other)
{
    copy(other);
}


template <class K, class V>
void DHHashTable<K, V>::destroy()
{
    for (size_t i = 0; i < size; i++)
        delete table[i];
    delete[] table; table = nullptr;
    delete[] should_probe; should_probe = nullptr;
}

template <class K, class V>
void DHHashTable<K, V>::copy(const DHHashTable<K, V>& other)
{
    table = new std::pair<K, V>*[other.size];
    should_probe = new bool[other.size];
    for (size_t i = 0; i < other.size; i++) {
        should_probe[i] = other.should_probe[i];
        if (other.table[i] == NULL)
            table[i] = NULL;
        else
            table[i] = new std::pair<K, V>(*(other.table[i]));
    }
    size = other.size;
    elems = other.elems;
}

template <class K, class V>
void DHHashTable<K, V>::insert(K const& key, V const& value)
{

    /**
     * @todo Implement this function.
     *
     * @note Remember to resize the table when necessary (load factor >=
     *  0.7). **Do this check *after* increasing elems!!** Also, don't
     *  forget to mark the cell for probing with should_probe!
     */

    size_t h = hash(key, size);
    size_t s = secondary_hash(key, size);

    while (table[h] != NULL) {
        h = (h + s) % size;
    }

    table[h] = new pair<K, V>(key, value);
    should_probe[h] = true;

    elems++;

    if (shouldResize()) resizeTable();
}

template <class K, class V>
void DHHashTable<K, V>::remove(K const& key)
{
    /**
     * @todo Implement this function
     */

    size_t h = hash(key, size);
    size_t jump = secondary_hash(key, size);

    while (table[h] != NULL && table[h]->first != key) {
        h = (h + jump) % size;
    }

    if (table[h] == NULL) {
        return;
    } else if (table[h]->first == key) {
        delete table[h];
        table[h] = NULL;
        elems--;
    }
}

template <class K, class V>
int DHHashTable<K, V>::findIndex(const K& key) const
{
    /**
     * @todo Implement this function
     */
    size_t h = hash(key, size);
    size_t s = secondary_hash(key, size);
    size_t start = h;
    while (should_probe[h]) {
        if (table[h] != NULL) {
            if (table[h]->first == key) return h;
        }
        h = (h + s) % size;
        if (start == h) break;
    }
    return -1;
}

template <class K, class V>
V DHHashTable<K, V>::find(K const& key) const
{
    int i = findIndex(key);
    if (i != -1)
        return table[i]->second;
    return V();
}

template <class K, class V>
V& DHHashTable<K, V>::operator[](K const& key)
{
    // First, attempt to find the key and return its value by reference
    int idx = findIndex(key);
    if (idx == -1) {
        // otherwise, insert the default value and return it
        insert(key, V());
        idx = findIndex(key);
    }
    return table[idx]->second;
}

template <class K, class V>
bool DHHashTable<K, V>::keyExists(K const& key) const
{
    return findIndex(key) != -1;
}

template <class K, class V>
void DHHashTable<K, V>::clear()
{
    destroy();

    table = new std::pair<K, V>*[17];
    should_probe = new bool[17];
    for (size_t i = 0; i < 17; i++)
        should_probe[i] = false;
    size = 17;
    elems = 0;
}

template <class K, class V>
void DHHashTable<K, V>::resizeTable()
{
    size_t nSize = findPrime(size * 2);
    std::pair<K, V>** temp = new std::pair<K, V>*[nSize];

    delete[] should_probe;
    should_probe = new bool[nSize];

    for (size_t i = 0; i < nSize; i++) {
        temp[i] = NULL;
        should_probe[i] = false;
    }

    for (size_t slot = 0; slot < size; slot++) {
        if (table[slot] != NULL) {
            size_t h = hashes::hash(table[slot]->first, nSize);
            size_t jump = hashes::secondary_hash(table[slot]->first, nSize);
            size_t i = 0;
            size_t idx = h; 

            while (temp[idx] != NULL) {
                ++i;
                idx = (h + jump*i) % nSize;
            }

            temp[idx] = table[slot];
            should_probe[idx] = true;
        }
    }

    delete[] table;
    table = temp;
    size = nSize;
}
