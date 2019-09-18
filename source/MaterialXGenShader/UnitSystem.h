//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_UNIT_SYSTEM_H
#define MATERIALX_UNIT_SYSTEM_H

/// @file
/// Unit  system classes

#include <MaterialXGenShader/Library.h>

#include <MaterialXGenShader/ShaderNode.h>
#include <MaterialXGenShader/ShaderNodeImpl.h>
#include <MaterialXGenShader/TypeDesc.h>
#include <MaterialXGenShader/Units.h>

#include <MaterialXCore/Document.h>

namespace MaterialX
{

class ShaderGenerator;

/// A shared pointer to a UnitSystem
using UnitSystemPtr = shared_ptr<class UnitSystem>;

/// @struct @UnitTransform
/// Structure that represents unit transform information
struct UnitTransform
{
    UnitTransform(const string& ss, const string& ts, const TypeDesc* t);

    string sourceUnit;
    string targetUnit;
    const TypeDesc* type;

    /// Comparison operator
    bool operator==(const UnitTransform &other) const
    {
        return sourceUnit == other.sourceUnit &&
            targetUnit == other.targetUnit &&
            type == other.type;
    }

    string measurementType() const
    {
        Units::DistanceUnit sourceType = Units::toUnit(sourceUnit);
        if ((Units::UNITTYPE_MM <= sourceType && sourceType <= Units::UNITTYPE_M))
            return "distance_unit";
        else
            return "unknown_unit";
    }
    float encodeSourceUnit() const
    {
        return ((float)Units::toUnit(sourceUnit));
    }

    float encodeTargetUnit() const
    {
        return ((float)Units::toUnit(targetUnit));
    }
};

/// @class UnitSystem
/// Abstract base class for a UnitSystem
///
class UnitSystem
{
public:
    virtual ~UnitSystem() { }

    /// Return the UnitSystem name
    virtual const string& getName() const = 0;

    /// Load a library of implementations from the provided document,
    /// replacing any previously loaded content.
    virtual void loadLibrary(DocumentPtr document);

    /// Returns whether this unit system supports a provided transform
    bool supportsTransform(const UnitTransform& transform) const;

    /// Create a node to use to perform the given unit space transformation.
    ShaderNodePtr createNode(const ShaderGraph* parent, const UnitTransform& transform, const string& name,
        GenContext& context) const;

protected:
    /// Protected constructor
    UnitSystem();

    /// Returns an implementation name for a given transform
    virtual string getImplementationName(const UnitTransform& transform) const = 0;

protected:
    DocumentPtr _document;
};

} // namespace MaterialX

#endif
