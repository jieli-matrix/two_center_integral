#include "math_sphbes.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
using namespace pybind11::literals;
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

PYBIND11_MODULE(_core, m) {
  m.def("sphbesj", overload_cast_<const int, const double>()(&sphbesj), "l"_a,
        "x"_a)
      .def("dsphbesj", overload_cast_<const int, const double>()(&dsphbesj),
           "l"_a, "x"_a)
      .def(
          "sphbesj",
          [](const int n, py::array_t<double> r, const double q, const int l,
             py::array_t<double> jl) {
            py::buffer_info r_info = r.request();
            if (r_info.ndim != 1) {
              throw std::runtime_error("r array must be 1-dimensional");
            }
            py::buffer_info jl_info = jl.request();
            if (jl_info.ndim != 1) {
              throw std::runtime_error("jl array must be 1-dimensional");
            }
            sphbesj(n, static_cast<const double *const>(r_info.ptr), q, l,
                    static_cast<double *const>(jl_info.ptr));
          },
          "n"_a, "r"_a, "q"_a, "l"_a, "jl"_a)
      .def(
          "dsphbesj",
          [](const int n, py::array_t<double> r, const double q, const int l,
             py::array_t<double> djl) {
            py::buffer_info r_info = r.request();
            if (r_info.ndim != 1) {
              throw std::runtime_error("r array must be 1-dimensional");
            }
            py::buffer_info djl_info = djl.request();
            if (djl_info.ndim != 1) {
              throw std::runtime_error("djl array must be 1-dimensional");
            }
            dsphbesj(n, static_cast<const double *const>(r_info.ptr), q, l,
                     static_cast<double *const>(djl_info.ptr));
          },
          "n"_a, "r"_a, "q"_a, "l"_a, "djl"_a);
#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
