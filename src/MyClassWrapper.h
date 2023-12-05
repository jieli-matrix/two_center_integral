#include <iostream>
#include <cstring>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;
const int MAX_SIZE = 100;
class MyClassWrapper{
private:
    int data[MAX_SIZE];
    int size;

public:
    MyClassWrapper() : size(0) {}
    void setData_py(py::array_t<int, py::array::c_style> arr, int arrSize){
        int *arr_c = static_cast<int *>(arr.request().ptr);
        size = (arrSize > MAX_SIZE) ? MAX_SIZE : arrSize;
        std::memcpy(data, arr_c, size * sizeof(int)); 
    }
    py::array_t<int> displayData_py(){
        py::array_t<int, py::array::c_style> arr(size, data);
        return arr;
    }
};