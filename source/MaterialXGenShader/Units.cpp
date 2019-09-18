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
        const std::string kDistanceUnitNames[] = {
            std::string(""),   
            std::string("millimeter"),    
            std::string("centimeter"),
            std::string("meter"),
            std::string("kilometer"), 
            std::string("inch"),
            std::string("foot"),
            std::string("mile")     
        };

        double unitScale(Units::DistanceUnit unit) {
            switch (unit) {
                // Length units are converted to millimeters.
            case Units::UNITTYPE_MM: {
                return 1.0;
            }
            case Units::UNITTYPE_CM: {
                return 10.0;
            }
            case Units::UNITTYPE_M: {
                return 1000.0;
            }
            case Units::UNITTYPE_KM: {
                return 1000000.0;
            }
            case Units::UNITTYPE_INCH: {
                return 25.4;
            }
            case Units::UNITTYPE_FOOT: {
                return 304.8;
            }
            case Units::UNITTYPE_MILE: {
                return 1609344.0;
            }

            // Unitless.
            default: { return 1.0; }
            }
        }

        double unitRatio(Units::DistanceUnit from, Units::DistanceUnit to) {
            if (from == Units::UNITTYPE_UNKNOWN || to == Units::UNITTYPE_UNKNOWN || from == to) {
                return 1.0;
            }

            return unitScale(from) / unitScale(to);
        }

    }  // anonymous namespace

    
    const std::string& Units::unitName(DistanceUnit unit) {
        return kDistanceUnitNames[unit];
    }

    Units::DistanceUnit Units::toUnit(const std::string& str) {
        if (str == "mm" || str == "millimeter") {
            return UNITTYPE_MM;
        }
        if (str == "cm" || str == "centimeter") {
            return UNITTYPE_CM;
        }
        if (str == "m" || str == "meter") {
            return UNITTYPE_M;
        }
        if (str == "km" || str == "kilometer") {
            return UNITTYPE_KM;
        }
        if (str == "in" || str == "inch") {
            return UNITTYPE_INCH;
        }
        if (str == "ft" || str == "foot") {
            return UNITTYPE_FOOT;
        }
        if (str == "mi" || str == "mile") {
            return UNITTYPE_MILE;
        }
        if (str == "") {
            return UNITTYPE_UNKNOWN;
        }

        throw ExceptionTypeError("Unrecognized property unit: " +  str);
    }

    double Units::convertUnit(double v, DistanceUnit from, DistanceUnit to) {
        return v * unitRatio(from, to);
    }

    Vector3 Units::convertUnit(Vector3 v, Units::DistanceUnit from, Units::DistanceUnit to) {
        double ratio = unitRatio(from, to);
        for (int i = 0; i < 3; ++i) {
            v[i] = static_cast<float>(ratio * v[i]);
        }
        return v;
    }

    void Units::convertUnit(std::vector<float>* v, DistanceUnit from, DistanceUnit to) {
        double ratio = unitRatio(from, to);
        for (float& f : *v) {
            f = static_cast<float>(ratio * f);
        }
    }
}