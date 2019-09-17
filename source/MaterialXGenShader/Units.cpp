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

#include <MaterialXGenShader/Units.h>

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