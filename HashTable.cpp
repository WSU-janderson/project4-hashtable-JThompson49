#include "HashTable.h"

/**
 * Author: Jack Thompson
 * Date: 11/3/2025
 * Course: CS 3100
 */

/**
 * HashTableBucket()
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
HashTableBucket::HashTableBucket() {
    key = "THIS_IS_EMPTY";
    value = 0;
    type = BucketType::ESS;
}

/**
 * HashTableBucket(string key, int value)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
HashTableBucket::HashTableBucket(string key, int value) {
    this->key = key;
    this->value = value;
    this->type = BucketType::NORMAL;
}

/**
 * load(string key, int value)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
void HashTableBucket::load(string key, int value) {
    this->key = key;
    this->value = value;
    this->type = BucketType::NORMAL;
}

/**
 * isEmpty()
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
bool HashTableBucket::isEmpty() const {
    if (type != BucketType::NORMAL) {
        return true;
    } else {
        return false;
    }
}

/**
 * &operator<<(ostream &os, const HashTableBucket &bucket)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
ostream &operator<<(ostream &os, const HashTableBucket &bucket) {
    if (bucket.isEmpty()) {
        os << "<empty>";
    } else {
        os << "<" << bucket.key << ", " << bucket.value << ">";
    }
    return os;
}


/**
 * HashTable(size_t initCapacity)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
HashTable::HashTable(size_t initCapacity) {
    tableData.resize(initCapacity);

    currentSize = 0;
    deletedCount = 0;
    generateOffsets(initCapacity, 0);
}

/**
 * insert(std::string key, size_t value)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
bool HashTable::insert(std::string key, size_t value) {
    if (contains(key)) {
        return false;
    }

    double currentAlpha = alpha();
    if (currentAlpha >= 0.5) {
        size_t newCap = capacity() * 2;
        if (newCap < 1) {
            newCap = 1;
        }
        resize(newCap);
    }
    return insertAfter(key, value);
}

/**
 * insertAfter(string key, size_t value)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
bool HashTable::insertAfter(string key, size_t value) {
    if (contains(key)) {
        return false;
    }

    srand(key.length());
    generateOffsets(capacity(), key.length());

    size_t home = hashIndex(key);
    size_t firstEAR = capacity();

    for (size_t i = 0; i <= offsets.size(); ++i) {
        size_t index;
        if (i == 0) {
            index = home;
        } else {
            index = (home + offsets[i - 1]) % capacity();
        }

        HashTableBucket &bucket = tableData[index];

        if (bucket.type == BucketType::ESS) {
            bucket.load(key, value);
            ++currentSize;
            return true;
        } else if (bucket.type == BucketType::EAR) {
            if (firstEAR == capacity()) {
                firstEAR = index;
            }
        }
    }

    if (firstEAR != capacity()) {
        tableData[firstEAR].load(key, value);
        ++currentSize;
        --deletedCount;
        return true;
    }
    return false;
}

/**
 * resize(size_t newCap)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
void HashTable::resize(size_t newCap) {

    vector<HashTableBucket> old = tableData;

    tableData.clear();
    tableData.resize(newCap);
    currentSize = 0;
    deletedCount = 0;
    offsets.clear();
    generateOffsets(newCap, 0);

    for (size_t i = 0; i < old.size(); ++i) {
        const HashTableBucket &b = old[i];
        if (b.type == BucketType::NORMAL) {
            insertAfter(b.key, b.value);
        }
    }
}

/**
 * remove(string key)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
bool HashTable::remove(string key) {
    size_t home = hash<string>{}(key) % capacity();

    unsigned seed = key.length();
    vector<size_t> probeOffsets;
    for (size_t i = 1; i < capacity(); ++i) {
        probeOffsets.push_back(i);
    }
    default_random_engine rng(seed);
    shuffle(probeOffsets.begin(), probeOffsets.end(), rng);

    for (size_t i = 0; i <= probeOffsets.size(); ++i) {
        size_t index;
        if (i == 0) {
            index = home;
        } else {
            index = (home + probeOffsets[i - 1]) % capacity();
        }

        HashTableBucket &bucket = tableData[index];

        if (bucket.type == BucketType::ESS) {
            return false;
        } else {
            if (bucket.type == BucketType::NORMAL) {
                if (bucket.key == key) {
                    bucket.key = "THIS_IS_EMPTY";
                    bucket.value = 0;
                    bucket.type = BucketType::EAR;
                    if (currentSize > 0) {
                        --currentSize;
                    }
                    ++deletedCount;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * contains(const string &key)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
bool HashTable::contains(const string &key) const {
    size_t home = hash<string>{}(key) % capacity();

    srand(key.length());
    vector<size_t> probeOffsets;
    for (size_t i = 1; i < capacity(); ++i) {
        probeOffsets.push_back(i);
    }
    default_random_engine rng(key.length());
    shuffle(probeOffsets.begin(), probeOffsets.end(), rng);

    for (size_t i = 0; i <= probeOffsets.size(); ++i) {
        size_t index;
        if (i == 0) {
            index = home;
        }
        else {
            index = (home + probeOffsets[i - 1]) % capacity();
        }

        const HashTableBucket &bucket = tableData[index];

        if (bucket.type == BucketType::ESS) {
            return false;
        }
        else {
            if (bucket.type == BucketType::NORMAL) {
                if (bucket.key == key) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * get(const string &key)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
optional<int> HashTable::get(const string &key) const {
    size_t home = hash<string>{}(key) % capacity();

    unsigned seed = key.length();
    vector<size_t> probeOffsets;
    for (size_t i = 1; i < capacity(); ++i) {
        probeOffsets.push_back(i);
    }
    default_random_engine rng(seed);
    shuffle(probeOffsets.begin(), probeOffsets.end(), rng);

    for (size_t i = 0; i <= probeOffsets.size(); ++i) {
        size_t index;
        if (i == 0) {
            index = home;
        } else {
            index = (home + probeOffsets[i - 1]) % capacity();
        }

        const HashTableBucket &bucket = tableData[index];

        if (bucket.type == BucketType::ESS) {
            return nullopt;
        } else {
            if (bucket.type == BucketType::NORMAL) {
                if (bucket.key == key) {
                    return bucket.value;
                }
            }
        }
    }
    return nullopt;
}

/**
 * operator[](const string &key)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
int &HashTable::operator[](const string &key) {
    size_t home = hash<string>{}(key) % capacity();

    unsigned seed = key.length();
    vector<size_t> probeOffsets;
    for (size_t i = 1; i < capacity(); ++i) {
        probeOffsets.push_back(i);
    }
    default_random_engine rng(seed);
    shuffle(probeOffsets.begin(), probeOffsets.end(), rng);

    size_t firstEAR = capacity();

    for (size_t i = 0; i <= probeOffsets.size(); ++i) {
        size_t index;
        if (i == 0) {
            index = home;
        } else {
            index = (home + probeOffsets[i - 1]) % capacity();
        }

        HashTableBucket &bucket = tableData[index];

        if (bucket.type == BucketType::ESS) {
            if (firstEAR != capacity()) {
                tableData[firstEAR].load(key, 0);
                ++currentSize;
                if (deletedCount > 0) {
                    --deletedCount;
                }
                return tableData[firstEAR].value;
            } else {
                bucket.load(key, 0);
                ++currentSize;
                return bucket.value;
            }
        } else {
            if (bucket.type == BucketType::NORMAL) {
                if (bucket.key == key) {
                    return bucket.value;
                }
            } else {
                if (firstEAR == capacity()) {
                    firstEAR = index;
                }
            }
        }
    }

    if (firstEAR != capacity()) {
        tableData[firstEAR].load(key, 0);
        ++currentSize;
        if (deletedCount > 0) {
            --deletedCount;
        }
        return tableData[firstEAR].value;
    }

}

/**
 * keys()
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
vector<string> HashTable::keys() const {
    vector<string> out;
    out.reserve(currentSize);

    for (size_t i = 0; i < tableData.size(); ++i) {
        const HashTableBucket &bucket = tableData[i];
        if (!bucket.isEmpty()) {
            if (bucket.type == BucketType::NORMAL) {
                out.push_back(bucket.key);
            }
        }
    }
    return out;
}

/**
 * alpha()
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
double HashTable::alpha() const {
    return static_cast<double>(currentSize) / static_cast<double>(capacity());
}

/**
 * capacity()
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
size_t HashTable::capacity() const {
    return tableData.size();
}

/**
 * size()
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
size_t HashTable::size() const {
    return currentSize;
}

/**
 * &operator<<(ostream &os, const HashTable &hashTable)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
ostream &operator<<(ostream &os, const HashTable &hashTable) {
    for (size_t i = 0; i < hashTable.tableData.size(); ++i) {
        const HashTableBucket &bucket = hashTable.tableData[i];
        if (!bucket.isEmpty()) {
            os << "Bucket " << i << ": <" << bucket.key << ", " << bucket.value << ">\n";
        }
    }
    return os;
}

/**
 * generateOffsets(size_t cap, unsigned seed)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
void HashTable::generateOffsets(size_t cap, unsigned seed) {
    offsets.clear();
    if (cap < 2) {
        return;
    }
    for (size_t i = 1; i < cap; ++i) {
        offsets.push_back(i);
    }
    default_random_engine rng(seed);
    shuffle(offsets.begin(), offsets.end(), rng);
}

/**
 * hashIndex(const string &key)
 * Purpose:
 * Parameters:
 * Return:
 * Side effects:
 */
size_t HashTable::hashIndex(const string &key) const {
    return hash<string>{}(key) % capacity();
}

