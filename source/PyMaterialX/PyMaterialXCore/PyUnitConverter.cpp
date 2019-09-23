//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <PyMaterialX/PyMaterialX.h>

#include <MaterialXCore/UnitConverter.h>

namespace py = pybind11;
namespace mx = MaterialX;

class PyUnitConverter : public mx::UnitConverter 
{
  public:
    PyUnitConverter()
    {
    }

    float convert(float input, const std::string& inputUnit, const std::string& outputUnit) const override
    {
        PYBIND11_OVERLOAD_PURE(
            float, 
            mx::UnitConverter,
            convert,
            input,
            inputUnit,
            outputUnit
        );
    }
};

void bindPyUnitConverters(py::module& mod)
{
    py::class_<mx::UnitConverter, PyUnitConverter, mx::UnitConverterPtr>(mod, "UnitConverter")
        .def(py::init<>())
        .def("convert", &mx::UnitConverter::convert);

    py::class_<mx::LengthUnitConverter, mx::UnitConverter>(mod, "LengthUnitConverter")
        .def_static("create", &mx::LengthUnitConverter::create)
        .def("convert", &mx::LengthUnitConverter::convert);
}