//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

/**
 * Copyright 2019 Autodesk, Inc.
 * All rights reserved.
 *
 * This computer source code and related instructions and comments are the
 * unpublished confidential and proprietary information of Autodesk, Inc. and
 * are protected under Federal copyright and state trade secret law. They may
 * not be disclosed to, copied or used by any third party without the prior
 * written consent of Autodesk, Inc.
 *
 * @file Units.h
 * @brief Units declaration.
 */

#ifndef MATERIALX_SHADER_UNITS_H_
#define MATERIALX_SHADER_UNITS_H_

#include <MaterialXCore/Library.h>

#include <MaterialXCore/Types.h>

namespace MaterialX
{

    class Units {
    public:
        /// @brief The units of a property.
        enum DistanceUnit {
            UNITTYPE_UNKNOWN,  ///< No unit (dimensionless)
            UNITTYPE_MM,             ///< millimeter
            UNITTYPE_CM,             ///< centimeter
            UNITTYPE_M,              ///< meter
            UNITTYPE_KM,             ///< kilometer
            UNITTYPE_INCH,           ///< inch
            UNITTYPE_FOOT,           ///< foot
            UNITTYPE_MILE,           ///< mile
        };


        /// @brief Convert a unit to unit name string.
        /// @param unit The unit.
        /// @returns the unit name string.
        static const std::string& unitName(DistanceUnit unit);

        /// @brief Convert a string to a EUnit enum.
        /// @param unit The string to convert.
        /// @returns the unit, or E_UNIT_DIMENSIONLESS if the string did not represent a known unit.
        static Units::DistanceUnit toUnit(const char* unit) { return toUnit(std::string(unit)); }

        /// @brief Convert a string to a EUnit enum.
        /// @param unit The string to convert.
        /// @returns the unit, or E_UNIT_DIMENSIONLESS if the string did not represent a known unit.
        static Units::DistanceUnit toUnit(const std::string& unit);

        /// @brief Convert a value from one unit to another unit.
        /// @param v The value to convert.
        /// @param from The unit to convert from.
        /// @param to The unit to convert to.
        /// @returns the converted value.
        static double convertUnit(double v, Units::DistanceUnit from, Units::DistanceUnit to);

        /// @brief Convert a value from one unit to another unit.
        /// @param v The value to convert.
        /// @param from The unit to convert from.
        /// @param to The unit to convert to.
        /// @returns the converted value.
        static Vector3 convertUnit(Vector3 v, Units::DistanceUnit from, Units::DistanceUnit to);

        /// @brief Convert an array of values from one unit to another unit.
        /// @param[in,out] v The value array to convert.
        /// @param from The unit to convert from.
        /// @param to The unit to convert to.
        static void convertUnit(std::vector<float>* v, Units::DistanceUnit from, Units::DistanceUnit to);
    };

}  // namespace MaterialX

#endif  // MATERIALX_SHADER_UNITS_H_
