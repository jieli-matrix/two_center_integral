// TODO: add both stl and buffer_pool
#ifndef MYCLASS_H
#define MYCLASS_H
#include <iostream>
#include <cstring>

const int MAX_SIZE = 100;
class MyClass {
private:
    int data[MAX_SIZE];
    int size;

public:
    MyClass();
    MyClass(const MyClass &other);
    void setData(const int arr[], int arrSize);
    MyClass& operator=(const MyClass &other);
    MyClass operator+(const MyClass &other) const;
    const int* displayData() const;
    int MyClass::displayData(int index) const;
};

#endif // MYCLASS_H
