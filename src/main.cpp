#include <pybind11/pybind11.h>
#include "numerical_radial.h"
using namespace pybind11::literals;
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    // enable dynamic attributes
    py::class_<NumericalRadial> nr(m, "NumericalRadial"); 
    // constructors
    nr.def(py::init<>()) 
      .def(py::init<NumericalRadial const&>())
      // methods
      .def("build", &NumericalRadial::build, "l"_a, "for_r_space"_a, "ngrid"_a, "grid"_a, "value"_a, "p"_a = 0, "izeta"_a = 0, "symbol"_a = "", "itype"_a = 0, "init_sbt"_a = true)
      .def("set_grid", &NumericalRadial::set_grid, "for_r_space"_a, "ngrid"_a, "grid"_a, "mode"_a = 'i')
      .def("set_uniform_grid", &NumericalRadial::set_uniform_grid, "for_r_space"_a, "ngrid"_a, "cutoff"_a, "mode"_a = 'i', "enable_fft"_a = false)
      .def("set_value", &NumericalRadial::set_value, "for_r_space"_a, "value"_a, "p"_a)
      .def("wipe", &NumericalRadial::wipe, "r_space"_a = true, "k_space"_a = true)
      // getter properties
      .def_property_readonly("symbol", &NumericalRadial::symbol)
      .def_property_readonly("itype", &NumericalRadial::itype)
      .def_property_readonly("izeta", &NumericalRadial::izeta)
      .def_property_readonly("l", &NumericalRadial::l)
      .def_property_readonly("nr", &NumericalRadial::nr)
      .def_property_readonly("nk", &NumericalRadial::nk)
      .def_property_readonly("rcut", &NumericalRadial::rcut)
      .def_property_readonly("kcut", &NumericalRadial::kcut)
      .def_property_readonly("pr", &NumericalRadial::pr)
      .def_property_readonly("pk", &NumericalRadial::pk)
      // overloaded methods const
      .def("rgrid", py::overload_cast<>(&NumericalRadial::rgrid, py::const_))
      .def("rgrid", py::overload_cast<int>(&NumericalRadial::rgrid, py::const_))
      .def("kgrid", py::overload_cast<>(&NumericalRadial::kgrid, py::const_))
      .def("kgrid", py::overload_cast<int>(&NumericalRadial::kgrid, py::const_))
      .def("rvalue", py::overload_cast<>(&NumericalRadial::rvalue, py::const_))
      .def("rvalue", py::overload_cast<int>(&NumericalRadial::rvalue, py::const_))
      .def("kvalue", py::overload_cast<>(&NumericalRadial::kvalue, py::const_))
      .def("kvalue", py::overload_cast<int>(&NumericalRadial::kvalue, py::const_))
      .def_property_readonly("is_fft_compliant", py::overload_cast<>(&NumericalRadial::is_fft_compliant, py::const_));
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: two_center_integral

        .. autosummary::
           :toctree: _generate

           NumericalRadial
    )pbdoc";    


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}