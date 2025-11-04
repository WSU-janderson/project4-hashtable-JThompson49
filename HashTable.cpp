#include "HashTable.h"


/**
* The default constructor can simply set the bucket type to ESS.
 *
*/
HashTableBucket::HashTableBucket() {
    key = "THIS_IS_EMPTY";
    value = 0;
    type = BucketType::ESS;
}

/**
* A parameterized constructor could initialize the key and value, as
* well as set the bucket type to NORMAL.
*/
HashTableBucket::HashTableBucket(string key, int value) {
    this->key = key;
    this->value = value;
    this->type = BucketType::NORMAL;
}

/**
* A load method could load the key-value pair into the bucket, which
* should then also mark the bucket as NORMAL.
*/
void HashTableBucket::load(string key, int value) {
    this->key = key;
    this->value = value;
    this->type = BucketType::NORMAL;
}

/**
* This method would return whether the bucket is empty, regardless of
* if it has had data placed in it or not.
*/
bool HashTableBucket::isEmpty() const {
    if (type != BucketType::NORMAL) {
        return true;
    } else {
        return false;
    }
}

/**
* The stream insertion operator could be overloaded to print the
* bucket's contents. Or if preferred, you could write a print method
* instead.
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
* Only a single constructor that takes an initial capacity for the table is
* necessary. If no capacity is given, it defaults to 8 initially
*/
HashTable::HashTable(size_t initCapacity) {
    tableData.resize(initCapacity);

    currentSize = 0;
    deletedCount = 0;
    generateOffsets(initCapacity, 0);
}

/**
* Insert a new key-value pair into the table. Duplicate keys are NOT allowed. The
* method should return true if the insertion was successful. If the insertion was
* unsucessful, such as when a duplicate is attempted to be inserted, the method
* should return false
*/
bool HashTable::insert(string key, size_t value) {
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
* If the key is in the table, remove will “erase” the key-value pair from the
* table. This might just be marking a bucket as empty-after-remove
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
* contains returns true if the key is in the table and false if the key is not in
* the table.
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
* If the key is found in the table, find will return the value associated with
* that key. If the key is not in the table, find will return something called
* nullopt, which is a special value in C++. The find method returns an
* optional<int>, which is a way to denote a method might not have a valid value
* to return. This approach is nicer than designating a special value, like -1, to
* signify the return value is invalid. It's also much better than throwing an
* exception if the key is not found.
*/
optional<int> HashTable::get(const string &key) const {
}

/**
* The bracket operator lets us access values in the map using a familiar syntax,
* similar to C++ std::map or Python dictionaries. It behaves like get, returnin
* the value associated with a given key:
 int idNum = hashTable[“James”];
* Unlike get, however, the bracker operator returns a reference to the value,
* which allows assignment:
 hashTable[“James”] = 1234;
 If the key is not
* in the table, returning a valid reference is impossible. You may choose to
* throw an exception in this case, but for our implementation, the situation
* results in undefined behavior. Simply put, you do not need to address attempts
* to access keys not in the table inside the bracket operator method.
*/
int &HashTable::operator[](const string &key) {
}

/**
* keys returns a std::vector (C++ version of ArrayList, or simply list/array)
* with all of the keys currently in the table. The length of the vector should be
* the same as the size of the hash table.
*/
vector<string> HashTable::keys() const {
}

/**
* alpha returns the current load factor of the table, or size/capacity. Since
* alpha returns a double,make sure to properly cast the size and capacity, which
* are size_t, to avoid integer division. You can cast a size_t num to a double in
* C++ like:
 static_cast<double>(num)
 The time complexity for
* this method must be O(1).
*/
double HashTable::alpha() const {
    return static_cast<double>(currentSize) / static_cast<double>(capacity());
}

/**
* capacity returns how many buckets in total are in the hash table. The time
* complexity for this algorithm must be O(1).
*/
size_t HashTable::capacity() const {
    return tableData.size();
}

/**
* The size method returns how many key-value pairs are in the hash table. The
* time complexity for this method must be O(1)
*/
size_t HashTable::size() const {
    return currentSize;
}

/**
* operator<< is another example of operator overloading in C++, similar to
* operator[]. The friend keyword only needs to appear in the class declaration,
* but not the definition. In addition, operator<< is not a method of HashTable,
* so do not put HashTable:: before it when defining it. operator<< will allow us
* to print the contents of our hash table using the normal syntax:
cout <<
* myHashTable << endl;
You should only print the buckets which are occupied,
* and along with each item you will print which bucket (the index of the bucket)
* the item is in. To make it easy, I suggest creating a helper method called
* something like printMe() that returns a string of everything in the table. An
* example which uses open addressing for collision resolution could print
* something like:
Bucket 5: <James, 4815>
Bucket 2: <Juliet, 1623>
Bucket
* 11: <Hugo, 42108>
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

size_t HashTable::hashIndex(const string &key) const {
    return hash<string>{}(key) % capacity();
}

