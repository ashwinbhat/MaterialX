//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

/**
* Copyright 2019 Autodesk, Inc.
* All rights reserved.
*
* This computer source code and related instructions and comments are the unpublished confidential
* and proprietary information of Autodesk, Inc. and are protected under Federal copyright and state
* trade secret law. They may not be disclosed to, copied or used by any third party without the
* prior written consent of Autodesk, Inc.
*
* @file Units.cpp
* @brief Units implementation.
*/

#include <MaterialXCore/Units.h>

#include <MaterialXCore/Util.h>
#include <MaterialXCore/Value.h>

namespace MaterialX
{

    namespace
    {

        // Matching Units::EUnit.
        const std::string kUnitNames[] = {
            std::string(""),    std::string("mm"),    std::string("cm"),
            std::string("m"),   std::string("km"),    std::string("\""),
            std::string("ft"),  std::string("mi"),    std::string("deg"),
            std::string("rad"), std::string("cd/m^2") };

        double unitScale(Units::EUnit unit) {
            switch (unit) {
                // Length units are converted to millimeters.
            case Units::E_UNIT_MM: {
                return 1.0;
            }
            case Units::E_UNIT_CM: {
                return 10.0;
            }
            case Units::E_UNIT_M: {
                return 1000.0;
            }
            case Units::E_UNIT_KM: {
                return 1000000.0;
            }
            case Units::E_UNIT_INCH: {
                return 25.4;
            }
            case Units::E_UNIT_FOOT: {
                return 304.8;
            }
            case Units::E_UNIT_MILE: {
                return 1609344.0;
            }

                                     // Angle units are converted to radians.
            case Units::E_UNIT_RADIAN: {
                return 1.0;
            }
            case Units::E_UNIT_DEGREE: {
                return 0.01745329251994329576923690768488612713443;
            }

                                       // Luminance units are converted to candela per m^2.
            case Units::E_UNIT_CD_PER_M2: {
                return 1.0;
            }

                                          // Unitless.
            default: { return 1.0; }
            }
        }

        double unitRatio(Units::EUnit from, Units::EUnit to) {
            if (from == Units::E_UNIT_DIMENSIONLESS || to == Units::E_UNIT_DIMENSIONLESS || from == to) {
                return 1.0;
            }

            return unitScale(from) / unitScale(to);
        }

    }  // anonymous namespace

    float Units::unitToCentimeterScale(const std::string& unit) {
        // Metric SI units.
        if (unit == "yottameter") {
            return 1.0e26f;
        }
        if (unit == "zettameter") {
            return 1.0e23f;
        }
        if (unit == "exameter") {
            return 1.0e20f;
        }
        if (unit == "petameter") {
            return 1.0e17f;
        }
        if (unit == "terameter") {
            return 1.0e14f;
        }
        if (unit == "gigameter") {
            return 1.0e11f;
        }
        if (unit == "megameter") {
            return 1.0e8f;
        }
        if (unit == "kilometer" || unit == "km") {
            return 1.0e5f;
        }
        if (unit == "hectometer") {
            return 1.0e4f;
        }
        if (unit == "decameter") {
            return 1.0e3f;
        }
        if (unit == "meter" || unit == "m") {
            return 1.0e2f;
        }
        if (unit == "decimeter") {
            return 1.0e1f;
        }
        if (unit == "centimeter" || unit == "cm") {
            return 1.0f;
        }
        if (unit == "millimeter" || unit == "mm") {
            return 1.0e-1f;
        }
        if (unit == "micrometer") {
            return 1.0e-4f;
        }
        if (unit == "nanometer") {
            return 1.0e-7f;
        }
        if (unit == "picometer") {
            return 1.0e-10f;
        }
        if (unit == "femtometer") {
            return 1.0e-13f;
        }
        if (unit == "attometer") {
            return 1.0e-16f;
        }
        if (unit == "zeptometer") {
            return 1.0e-19f;
        }
        if (unit == "yoctometer") {
            return 1.0e-22f;
        }

        // Metric, non-SI.
        if (unit == "mil" || unit == "mil_swedennorway") {
            return 1.0e6f;
        }
        if (unit == "angstrom") {
            return 1.0e-8f;
        }
        if (unit == "micron") {
            return 1.0e-4f;
        }

        // Imperial units.
        if (unit == "microinch") {
            return 2.54e-6f;
        }
        if (unit == "inch" || unit == "in") {
            return 2.54f;
        }
        if (unit == "foot" || unit == "ft") {
            return 30.48f;
        }
        if (unit == "yard") {
            return 91.44f;
        }
        if (unit == "mile") {
            return 160934.4f;
        }

        // Astronomical units.
        if (unit == "lightyear") {
            return 9.4605284e17f;
        }
        if (unit == "parsec") {
            return 3.08567758e18f;
        }
        if (unit == "astronomicalunit") {
            return 1.49597871e13f;
        }

        // WFT units!
        if (unit == "hand") {
            return 10.16f;
        }
        if (unit == "span" || unit == "span_cloth") {
            return 22.86f;
        }
        if (unit == "fathom") {
            return 182.88f;
        }
        if (unit == "rope") {
            return 609.6f;
        }
        if (unit == "chain") {
            return 2011.68f;
        }
        if (unit == "furlong") {
            return 20116.8f;
        }

        // No dimension.
        if (unit == "dimensionless") {
            return 1.0f;
        }

        // Unsupported unit.
        throw ExceptionTypeError("Unsupported unit:: " + unit);
    }

    const std::string& Units::unitName(EUnit unit) {
        return kUnitNames[unit];
    }

    Units::EUnit Units::toUnit(const std::string& str) {
        if (str == "mm" || str == "millimeter") {
            return E_UNIT_MM;
        }
        if (str == "cm" || str == "centimeter") {
            return E_UNIT_CM;
        }
        if (str == "m" || str == "meter") {
            return E_UNIT_M;
        }
        if (str == "km" || str == "kilometer") {
            return E_UNIT_KM;
        }

        if (str == "in" || str == "inch") {
            return E_UNIT_INCH;
        }
        if (str == "ft" || str == "foot") {
            return E_UNIT_FOOT;
        }
        if (str == "mi" || str == "mile") {
            return E_UNIT_MILE;
        }

        if (str == "deg" || str == "degree") {
            return E_UNIT_DEGREE;
        }
        if (str == "rad" || str == "radian") {
            return E_UNIT_RADIAN;
        }

        if (str == "cd/m^2") {
            return E_UNIT_CD_PER_M2;
        }

        if (str == "") {
            return E_UNIT_DIMENSIONLESS;
        }

        throw ExceptionTypeError("Unrecognized property unit: " +  str);
    }

    double Units::convertUnit(double v, EUnit from, EUnit to) {
        return v * unitRatio(from, to);
    }

    Vector3 Units::convertUnit(Vector3 v, Units::EUnit from, Units::EUnit to) {
        double ratio = unitRatio(from, to);
        for (int i = 0; i < 3; ++i) {
            v[i] = static_cast<float>(ratio * v[i]);
        }
        return v;
    }

    void Units::convertUnit(std::vector<float>* v, EUnit from, EUnit to) {
        double ratio = unitRatio(from, to);
        for (float& f : *v) {
            f = static_cast<float>(ratio * f);
        }
    }
}