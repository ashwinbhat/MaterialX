//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_SCALERUNITNODE_H
#define MATERIALX_SCALERUNITNODE_H

#include <MaterialXGenShader/Nodes/SourceCodeNode.h>
#include <MaterialXGenShader/UnitSystem.h>
namespace MaterialX
{

    // Helper class to create the constant block for mx_<unittype>_unit
    class ScalarUnitNode : public SourceCodeNode
    {
    public:
        
        explicit ScalarUnitNode() 
        {
        }
        
        static ShaderNodeImplPtr create();

        void emitFunctionDefinition(const ShaderNode& node, GenContext& context, ShaderStage& stage) const override;

        static constexpr char SCALAR_UNIT_FUNCTION[] = "mx_distance_unit_ratio";
    };

} // namespace MaterialX

#endif
