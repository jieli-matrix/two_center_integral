#include "MyClass.h"
#include <iostream>
#include <cstring>

// default constructor
MyClass::MyClass() : size(0) {}

// copy constructor
MyClass::MyClass(const MyClass &other) : size(other.size) {
    std::memcpy(data, other.data, size * sizeof(int));
}


// Setter 
void MyClass::setData(const int arr[], int arrSize) {
    size = (arrSize > MAX_SIZE) ? MAX_SIZE : arrSize;
    std::memcpy(data, arr, size * sizeof(int));
}

// deep copy assignment operator
MyClass& MyClass::operator=(const MyClass &other) {
    if (this != &other) {
        size = other.size;
        std::memcpy(data, other.data, size * sizeof(int));
    }
    return *this;
}

// operator overloading
MyClass MyClass::operator+(const MyClass &other) const {
    MyClass result;
    int maxSize = (size > other.size) ? size : other.size;

    for (int i = 0; i < maxSize; ++i) {
        result.data[i] = data[i % size] + other.data[i % other.size];
        result.size++;
    }

    return result;
}

// overloaded function
const int* MyClass::displayData() const {
    return data;
}

int MyClass::displayData(int index) const {
    return data[index];
}
