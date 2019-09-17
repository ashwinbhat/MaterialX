//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/DefaultUnitSystem.h>

#include <MaterialXGenShader/ShaderGenerator.h>

namespace MaterialX
{

const string DefaultUnitSystem::US_NAME = "default_us";

//
// DefaultUnitSystem methods
//

string DefaultUnitSystem::getImplementationName(const UnitTransform& /*transform*/) const
{
    //return "IM_" + transform.sourceUnit + "_to_" + transform.targetUnit + "_" + transform.type->getName() + "_" + _language;
    return "IM_distance_unit_float_genglsl";
}

DefaultUnitSystemPtr DefaultUnitSystem::create(const string& language)
{
    DefaultUnitSystemPtr result(new DefaultUnitSystem(language));
    return result;
}

DefaultUnitSystem::DefaultUnitSystem(const string& language)
    : UnitSystem()
{
    _language = createValidName(language);
}

} // namespace MaterialX
