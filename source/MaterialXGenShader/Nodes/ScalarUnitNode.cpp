//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/Nodes/ScalarUnitNode.h>
#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderNode.h>
#include <MaterialXGenShader/ShaderStage.h>
#include <MaterialXGenShader/ShaderGenerator.h>
#include <MaterialXGenShader/TypeDesc.h>

namespace MaterialX
{
#if 0
    void ScalarUnitNode::initialize(const InterfaceElement&, GenContext& context)
    {
        // Store name and hash
        _name = SCALAR_UNIT_FUNCTION;
        _hash = std::hash<string>{}(_name);
    };
#endif

    ShaderNodeImplPtr ScalarUnitNode::create(/*LinearUnitConverterPtr scalarUnitConverter*/)
    {
        return std::make_shared<ScalarUnitNode>();
    }

    void ScalarUnitNode::emitFunctionDefinition(const ShaderNode&, GenContext& context, ShaderStage& stage) const
    {
        UnitSystemPtr activeUnitSystem = context.getShaderGenerator().getUnitSystem();
        UnitConverterRegistryPtr unitRegistry = activeUnitSystem->getUnitConverterRegistry();
        
        LinearUnitConverterPtr _scalarUnitConverter;
        for (auto const &converter : unitRegistry->getUnitConverters())
        {
            if (std::dynamic_pointer_cast<LinearUnitConverter>(converter))
            {
                _scalarUnitConverter = std::dynamic_pointer_cast<LinearUnitConverter>(converter);
                break;
            }
        }
    
        if (_scalarUnitConverter)
        {
            BEGIN_SHADER_STAGE(stage, Stage::PIXEL)
            {
                // Emit the helper funtion mx_<unittype>_unit_ratio that embeds a look up table for unit scale. 
                // This needs to be done only once.
                vector<float> unitScales;
                unitScales.reserve(_scalarUnitConverter->getUnitScale().size());
                auto unitScaleMap = _scalarUnitConverter->getUnitScale();
                unitScales.resize(unitScaleMap.size());
                for (auto unitScale : unitScaleMap)
                {
                    int location = _scalarUnitConverter->getUnitAsInteger(unitScale.first);
                    unitScales[location] = unitScale.second;
                }
                // See stdlib/gen*/mx_<unittype>_unit. This helper function is called by these shaders.
                const string VAR_UNIT_SCALE = "u_" + _scalarUnitConverter->getUnitType() + "_unit_scales";
                VariableBlock unitLUT("unitLUT", EMPTY_STRING);
                ScopedFloatFormatting fmt(Value::FloatFormatFixed, 15);
                unitLUT.add(Type::FLOATARRAY, VAR_UNIT_SCALE, Value::createValue<vector<float>>(unitScales));

                const ShaderGenerator& shadergen = context.getShaderGenerator();
                shadergen.emitString("float mx_" + _scalarUnitConverter->getUnitType() + "_unit_ratio(int unit_from, int unit_to)", stage);
                shadergen.emitLineBreak(stage);
                shadergen.emitScopeBegin(stage);

                shadergen.emitLineBreak(stage);
                shadergen.emitVariableDeclarations(unitLUT, shadergen.getSyntax().getConstantQualifier(), ";", context, stage, true);

                shadergen.emitLineBreak(stage);
                shadergen.emitString("return (" + VAR_UNIT_SCALE + "[unit_from] / " + VAR_UNIT_SCALE + "[unit_to]);", stage);
                shadergen.emitLineBreak(stage);
                shadergen.emitScopeEnd(stage);
                shadergen.emitLineBreak(stage);

            }
            END_SHADER_STAGE(shader, Stage::PIXEL)
        }

    }

} // namespace MaterialX
