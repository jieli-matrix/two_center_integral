#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "MyClass.h"
#include "MyClassWrapper.h"
using namespace pybind11::literals;
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;


#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    py::class_<MyClassWrapper>(m, "MyClassWrapper")
        .def(py::init<>())
        .def("setData", &MyClassWrapper::setData_py)
        .def("displayData", &MyClassWrapper::displayData_py);
        //TODO: copy constructor
        //.def("setData", &MyClassWrapper::setData_py);
        //.def("displayData", overload_cast_<int>()(&MyClass::displayData, py::const_));
//.def("displayData", overload_cast_<>()(&MyClassWrapper::displayData_py));



#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
