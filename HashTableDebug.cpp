/**
 * HashTableDebug.cpp
 *
 * Write your tests in this file
 */
#include <iostream>
#include "HashTable.h"

int main() {
    HashTable ht(8);
    cout << "Capacity: " << ht.capacity() << endl;
    cout << "Size: " << ht.size() << endl;
    cout << "Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    cout << ht.insert("A", 1)<< endl;
    cout << ht.insert("B", 2)<< endl;
    cout << ht.insert("D", 1)<< endl;
    cout << ht.insert("A", 3)<< endl;

    cout << ht.contains("A")<< endl;
    cout << ht.contains("B")<< endl;
    cout << ht.contains("C")<< endl;

    cout << ht<< endl;

    cout << "Capacity: " << ht.capacity() << endl;
    cout << "Size: " << ht.size() << endl;
    cout << "Alpha: " << ht.alpha() << endl;

    cout << ht.remove("A")<< endl;

    cout << ht<< endl;

    cout << "Capacity: " << ht.capacity() << endl;
    cout << "Size: " << ht.size() << endl;
    cout << "Alpha: " << ht.alpha() << endl;

    cout << ht.remove("D")<< endl;

    cout << ht<< endl;

    cout << "Capacity: " << ht.capacity() << endl;
    cout << "Size: " << ht.size() << endl;
    cout << "Alpha: " << ht.alpha() << endl;

}