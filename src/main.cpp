#include <pybind11/pybind11.h>
#include "math_sphbes.h"
using namespace pybind11::literals;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

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

    m.def("sphbesj", &sphbesj, "l"_a, "x"_a);
    m.def("dsphbesj", &dsphbesj, "l"_a, "x"_a);


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
