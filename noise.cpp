#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "noise/noise1d.hpp"
#include "noise/noise2d.hpp"
#include "noise/noise3d.hpp"

PYBIND11_MODULE(noise, m)
{
  py::class_<Noise1D>(m, "Noise1D")
      .def(py::init<std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, double, int, double, double>())
      .def_property_readonly("seed", [](Noise1D const &n) { return n.seed(); })
      .def_property_readonly("x", [](Noise1D const &n) { return n.x(); })
      .def_property_readonly("data", [](Noise1D const &n) { return std::vector<double>(n.data(), n.data() + n.x()); })
      .def("__getitem__", [](Noise1D const &n, std::size_t idx) { return n(idx); });

  py::class_<Noise2D>(m, "Noise2D")
      .def(py::init<std::size_t, std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::size_t, double, int, double, double>())
      .def_property_readonly("seed", [](Noise2D const &n) { return n.seed(); })
      .def_property_readonly("x", [](Noise2D const &n) { return n.x(); })
      .def_property_readonly("y", [](Noise2D const &n) { return n.y(); })
      .def_property_readonly("data",
                             [](Noise2D const &n) { return std::vector<double>(n.data(), n.data() + n.x() * n.y()); })
      .def("__getitem__", [](Noise2D const &n, std::vector<std::size_t> idx) { return n(idx[0], idx[1]); });

  py::class_<Noise3D>(m, "Noise3D")
      .def(py::init<std::size_t, std::size_t, std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::size_t, std::size_t, double, int, double, double>())
      .def_property_readonly("seed", [](Noise3D const &n) { return n.seed(); })
      .def_property_readonly("x", [](Noise3D const &n) { return n.x(); })
      .def_property_readonly("y", [](Noise3D const &n) { return n.y(); })
      .def_property_readonly("z", [](Noise3D const &n) { return n.z(); })
      //   .def_property_readonly("data",
      //                          [](Noise3D const &n) { return std::vector<double>(n.data(), n.data() + n.x() * n.y());
      //                          })
      .def("__getitem__", [](Noise3D const &n, std::vector<std::size_t> idx) { return n(idx[0], idx[1], idx[2]); });
}
