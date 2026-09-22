//
// Copyright Contributors to the MaterialX Project
// SPDX-License-Identifier: Apache-2.0
//

#include <MaterialXGenWgsl/WgslResourceBindingContext.h>

#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderGenerator.h>
#include <MaterialXGenShader/ShaderStage.h>
#include <MaterialXGenShader/Syntax.h>

MATERIALX_NAMESPACE_BEGIN

WgslResourceBindingContext::WgslResourceBindingContext(size_t group) :
    _group(group),
    _binding(0)
{
}

void WgslResourceBindingContext::initialize()
{
    _binding = 0;
}

void WgslResourceBindingContext::emitDirectives(GenContext&, ShaderStage&)
{
    // WGSL has no preprocessor directives for resource binding.
}

void WgslResourceBindingContext::emitResourceBindings(GenContext& context, const VariableBlock& uniforms, ShaderStage& stage)
{
    const ShaderGenerator& generator = context.getShaderGenerator();
    const Syntax& syntax = generator.getSyntax();
    const string groupStr = std::to_string(_group);

    if (context.getOptions().hwUniformLayout == UNIFORM_LAYOUT_STRUCT)
    {
        // Struct-packed mode: collect value uniforms into a struct, emit textures individually.
        vector<const ShaderPort*> valueUniforms;
        for (const ShaderPort* uniform : uniforms.getVariableOrder())
        {
            const TypeDesc type = uniform->getType();
            if (type.isClosure())
                continue;
            if (type == Type::FILENAME)
            {
                const string& name = uniform->getVariable();
                generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                                       ") var " + name + "_texture: texture_2d<f32>",
                                   stage);
                generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                                       ") var " + name + "_sampler: sampler",
                                   stage);
                continue;
            }
            valueUniforms.push_back(uniform);
        }

        if (!valueUniforms.empty())
        {
            // Resolve token names to literal identifiers for struct member names.
            const StringMap& tokenSubs = generator.getTokenSubstitutions();

            generator.emitLine("struct " + uniforms.getName() + " ", stage, false);
            generator.emitScopeBegin(stage);
            for (size_t i = 0; i < valueUniforms.size(); ++i)
            {
                const ShaderPort* port = valueUniforms[i];
                string typeName = syntax.getTypeName(port->getType());
                if (port->getType() == Type::BOOLEAN)
                    typeName = "u32";
                if (port->getType().isArray() && port->getValue())
                    typeName = "array<" + typeName + ", " + std::to_string(port->getValue()->asA<vector<float>>().size()) + ">";

                // Resolve the member name: if it's a $-token, look up the substitution.
                string memberName = port->getVariable();
                if (!memberName.empty() && memberName[0] == '$')
                {
                    auto it = tokenSubs.find(memberName);
                    if (it != tokenSubs.end())
                        memberName = it->second;
                }
                // Strip any struct instance prefix (e.g. "u_prv.u_envMatrix" -> "u_envMatrix")
                // that may have been applied by applyStructUniformTokenOverrides.
                const string& inst = uniforms.getInstance();
                const string prefix = inst + ".";
                if (memberName.size() > prefix.size() && memberName.substr(0, prefix.size()) == prefix)
                    memberName = memberName.substr(prefix.size());

                const string comma = (i + 1 < valueUniforms.size()) ? "," : "";
                generator.emitLine("    " + memberName + ": " + typeName + comma, stage, false);
            }
            generator.emitScopeEnd(stage, false, false);
            generator.emitLineBreak(stage);

            generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                                   ") var<uniform> " + uniforms.getInstance() + ": " + uniforms.getName(),
                               stage);
        }
    }
    else
    {
        // Individual mode: each uniform gets its own binding.
        for (const ShaderPort* uniform : uniforms.getVariableOrder())
        {
            const TypeDesc type = uniform->getType();
            if (type.isClosure())
                continue;

            const string& name = uniform->getVariable();

            if (type == Type::FILENAME)
            {
                generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                                       ") var " + name + "_texture: texture_2d<f32>",
                                   stage);
                generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                                       ") var " + name + "_sampler: sampler",
                                   stage);
                continue;
            }

            string typeName = syntax.getTypeName(type);
            if (type == Type::BOOLEAN)
                typeName = "u32";
            if (type.isArray() && uniform->getValue())
                typeName = "array<" + typeName + ", " + std::to_string(uniform->getValue()->asA<vector<float>>().size()) + ">";
            generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                                   ") var<uniform> " + name + ": " + typeName,
                               stage);
        }
    }
    generator.emitLineBreak(stage);
}

void WgslResourceBindingContext::emitStructuredResourceBindings(GenContext& context, const VariableBlock& uniforms,
                                                                ShaderStage& stage, const string& structInstanceName,
                                                                const string& arraySuffix)
{
    const ShaderGenerator& generator = context.getShaderGenerator();
    const Syntax& syntax = generator.getSyntax();
    const string groupStr = std::to_string(_group);

    // Emit the element struct definition.
    generator.emitLine("struct " + uniforms.getName() + " ", stage, false);
    generator.emitScopeBegin(stage);
    const auto& order = uniforms.getVariableOrder();
    for (size_t i = 0; i < order.size(); ++i)
    {
        const ShaderPort* port = order[i];
        string typeName = syntax.getTypeName(port->getType());
        if (port->getType() == Type::BOOLEAN)
        {
            typeName = "u32";
        }
        if (port->getType().isArray() && port->getValue())
        {
            typeName = "array<" + typeName + ", " + std::to_string(port->getValue()->asA<vector<float>>().size()) + ">";
        }
        const string comma = (i + 1 < order.size()) ? "," : "";
        generator.emitLine("    " + port->getVariable() + ": " + typeName + comma, stage, false);
    }
    generator.emitScopeEnd(stage, false, false);
    generator.emitLineBreak(stage);

    // Convert a GLSL-style "[N]" suffix into a WGSL array<T, N> store type.
    string storeType = uniforms.getName();
    if (!arraySuffix.empty() && arraySuffix.front() == '[' && arraySuffix.back() == ']')
    {
        const string count = arraySuffix.substr(1, arraySuffix.size() - 2);
        storeType = "array<" + uniforms.getName() + ", " + count + ">";
    }

    // Bind the structured array (e.g. the light-data array) as read-only storage
    // rather than uniform: the WGSL uniform address space requires array element
    // strides to be a multiple of 16, which a minimal element struct (e.g. a single
    // i32 light_type) does not satisfy. The storage address space relaxes this and
    // is the idiomatic choice for variable-length light arrays.
    generator.emitLine("@group(" + groupStr + ") @binding(" + std::to_string(_binding++) +
                           ") var<storage, read> " + structInstanceName + ": " + storeType,
                       stage);
    generator.emitLineBreak(stage);
}

MATERIALX_NAMESPACE_END
