//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/UnitSystem.h>

#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderGenerator.h>
#include <MaterialXGenShader/Nodes/SourceCodeNode.h>

namespace MaterialX
{
//
// Unit transform methods
//

UnitTransform::UnitTransform(const string& ss, const string& ts, const TypeDesc* t) :
    sourceSpace(ss),
    targetSpace(ts),
    type(t)
{
    if (type != Type::COLOR3 && type != Type::COLOR4)
    {
        throw ExceptionShaderGenError("Unit space transform can only be a color3 or color4.");
    }
}


UnitSystem::UnitSystem()
{
}

void UnitSystem::loadLibrary(DocumentPtr document)
{
    _document = document;
}

bool UnitSystem::supportsTransform(const UnitTransform& transform) const
{
    const string implName = getImplementationName(transform);
    ImplementationPtr impl = _document->getImplementation(implName);
    return impl != nullptr;
}

ShaderNodePtr UnitSystem::createNode(const ShaderGraph* parent, const UnitTransform& transform, const string& name,
    GenContext& context) const
{
    const string implName = getImplementationName(transform);
    ImplementationPtr impl = _document->getImplementation(implName);
    if (!impl)
    {
        throw ExceptionShaderGenError("No implementation found for transform: ('" + transform.sourceSpace + "', '" + transform.targetSpace + "').");
    }

    // Check if it's created and cached already,
    // otherwise create and cache it.
    ShaderNodeImplPtr nodeImpl = context.findNodeImplementation(implName);
    if (!nodeImpl)
    {
        nodeImpl = SourceCodeNode::create();
        nodeImpl->initialize(*impl, context);
        context.addNodeImplementation(implName, nodeImpl);
    }

    // Create the node.
    ShaderNodePtr shaderNode = ShaderNode::create(parent, name, nodeImpl, ShaderNode::Classification::TEXTURE);

    // Create ports on the node.
    ShaderInput* input = shaderNode->addInput("in", transform.type);
    if (transform.type == Type::COLOR3)
    {
        input->setValue(Value::createValue(Color3(0.0f, 0.0f, 0.0f)));
    }
    else if (transform.type == Type::COLOR4)
    {
        input->setValue(Value::createValue(Color4(0.0f, 0.0f, 0.0f, 1.0)));
    }
    else
    {
        throw ExceptionShaderGenError("Invalid type specified to createColorTransform: '" + transform.type->getName() + "'");
    }
    shaderNode->addOutput("out", transform.type);

    return shaderNode;
}

} // namespace MaterialX
