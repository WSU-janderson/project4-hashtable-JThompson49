/**
 * HashTableDebug.cpp
 *
 * Write your tests in this file
 */
#include <iostream>
#include "HashTable.h"

int main() {
    HashTable ht(8);
    std::cout << "Capacity: " << ht.capacity() << "\n";
    std::cout << "Size: " << ht.size() << "\n";
    std::cout << "Alpha: " << ht.alpha() << "\n";
    std::cout << ht << std::endl;

}