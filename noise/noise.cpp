#include <array>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "noise1d.hpp"
#include "noise2d.hpp"
#include "noise3d.hpp"

PYBIND11_MODULE(noise, m)
{
  py::class_<Noise1D>(m, "Noise1D")
      .def(py::init<std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::size_t, std::size_t, double, double>())
      .def_property_readonly("seed", [](Noise1D const &n) { return n.seed(); })
      .def_property_readonly("xsz", [](Noise1D const &n) { return n.xsz(); })
      .def_property_readonly("scale", [](Noise1D const &n) { return n.scale(); })
      .def_property_readonly("octaves", [](Noise1D const &n) { return n.octaves(); })
      .def_property_readonly("lacunarity", [](Noise1D const &n) { return n.lacunarity(); })
      .def_property_readonly("persistance", [](Noise1D const &n) { return n.persistance(); })
      .def_property_readonly("data", [](Noise1D const &n) { return n.data(); })
      .def("__getitem__", [](Noise1D const &n, std::size_t idx) { return n(idx); });

  py::class_<Noise2D>(m, "Noise2D")
      .def(py::init<std::size_t, std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, double, double>())
      .def_property_readonly("seed", [](Noise2D const &n) { return n.seed(); })
      .def_property_readonly("xsz", [](Noise2D const &n) { return n.xsz(); })
      .def_property_readonly("ysz", [](Noise2D const &n) { return n.ysz(); })
      .def_property_readonly("scale", [](Noise2D const &n) { return n.scale(); })
      .def_property_readonly("octaves", [](Noise2D const &n) { return n.octaves(); })
      .def_property_readonly("lacunarity", [](Noise2D const &n) { return n.lacunarity(); })
      .def_property_readonly("persistance", [](Noise2D const &n) { return n.persistance(); })
      .def_property_readonly("data", [](Noise2D const &n) { return n.data(); })
      .def("__getitem__", [](Noise2D const &n, std::array<std::size_t, 2> idx) { return n(idx.at(0), idx.at(1)); });

  py::class_<Noise3D>(m, "Noise3D")
      .def(py::init<std::size_t, std::size_t, std::size_t, std::size_t>())
      .def(py::init<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, double, double>())
      .def_property_readonly("seed", [](Noise3D const &n) { return n.seed(); })
      .def_property_readonly("xsz", [](Noise3D const &n) { return n.xsz(); })
      .def_property_readonly("ysz", [](Noise3D const &n) { return n.ysz(); })
      .def_property_readonly("zsz", [](Noise3D const &n) { return n.zsz(); })
      .def_property_readonly("scale", [](Noise3D const &n) { return n.scale(); })
      .def_property_readonly("octaves", [](Noise3D const &n) { return n.octaves(); })
      .def_property_readonly("lacunarity", [](Noise3D const &n) { return n.lacunarity(); })
      .def_property_readonly("persistance", [](Noise3D const &n) { return n.persistance(); })
      .def_property_readonly("data", [](Noise3D const &n) { return n.data(); })
      .def("__getitem__", [](Noise3D const &n, std::array<std::size_t, 3> idx) { return n(idx.at(0), idx.at(1), idx.at(2)); });
}
