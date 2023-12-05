#include <iostream>
#include <cstring>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include "MyClass.h"
namespace py = pybind11;

class MyClassWrapper : public MyClass{
public:
    void setData(py::array_t<int, py::array::c_style> arr, int arrSize){
        int *arr_c = static_cast<int *>(arr.request().ptr);
        MyClass::setData(arr_c, arrSize); 
    }
    py::array_t<int> displayData(){
        py::array_t<int, py::array::c_style> arr(this->size, this->data);
        return arr;
    }
    int displayData(int index) const{
        return MyClass::displayData(index);
    }
};