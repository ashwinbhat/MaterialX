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

#ifndef MATERIALX_UNITS_H_
#define MATERIALX_UNITS_H_

#include <MaterialXCore/Library.h>

#include <MaterialXCore/Types.h>

namespace MaterialX
{

    class Units {
    public:
        /// @brief The units of a property.
        enum EUnit {
            E_UNIT_DIMENSIONLESS,  ///< No unit (dimensionless)
            E_UNIT_MM,             ///< millimeter
            E_UNIT_CM,             ///< centimeter
            E_UNIT_M,              ///< meter
            E_UNIT_KM,             ///< kilometer
            E_UNIT_INCH,           ///< inch
            E_UNIT_FOOT,           ///< foot
            E_UNIT_MILE,           ///< mile
            E_UNIT_DEGREE,         ///< degree
            E_UNIT_RADIAN,         ///< radian
            E_UNIT_CD_PER_M2       ///< cd/m^2
        };

        /// @brief Convert a unit string to a centimeter scale.
        /// @param unit The unit string.
        /// @returns the scaling factor for converting the given unit to centimeters.
        static float unitToCentimeterScale(const std::string& unit);

        /// @brief Convert a unit to unit name string.
        /// @param unit The unit.
        /// @returns the unit name string.
        static const std::string& unitName(EUnit unit);

        /// @brief Convert a string to a EUnit enum.
        /// @param unit The string to convert.
        /// @returns the unit, or E_UNIT_DIMENSIONLESS if the string did not represent a known unit.
        static Units::EUnit toUnit(const char* unit) { return toUnit(std::string(unit)); }

        /// @brief Convert a string to a EUnit enum.
        /// @param unit The string to convert.
        /// @returns the unit, or E_UNIT_DIMENSIONLESS if the string did not represent a known unit.
        static Units::EUnit toUnit(const std::string& unit);

        /// @brief Convert a value from one unit to another unit.
        /// @param v The value to convert.
        /// @param from The unit to convert from.
        /// @param to The unit to convert to.
        /// @returns the converted value.
        static double convertUnit(double v, Units::EUnit from, Units::EUnit to);

        /// @brief Convert a value from one unit to another unit.
        /// @param v The value to convert.
        /// @param from The unit to convert from.
        /// @param to The unit to convert to.
        /// @returns the converted value.
        static Vector3 convertUnit(Vector3 v, Units::EUnit from, Units::EUnit to);

        /// @brief Convert an array of values from one unit to another unit.
        /// @param[in,out] v The value array to convert.
        /// @param from The unit to convert from.
        /// @param to The unit to convert to.
        static void convertUnit(std::vector<float>* v, Units::EUnit from, Units::EUnit to);
    };

}  // namespace MaterialX

#endif  // MATERIALX_UNITS_H_
