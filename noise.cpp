#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "noise/noise1d.hpp"

PYBIND11_MODULE(noise, m)
{
  py::class_<Noise1D>(m, "Noise1D")
      .def(py::init<std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, double, int, double, double>())
      .def_property_readonly("seed", [](Noise1D const &n) { return n.seed(); })
      .def_property_readonly("x", [](Noise1D const &n) { return n.x(); })
      .def_property_readonly("data", [](Noise1D const &n) { return std::vector<double>(n.data(), n.data() + n.x()); })
      .def("__getitem__", [](Noise1D const &n, std::size_t idx) { return n(idx); });
}
