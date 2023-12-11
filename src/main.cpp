#include <pybind11/pybind11.h>
#include "testm.h"
#include<pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <vector>
using namespace pybind11::literals;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
PYBIND11_MAKE_OPAQUE(std::vector<double>);

PYBIND11_MODULE(_core, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: two_center_integral

        .. autosummary::
           :toctree: _generate

           sphbesj
           dsphbesj
    )pbdoc";
    py::bind_vector<std::vector<double>>(m, "VectorDouble");
    m.def("test_modify", [](double e, std::vector<double>& v){test_modify(e, v); return std::make_tuple(e, v);});


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
